/*
 * f2fs_gc_defrag.c
 * 
 * F2FS垃圾回收优化方案：减少无效块碎片化的影响
 * 基于Linux 5.15.179内核
 */

#include <linux/fs.h>
#include <linux/f2fs_fs.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/blkdev.h>
#include <linux/bio.h>
#include <linux/prefetch.h>
#include <linux/types.h>
#include <linux/backing-dev.h>
#include <linux/kthread.h>
#include <linux/sched/signal.h>

#include "f2fs.h"
#include "node.h"
#include "segment.h"
#include "gc.h"


extern void segno_from_rbtree_wb(struct f2fs_sb_info *sbi,
        struct victim_sel_policy *p);
extern unsigned int get_gc_cost(struct f2fs_sb_info *sbi, unsigned int segno,
                               struct victim_sel_policy *p);
extern int do_garbage_collect(struct f2fs_sb_info *sbi, 
                             unsigned int start_segno,
                             struct gc_inode_list *gc_list, 
                             int gc_type,
                             bool force_migrate);
extern int f2fs_get_node_info(struct f2fs_sb_info *sbi, nid_t nid,
                                struct node_info *ni, bool checkpoint_context);
extern unsigned int get_valid_blocks(struct f2fs_sb_info *sbi,
                                    unsigned int segno, bool use_section);
extern int f2fs_get_free_segments(struct f2fs_sb_info *sbi);

static int defragment_segment(struct f2fs_sb_info *sbi, unsigned int segno,
                             struct f2fs_gc_defrag_info *defrag_info);

#define GET_SEGNO_FROM_SEC(sbi, secno) ((secno) * (sbi)->segs_per_sec)

#define for_each_dirty_segment(sbi, p, reverse) \
    for_each_set_bit_from(p->min_segno, (sbi)->dirty_segments_bitmap, \
                         MAIN_SEGS(sbi))

#define for_each_dirty_segment_ckpt(sbi, p, reverse) for_each_dirty_segment(sbi, p, reverse)

/**
 * 评估段内无效块的碎片化程度
 * 返回碎片化程度评分 (0-100)，值越大表示碎片化越严重
 */
static unsigned int assess_segment_fragmentation(struct f2fs_sb_info *sbi,
                             struct f2fs_gc_defrag_info *defrag_info,
                             unsigned int segno)
{
    struct seg_entry *se = get_seg_entry(sbi, segno);
    char *bitmap = (char *)se->cur_valid_map;
    char *invalidmap;
    unsigned int i, prev_invalid, invalid_clusters, total_invalid;
    unsigned int blocks_per_seg = sbi->blocks_per_seg;
    unsigned int fragmentation;
    
    /* 创建无效块位图 */
    invalidmap = f2fs_kvzalloc(sbi, f2fs_bitmap_size(blocks_per_seg), GFP_KERNEL);
    if (!invalidmap)
        return 0;
    
    /* 构建无效块位图 (无效块设为1，有效块设为0) */
    for (i = 0; i < blocks_per_seg; i++) {
        if (!f2fs_test_bit(i, bitmap))
            f2fs_set_bit(i, invalidmap);
    }
    
    /* 统计无效块集群数量 */
    prev_invalid = 0;
    invalid_clusters = 0;
    total_invalid = 0;
    
    for (i = 0; i < blocks_per_seg; i++) {
        if (f2fs_test_bit(i, invalidmap)) {
            total_invalid++;
            
            /* 检测无效块集群的开始 */
            if (!prev_invalid)
                invalid_clusters++;
                
            prev_invalid = 1;
        } else {
            prev_invalid = 0;
        }
    }
    
    if (total_invalid == 0) {
        fragmentation = 0;
    } else {
        /* 计算碎片化得分: 无效块集群数 / 无效块总数 * 100 */
        fragmentation = invalid_clusters * 100 / total_invalid;
    }
    
    /* 填充碎片化信息结构体 */
    defrag_info->invalid_blocks = total_invalid;
    defrag_info->invalid_clusters = invalid_clusters;
    defrag_info->fragmentation_score = fragmentation;
    defrag_info->high_fragmentation = (fragmentation > 50) ? true : false;
    
    kvfree(invalidmap);
    return fragmentation;
}

/**
 * 基于碎片化程度增强的段选择策略
 */
static unsigned int get_gc_cost_with_defrag(struct f2fs_sb_info *sbi,
                          unsigned int segno,
                          struct victim_sel_policy *p)
{
    unsigned int cost, frag_weight;
    unsigned int frag_score;
    struct f2fs_gc_defrag_info defrag_info;
    
    cost = get_gc_cost(sbi, segno, p);
    
    /* 评估无效块的碎片化程度 */
    frag_score = assess_segment_fragmentation(sbi, &defrag_info, segno);
    
    /* 根据碎片化程度调整成本权重
     * - 碎片化高的段会降低成本，增加被选中的概率
     * - 原因: 选择高碎片化的段进行回收可以更有效地减少碎片 */
    if (frag_score > 70) {
        /* 高度碎片化: 显著降低成本 */
        frag_weight = 60;
    } else if (frag_score > 40) {
        /* 中度碎片化: 适度降低成本 */
        frag_weight = 80;
    } else {
        /* 低度碎片化: 略微增加成本 */
        frag_weight = 110;
    }
    
    /* 应用碎片化权重 */
    cost = cost * frag_weight / 100;
    
    return cost;
}

static bool for_each_dirty_segment_more(struct f2fs_sb_info *sbi,
                                        struct victim_sel_policy *p,
                                        unsigned int *nsearched,
                                        unsigned int limit)
{
    unsigned int segno;
    unsigned int cost, max_cost;
    struct f2fs_gc_defrag_info defrag_info;
    
    max_cost = 0;
    for (segno = 0; segno < MAIN_SEGS(sbi) && *nsearched < limit; segno++) {
        struct seg_entry *se = get_seg_entry(sbi, segno);
        
        /* 跳过不符合条件的段 */
        if (se->valid_blocks == 0 || se->valid_blocks == sbi->blocks_per_seg)
            continue;
            
        /* 评估成本 */
        cost = get_gc_cost(sbi, segno, p);
        
        /* 更新最小成本段 */
        if (cost < p->min_cost || p->min_segno == NULL_SEGNO) {
            p->min_cost = cost;
            p->min_segno = segno;
        }
        
        (*nsearched)++;
    }
    
    return (p->min_segno != NULL_SEGNO);
}


/**
 * 选择最佳的段用于垃圾回收，同时考虑碎片化因素 
 */
static void select_segments_with_defrag(struct f2fs_sb_info *sbi,
                    struct victim_sel_policy *p,
                    unsigned int *result)
{
    struct f2fs_gc_defrag_info defrag_info;
    struct dirty_seglist_info *dirty_i = DIRTY_I(sbi);
    unsigned int secno, last_victim_secno, last_victim_segno;
    unsigned int segno;
    unsigned int cost, max_cost, last_cost;
    unsigned int nsearched = 0;
    
    max_cost = 0;
    last_victim_secno = NULL_SECNO;
    last_victim_segno = NULL_SEGNO;
    last_cost = 0;
    
    mutex_lock(&dirty_i->seglist_lock);
    
    /* 搜索每个部分，选择最佳段 */
    p->alloc_mode = AT_SSR;
    p->min_segno = NULL_SEGNO;
    p->min_cost = max_cost;
    
    /* 搜索符合类型和状态的段 */
    if (p->gc_mode == GC_CB) {
        // 冷数据搜索逻辑
        select_victim_segments(sbi, p);
    } else if (p->gc_mode == GC_GREEDY) {
        // 贪婪搜索逻辑
        for_each_dirty_segment_ckpt(sbi, p, true);
    }
    
    mutex_unlock(&dirty_i->seglist_lock);
    
    if (p->min_segno == NULL_SEGNO)
        goto out;
        
    /* 考虑最低成本段的碎片化程度 */
    segno = p->min_segno;
    secno = GET_SEC_FROM_SEG(sbi, segno);
    
    /* 1. 评估碎片化程度 */
    assess_segment_fragmentation(sbi, &defrag_info, segno);
    
    /* 2. 如果高度碎片化，我们接受该段 */
    if (defrag_info.high_fragmentation) {
        *result = (p->min_segno << 1) | GET_SEGNO_FROM_SEC(sbi, secno);
        return;
    }
    
    /* 3. 否则，搜索更多段以寻找碎片化程度更高的段 */
    mutex_lock(&dirty_i->seglist_lock);

    secno = NULL_SECNO;
    segno = NULL_SEGNO;
    
    /* 扩展搜索，限制在前10个最佳候选段中 */
    nsearched = 0;
    if ( for_each_dirty_segment_more(sbi, p, &nsearched, 10) ) {
        unsigned int new_segno = p->min_segno;
        struct f2fs_gc_defrag_info new_defrag_info;
        
        /* 评估该段的碎片化程度 */
        assess_segment_fragmentation(sbi, &new_defrag_info, new_segno);
        
        /* 如果找到高度碎片化的段，立即选择它 */
        if (new_defrag_info.high_fragmentation) {
            segno = new_segno;
            break;
        }
        
        /* 比较碎片化得分，保留得分最高的段 */
        if (new_defrag_info.fragmentation_score > defrag_info.fragmentation_score) {
            defrag_info = new_defrag_info;
            segno = new_segno;
        }
    }
    
    mutex_unlock(&dirty_i->seglist_lock);
    
    /* 如果找到了更好的候选段，使用它 */
    if (segno != NULL_SEGNO) {
        secno = GET_SEC_FROM_SEG(sbi, segno);
        *result = (segno << 1) | GET_SEGNO_FROM_SEC(sbi, secno);
    } else {
        /* 否则回退到原始选择 */
        *result = (p->min_segno << 1) | GET_SEGNO_FROM_SEC(sbi, GET_SEC_FROM_SEG(sbi, p->min_segno));
    }
    
out:
    return;
}

static int get_gc_status(struct f2fs_sb_info *sbi)
{
	if ( get_free_segments(sbi) > sbi->reserved_segments )
		return 0;
	return 1;
}
#define GC_NONE 0
#define NO_CHECK_TYPE 0
#define LFS_MODE 1

/**
 * 优化的垃圾回收函数
 */
int f2fs_gc_with_defrag(struct f2fs_sb_info *sbi, bool sync,
                       bool background, bool force, unsigned int segno)
{
    unsigned int gc_type = sync ? FG_GC : BG_GC;
    struct victim_sel_policy p;
    struct gc_inode_list gc_list = {
        .ilist = LIST_HEAD_INIT(gc_list.ilist),
        .iroot = RADIX_TREE_INIT(GFP_NOFS),
    };
    unsigned int selected_segno, i;
    unsigned int result;
    int nfree, ret = -EINVAL;
    int gc_status;
    
    /* 初始化段选择策略 */
    memset(&p, 0, sizeof(struct victim_sel_policy));
    p.gc_mode = gc_type;
    /* ???  */

    /* 检查是否可以跳过GC */
    gc_status = get_gc_status(sbi);
    if (gc_status == GC_NONE)
        return 0;
    
    /* 
     * 如果传入了有效的segno，直接使用它
     * 否则使用我们的碎片化感知选择算法
     */
    if (segno != NULL_SEGNO) {
        /* 直接使用传入的segno */
        result = (segno << 1) | GET_SEGNO_FROM_SEC(sbi, GET_SEC_FROM_SEG(sbi, segno));
    } else {
        /* 选择受害者段，考虑碎片化因素 */
        select_segments_with_defrag(sbi, &p, &result);
    }
    
    if (IS_ALIGNED(result, 2)) {
        /* 单段GC */
        selected_segno = result >> 1;
        
        /* 现有的GC逻辑 */
        ret = do_garbage_collect(sbi, selected_segno, &gc_list, gc_type, force);
    } else {
        /* 多段GC (一个小节内) */
        unsigned int start_segno = START_SEGNO(result);
        unsigned int end_segno = start_segno + sbi->segs_per_sec;
        unsigned int cleaned = 0;
        
        for (selected_segno = start_segno; selected_segno < end_segno; selected_segno++) {
            /* 检查是否值得GC这个段 */
            struct f2fs_gc_defrag_info defrag_info;
            unsigned int frag_score = assess_segment_fragmentation(sbi, 
                                                                   &defrag_info, 
                                                                   selected_segno);
                                                                    
            /* 对于有效性低于阈值或高度碎片化的段，执行GC */
            if (get_valid_blocks(sbi, selected_segno, false) == 0 ||
                defrag_info.high_fragmentation) {
                ret = do_garbage_collect(sbi, selected_segno, &gc_list, gc_type, force);
                if (ret)
                    return ret;
                cleaned++;
            }
        }
        
        /* 如果没有段被清理，尝试强制清理最佳段 */
        if (cleaned == 0 && force) {
            unsigned int best_segno = start_segno;
            unsigned int min_valid = UINT_MAX;
            struct f2fs_gc_defrag_info best_defrag_info = {0};
            unsigned int best_score = 0;
            
            /* 找出最佳段进行GC */
            for (selected_segno = start_segno; selected_segno < end_segno; selected_segno++) {
                unsigned int valid_blocks = get_valid_blocks(sbi, selected_segno, false);
                struct f2fs_gc_defrag_info defrag_info;
                unsigned int frag_score = assess_segment_fragmentation(sbi, 
                                                                      &defrag_info, 
                                                                      selected_segno);
                
                /* 考虑碎片化程度和有效块数的组合得分 */
                unsigned int combined_score = (100 - valid_blocks * 100 / sbi->blocks_per_seg) +
                                             (frag_score / 2);
                                             
                if (combined_score > best_score) {
                    best_score = combined_score;
                    best_segno = selected_segno;
                    best_defrag_info = defrag_info;
                }
            }
            
            /* 对最佳段执行GC */
            ret = do_garbage_collect(sbi, best_segno, &gc_list, gc_type, force);
        }
    }
    
    return ret;
}


/**
 * 优化的段整合函数，主动合并碎片化的数据
 * 在GC后调用，合并高度碎片化但未被GC的段
 */
int f2fs_defrag_segments(struct f2fs_sb_info *sbi, unsigned int segno,
                          unsigned int count)
{
    struct f2fs_gc_defrag_info defrag_info;
    unsigned int i;
    int ret = 0;
    
    for (i = 0; i < count && i + segno < MAIN_SEGS(sbi); i++) {
        unsigned int cur_segno = segno + i;
        unsigned int frag_score;
        
        /* 评估段的碎片化程度 */
        frag_score = assess_segment_fragmentation(sbi, &defrag_info, cur_segno);
        
        /* 对高度碎片化的段执行整合 */
        if (frag_score > 60 && defrag_info.invalid_blocks > 0) {
            /* 对该段执行整合操作
             * 1. 读取所有有效块
             * 2. 将它们重新写入新的段
             * 3. 废弃旧段，标记待GC
             */
            ret = defragment_segment(sbi, cur_segno, &defrag_info);
            if (ret)
                break;
        }
    }
    
    return ret;
}

/**
 * 段整合实现函数
 * 对高度碎片化的段进行重整
 */
static int defragment_segment(struct f2fs_sb_info *sbi, unsigned int segno,
                              struct f2fs_gc_defrag_info *defrag_info)
{
    struct seg_entry *se = get_seg_entry(sbi, segno);
    char *bitmap = (char *)se->cur_valid_map;
    struct f2fs_summary_block *sum_blk;
    struct f2fs_summary *sum;
    struct page *sum_page;
    unsigned int valid_blocks = 0;
    unsigned int blocks_per_seg = sbi->blocks_per_seg;
    unsigned int i, blkoff;
    int ret = 0;
    
    /* 获取段的摘要信息 */
    sum_page = f2fs_get_sum_page(sbi, segno);
    if (IS_ERR(sum_page))
        return PTR_ERR(sum_page);
        
    sum_blk = (struct f2fs_summary_block *)page_address(sum_page);
    
    /* 统计有效块数 */
    for (i = 0; i < blocks_per_seg; i++) {
        if (f2fs_test_bit(i, bitmap))
            valid_blocks++;
    }
    
    /* 如果有效块太少或太多，不整合 */
    if (valid_blocks == 0 || valid_blocks == blocks_per_seg) {
        f2fs_put_page(sum_page, 1);
        return 0;
    }
    
    /* 为每个有效块分配新位置并迁移数据 */
    for (i = 0; i < blocks_per_seg; i++) {
        if (!f2fs_test_bit(i, bitmap))
            continue;
            
        sum = &sum_blk->entries[i];
        
        /* 读取原始数据块 */
        struct node_info ni;
        struct page *data_page;
    	struct inode *inode;
        
        ret = f2fs_get_node_info(sbi, le32_to_cpu(sum->nid), &ni);
        if (ret) {
            goto out;
        }
        
        inode = f2fs_iget(sbi->sb, ni.ino);
        if (IS_ERR(inode)) {
            ret = PTR_ERR(inode);
            goto out;
        }
        
        blkoff = i;
        
        data_page = f2fs_get_read_data_page(inode, blkoff, 0, true);
        if (IS_ERR(data_page)) {
            ret = PTR_ERR(data_page);
            iput(inode);
            goto out;
        }
        
        /* 分配新块并写入数据 */
        ret = relocate_data_block(sbi, data_page, sum, &ni);
        f2fs_put_page(data_page, 1);
        iput(inode);
        
        if (ret)
            goto out;
    }
    
    /* 标记原段为无效，等待后续GC */
    f2fs_invalidate_blocks(sbi, segno);
    
out:
    f2fs_put_page(sum_page, 1);
    return ret;
}

/**
 * 智能GC触发函数 - 根据碎片化程度自动触发GC
 */
void trigger_defrag_gc(struct f2fs_sb_info *sbi)
{
    unsigned int total_segs = MAIN_SEGS(sbi);
    unsigned int i, checked = 0;
    unsigned int high_frag_count = 0;
    unsigned int check_limit = min_t(unsigned int, 100, total_segs / 10);
    
    /* 随机检查一部分段的碎片化程度 */
    for (i = 0; i < check_limit; i++) {
        struct f2fs_gc_defrag_info defrag_info;
        unsigned int segno = get_random_u32() % total_segs;
        
        /* 评估段的碎片化程度 */
        assess_segment_fragmentation(sbi, &defrag_info, segno);
        checked++;
        
        if (defrag_info.high_fragmentation)
            high_frag_count++;
    }
    
    /* 如果高碎片化段比例超过阈值，触发GC */
    if (high_frag_count * 100 / checked > 30) {
        unsigned int result;
        f2fs_gc_with_defrag(sbi, false, true, false, NULL_SEGNO);
    }
}

/* 
 * 针对checkpoint操作的优化：在创建checkpoint前处理高度碎片化的段
 * 有助于减少F2FS的元数据负担
 */
int f2fs_defrag_before_checkpoint(struct f2fs_sb_info *sbi)
{
    unsigned int total_segs = MAIN_SEGS(sbi);
    unsigned int i, checked = 0;
    unsigned int defragged = 0;
    unsigned int check_limit = min_t(unsigned int, 50, total_segs / 20);
    int ret = 0;
    
    /* 不要对所有段都检查，只随机挑选一些 */
    for (i = 0; i < check_limit && !ret; i++) {
        struct f2fs_gc_defrag_info defrag_info;
        unsigned int segno = get_random_u32() % total_segs;
        unsigned int frag_score;
        
        /* 评估段的碎片化程度 */
        frag_score = assess_segment_fragmentation(sbi, &defrag_info, segno);
        checked++;
        
        /* 只处理严重碎片化的段 */
        if (frag_score > 80 && defrag_info.invalid_blocks > 0) {
            ret = defragment_segment(sbi, segno, &defrag_info);
            if (!ret)
                defragged++;
                
            /* 限制每次处理的段数量 */
            if (defragged >= 3)
                break;
        }
    }
    
    return ret;
}

