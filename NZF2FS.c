/*
 * F2FS GC Optimization Based on NZF2FS Approach
 * For Linux Kernel 5.15.99
 * 
 * This implementation focuses on three main optimization directions:
 * 1. Zone classification by data lifetime (Transfer/Hot/Warm/GC_Hot/GC_Warm/Cold)
 * 2. Improved GC benefit calculation for periodic garbage collection
 * 3. Invalid block fragmentation reduction and migration cost optimization
 */

/* 
 * PART 1: Zone Type Definitions and Structures
 * Define new zone types and extend F2FS structures to support them
 */

// Include in fs/f2fs/f2fs.h
enum {
    // Original zone types
    LFS = 0,
    SSR = 1,
    
    // New zone types for optimization
    TRANSFER = 2,  // For newly written data
    HOT = 3,       // For frequently accessed data
    WARM = 4,      // For moderately accessed data
    GC_HOT = 5,    // Hot data from GC
    GC_WARM = 6,   // Warm data from GC
    COLD = 7,      // Rarely accessed data
};

// Add to struct f2fs_sb_info in fs/f2fs/f2fs.h
struct f2fs_sb_info {
    /* ... existing fields ... */
    
    /* Zone management */
    struct f2fs_zone_info {
        struct list_head transfer_list;    /* list of transfer segments */
        struct list_head hot_list;         /* list of hot segments */
        struct list_head warm_list;        /* list of warm segments */
        struct list_head gc_hot_list;      /* list of gc hot segments */
        struct list_head gc_warm_list;     /* list of gc warm segments */
        struct list_head cold_list;        /* list of cold segments */
        
        unsigned int transfer_segno;       /* current transfer segment */
        unsigned int hot_segno;            /* current hot segment */
        unsigned int warm_segno;           /* current warm segment */
        unsigned int gc_hot_segno;         /* current gc hot segment */
        unsigned int gc_warm_segno;        /* current gc warm segment */
        unsigned int cold_segno;           /* current cold segment */
        
        unsigned int transfer_blkoff;      /* next block offset in transfer */
        unsigned int hot_blkoff;           /* next block offset in hot */
        unsigned int warm_blkoff;          /* next block offset in warm */
        unsigned int gc_hot_blkoff;        /* next block offset in gc hot */
        unsigned int gc_warm_blkoff;       /* next block offset in gc warm */
        unsigned int cold_blkoff;          /* next block offset in cold */
        
        /* Zone thresholds and counters */
        unsigned long access_threshold;    /* threshold for hot/warm/cold classification */
        atomic_t transfer_count;           /* count of blocks in transfer zone */
        atomic_t hot_count;                /* count of blocks in hot zone */
        atomic_t warm_count;               /* count of blocks in warm zone */
        atomic_t gc_hot_count;             /* count of blocks in gc hot zone */
        atomic_t gc_warm_count;            /* count of blocks in gc warm zone */
        atomic_t cold_count;               /* count of blocks in cold zone */
    } zone_info;
    
    /* ... */
};

/* 
 * PART 2: Data Lifetime Tracking Mechanisms
 * Track block access frequency to classify data into appropriate zones
 */

// Add to struct f2fs_inode_info in fs/f2fs/f2fs.h
struct f2fs_inode_info {
    /* ... existing fields ... */
    
    /* Block access tracking */
    struct {
        unsigned long *access_map;         /* bitmap tracking block accesses */
        unsigned long last_access;         /* time of last access (jiffies) */
        unsigned int access_count;         /* number of accesses since creation */
        u8 zone_type;                      /* current zone classification */
    } lifetime_info;
    
    /* ... */
};

/*
 * PART 3: Zone-based Block Allocation Implementation
 * Implementation of block allocation according to zone types
 */

/**
 * get_zone_type - Determine the zone type for a data block
 * @inode: inode to which the block belongs
 * @blkaddr: block address to check
 *
 * Returns the appropriate zone type based on access patterns
 */
static int get_zone_type(struct inode *inode, block_t blkaddr)
{
    struct f2fs_inode_info *fi = F2FS_I(inode);
    struct f2fs_sb_info *sbi = F2FS_SB(inode->i_sb);
    unsigned long access_time = fi->lifetime_info.last_access;
    unsigned int access_count = fi->lifetime_info.access_count;
    unsigned long cur_time = jiffies;
    
    /* New blocks go to transfer zone */
    if (blkaddr == NEW_ADDR)
        return TRANSFER;
    
    /* Recently accessed blocks are hot */
    if (time_before(cur_time, access_time + sbi->zone_info.access_threshold))
        return HOT;
    
    /* Frequently accessed blocks are warm */
    if (access_count > 5)
        return WARM;
    
    /* Rarely accessed blocks are cold */
    return COLD;
}

/**
 * allocate_zone_block - Allocate a block in the appropriate zone
 * @sbi: super block info
 * @zone_type: type of zone to allocate from
 * 
 * Returns block address or error code
 */
static block_t allocate_zone_block(struct f2fs_sb_info *sbi, int zone_type)
{
    struct f2fs_zone_info *zi = &sbi->zone_info;
    unsigned int *segno_ptr;
    unsigned int *blkoff_ptr;
    struct list_head *seglist;
    block_t blkaddr;
    
    /* Determine segment and block offset pointers based on zone type */
    switch (zone_type) {
    case TRANSFER:
        segno_ptr = &zi->transfer_segno;
        blkoff_ptr = &zi->transfer_blkoff;
        seglist = &zi->transfer_list;
        break;
    case HOT:
        segno_ptr = &zi->hot_segno;
        blkoff_ptr = &zi->hot_blkoff;
        seglist = &zi->hot_list;
        break;
    case WARM:
        segno_ptr = &zi->warm_segno;
        blkoff_ptr = &zi->warm_blkoff;
        seglist = &zi->warm_list;
        break;
    case GC_HOT:
        segno_ptr = &zi->gc_hot_segno;
        blkoff_ptr = &zi->gc_hot_blkoff;
        seglist = &zi->gc_hot_list;
        break;
    case GC_WARM:
        segno_ptr = &zi->gc_warm_segno;
        blkoff_ptr = &zi->gc_warm_blkoff;
        seglist = &zi->gc_warm_list;
        break;
    case COLD:
        segno_ptr = &zi->cold_segno;
        blkoff_ptr = &zi->cold_blkoff;
        seglist = &zi->cold_list;
        break;
    default:
        /* Fallback to LFS for unrecognized zone types */
        return __get_free_data_block(sbi, NULL, 0);
    }
    
    /* Check if current segment has space */
    if (*segno_ptr != NULL_SEGNO && *blkoff_ptr < ENTRIES_IN_SEG) {
        blkaddr = START_BLOCK(*segno_ptr) + *blkoff_ptr;
        (*blkoff_ptr)++;
        return blkaddr;
    }
    
    /* Allocate a new segment for this zone */
    *segno_ptr = get_free_segment(sbi);
    if (IS_ERR_VALUE(*segno_ptr))
        return (block_t)*segno_ptr;
    
    /* Add the segment to the appropriate zone list */
    add_seg_entry(sbi, *segno_ptr, seglist);
    
    /* Reset block offset and return first block */
    *blkoff_ptr = 1; /* First block already being returned */
    return START_BLOCK(*segno_ptr);
}

/*
 * PART 4: Improved GC Benefit Calculation
 * Enhanced algorithms for GC victim selection and GC benefit calculation
 */

/**
 * calculate_gc_benefit - Calculate benefit of garbage collecting a segment
 * @sbi: super block info
 * @segno: segment number to evaluate
 *
 * Returns benefit score (higher means better candidate for GC)
 */
static unsigned int calculate_gc_benefit(struct f2fs_sb_info *sbi, unsigned int segno)
{
    struct sit_info *sit_i = SIT_I(sbi);
    struct seg_entry *se = get_seg_entry(sbi, segno);
    unsigned int vblocks = get_valid_blocks(sbi, segno, false);
    unsigned int invalid = ENTRIES_IN_SEG - vblocks;
    unsigned int age = get_segment_age(sbi, segno);
    unsigned long long cost, benefit;
    unsigned int fragmentation;
    
    if (vblocks == 0)
        return UINT_MAX; /* Free segment, highest priority */
    
    if (vblocks >= ENTRIES_IN_SEG - 1)
        return 1; /* Almost full, lowest priority */
    
    /* Calculate fragmentation level (0-100) */
    fragmentation = calculate_fragmentation_level(sbi, segno);
    
    /*
     * Benefit calculation factors:
     * 1. Invalid block count (more is better)
     * 2. Age of segment (older is better)
     * 3. Fragmentation level (more fragmented is better to clean)
     * 4. Cost of moving valid blocks (fewer valid blocks is better)
     */
    
    /* Cost is proportional to number of valid blocks to move */
    cost = vblocks * 100;
    
    /* Benefit increases with invalid blocks, age, and fragmentation */
    benefit = invalid * 100 + (age / 86400) * 50 + fragmentation * 30;
    
    /* Adjust benefit based on zone type */
    if (se->type == HOT || se->type == GC_HOT)
        benefit = benefit * 80 / 100; /* Reduce benefit for hot data */
    else if (se->type == COLD)
        benefit = benefit * 120 / 100; /* Increase benefit for cold data */
    
    /* Final benefit score */
    return benefit > cost ? benefit / cost : 1;
}

/**
 * calculate_fragmentation_level - Calculate fragmentation level of a segment
 * @sbi: super block info
 * @segno: segment number to evaluate
 *
 * Returns fragmentation level (0-100, higher means more fragmented)
 */
static unsigned int calculate_fragmentation_level(struct f2fs_sb_info *sbi, 
                                                 unsigned int segno)
{
    struct seg_entry *se = get_seg_entry(sbi, segno);
    unsigned int i, valid_count = 0;
    unsigned int frag_count = 0;
    unsigned long long frag_score = 0;
    unsigned char *cur_valid_map = se->cur_valid_map;
    
    for (i = 0; i < ENTRIES_IN_SEG; i++) {
        if (test_bit(i, (unsigned long *)cur_valid_map)) {
            valid_count++;
            
            /* Check for fragmentation (valid block surrounded by invalid ones) */
            if (i > 0 && !test_bit(i - 1, (unsigned long *)cur_valid_map))
                frag_count++;
                
            if (i < ENTRIES_IN_SEG - 1 && 
                !test_bit(i + 1, (unsigned long *)cur_valid_map))
                frag_count++;
        }
    }
    
    if (valid_count == 0)
        return 0;
    
    /* Calculate fragmentation score based on fragmented valid blocks */
    frag_score = frag_count * 100 / (valid_count * 2);
    return (unsigned int)frag_score;
}

/*
 * PART 5: GC Victim Selection and GC Execution Logic
 * Selects optimal segments for GC and performs efficient cleaning
 */

/**
 * get_victim_by_zone - Get victim segment by zone
 * @sbi: super block info
 * @segno: segment number to fill
 * @zone_type: zone type to prioritize
 * @gc_type: GC type (FG/BG)
 *
 * Returns segment number of victim or NULL_SEGNO if none found
 */
static int get_victim_by_zone(struct f2fs_sb_info *sbi, unsigned int *segno,
                          int zone_type, int gc_type)
{
    struct f2fs_zone_info *zi = &sbi->zone_info;
    struct list_head *seglist;
    struct seg_entry *se;
    unsigned int secno, max_benefit = 0, best_segno = NULL_SEGNO;
    unsigned int list_size = 0;
    
    /* Select appropriate zone list */
    switch (zone_type) {
    case TRANSFER:
        seglist = &zi->transfer_list;
        break;
    case HOT:
        seglist = &zi->hot_list;
        break;
    case WARM:
        seglist = &zi->warm_list;
        break;
    case GC_HOT:
        seglist = &zi->gc_hot_list;
        break;
    case GC_WARM:
        seglist = &zi->gc_warm_list;
        break;
    case COLD:
        seglist = &zi->cold_list;
        break;
    default:
        return -EINVAL;
    }
    
    if (list_empty(seglist))
        return -ENODATA;
    
    /* Scan segments in the list to find best victim */
    list_for_each_entry(se, seglist, list) {
        unsigned int this_segno = se->segno;
        unsigned int benefit;
        
        /* Skip clean segments */
        if (se->valid_blocks == 0)
            continue;
            
        /* Calculate GC benefit for this segment */
        benefit = calculate_gc_benefit(sbi, this_segno);
        
        /* Update best victim if this one has higher benefit */
        if (benefit > max_benefit) {
            max_benefit = benefit;
            best_segno = this_segno;
        }
        
        /* Limit search for background GC */
        if (gc_type == BG_GC && ++list_size >= 5)
            break;
    }
    
    if (best_segno != NULL_SEGNO) {
        *segno = best_segno;
        return 0;
    }
    
    return -ENODATA;
}

/**
 * get_gc_victim - Get victim segment for garbage collection
 * @sbi: super block info
 * @segno: segment number to fill
 * @gc_type: GC type (FG/BG)
 *
 * Enhanced victim selection that considers zone types
 * Returns 0 on success, error code on failure
 */
int get_gc_victim(struct f2fs_sb_info *sbi, unsigned int *segno, int gc_type)
{
    /* Try zones in order of priority for GC benefit */
    
    /* First priority: Cold zone (data unlikely to be accessed again) */
    if (get_victim_by_zone(sbi, segno, COLD, gc_type) == 0)
        return 0;
    
    /* Second priority: Transfer zone (short-lived data) */
    if (get_victim_by_zone(sbi, segno, TRANSFER, gc_type) == 0)
        return 0;
    
    /* Third priority: GC Warm zone (already gone through one GC) */
    if (get_victim_by_zone(sbi, segno, GC_WARM, gc_type) == 0)
        return 0;
    
    /* Fourth priority: Warm zone */
    if (get_victim_by_zone(sbi, segno, WARM, gc_type) == 0)
        return 0;
    
    /* Fifth priority: GC Hot zone (already gone through one GC) */
    if (get_victim_by_zone(sbi, segno, GC_HOT, gc_type) == 0)
        return 0;
    
    /* Last priority: Hot zone (frequently accessed) */
    if (get_victim_by_zone(sbi, segno, HOT, gc_type) == 0)
        return 0;
    
    /* Fall back to original method if no victims found */
    return __get_victim(sbi, segno, gc_type);
}

/*
 * PART 6: Optimized Block Migration for GC
 * Reduces cost of valid block migration during garbage collection
 */

/**
 * determine_migration_zone - Determine target zone for migrated blocks
 * @sbi: super block info
 * @inode: inode the block belongs to
 * @old_blkaddr: old block address
 * @from_zone: source zone type
 *
 * Returns appropriate zone for migrated blocks
 */
static int determine_migration_zone(struct f2fs_sb_info *sbi, struct inode *inode,
                               block_t old_blkaddr, int from_zone)
{
    struct f2fs_inode_info *fi = F2FS_I(inode);
    unsigned long access_time = fi->lifetime_info.last_access;
    unsigned long cur_time = jiffies;
    unsigned int access_count = fi->lifetime_info.access_count;
    
    /* Recently accessed blocks go to GC_HOT, regardless of source */
    if (time_before(cur_time, access_time + F2FS_GC_HOT_THRESHOLD(sbi)))
        return GC_HOT;
    
    /* Based on access count */
    if (access_count > F2FS_GC_WARM_THRESHOLD(sbi))
        return GC_WARM;
    
    /* Cold data stays cold */
    if (from_zone == COLD)
        return COLD;
    
    /* Default to GC_WARM for other cases */
    return GC_WARM;
}

/**
 * f2fs_migrate_block - Migrate a valid block during GC
 * @sbi: super block info
 * @seg_entry: segment entry containing the block
 * @newblkaddr: new block address to migrate to
 * @old_blkaddr: old block address being migrated
 * @summary: summary entry for the block
 *
 * Enhanced block migration with zone-awareness
 */
void f2fs_migrate_block(struct f2fs_sb_info *sbi, struct seg_entry *se,
                  block_t newblkaddr, block_t old_blkaddr, 
                  struct f2fs_summary *sum)
{
    struct inode *inode;
    pgoff_t index;
    struct f2fs_io_info fio = {
        .sbi = sbi,
        .type = DATA,
        .temp = HOT,
        .op = REQ_OP_READ,
    };
    int from_zone, to_zone;
    struct node_info ni;
    
    /* Get node info from summary */
    get_node_info(sbi, le32_to_cpu(sum->nid), &ni);
    
    /* Look up inode */
    inode = f2fs_iget(sbi->sb, le32_to_cpu(sum->nid));
    if (IS_ERR(inode))
        goto skip_node_update;
    
    /* Extract index from summary */
    index = le32_to_cpu(sum->blk_addr);
    
    /* Determine source and target zones */
    from_zone = se->type;
    to_zone = determine_migration_zone(sbi, inode, old_blkaddr, from_zone);
    
    /* Set up I/O parameters */
    fio.op = REQ_OP_READ;
    fio.op_flags = REQ_SYNC;
    fio.new_blkaddr = newblkaddr;
    fio.old_blkaddr = old_blkaddr;
    fio.page = f2fs_grab_meta_page(sbi, old_blkaddr);
    
    /* Read data from old block */
    f2fs_submit_page_bio(&fio);
    
    /* Write to new block */
    fio.op = REQ_OP_WRITE;
    f2fs_submit_page_bio(&fio);
    
    /* Update node info to point to new block address */
    update_extent_cache(newblkaddr, inode, index);
    
    /* Update node page if needed */
    if (index < DIRECT_NODE_BLOCKS)
        update_inode_page(inode);
    else
        update_indirect_node_page(inode, index);
    
    iput(inode);

skip_node_update:
    /* Update segment type for the new block */
    update_segment_type(sbi, newblkaddr, to_zone);
}

/*
 * PART 7: Zone Management and Initialization
 * Initializes and manages zone structures
 */

/**
 * init_zone_info - Initialize zone management info
 * @sbi: super block info
 */
int init_zone_info(struct f2fs_sb_info *sbi)
{
    struct f2fs_zone_info *zi = &sbi->zone_info;
    
    /* Initialize zone lists */
    INIT_LIST_HEAD(&zi->transfer_list);
    INIT_LIST_HEAD(&zi->hot_list);
    INIT_LIST_HEAD(&zi->warm_list);
    INIT_LIST_HEAD(&zi->gc_hot_list);
    INIT_LIST_HEAD(&zi->gc_warm_list);
    INIT_LIST_HEAD(&zi->cold_list);
    
    /* Initialize zone segment numbers */
    zi->transfer_segno = NULL_SEGNO;
    zi->hot_segno = NULL_SEGNO;
    zi->warm_segno = NULL_SEGNO;
    zi->gc_hot_segno = NULL_SEGNO;
    zi->gc_warm_segno = NULL_SEGNO;
    zi->cold_segno = NULL_SEGNO;
    
    /* Initialize zone block offsets */
    zi->transfer_blkoff = 0;
    zi->hot_blkoff = 0;
    zi->warm_blkoff = 0;
    zi->gc_hot_blkoff = 0;
    zi->gc_warm_blkoff = 0;
    zi->cold_blkoff = 0;
    
    /* Initialize zone thresholds */
    zi->access_threshold = msecs_to_jiffies(F2FS_DEF_HOT_PERIOD_MS);
    
    /* Initialize zone counters */
    atomic_set(&zi->transfer_count, 0);
    atomic_set(&zi->hot_count, 0);
    atomic_set(&zi->warm_count, 0);
    atomic_set(&zi->gc_hot_count, 0);
    atomic_set(&zi->gc_warm_count, 0);
    atomic_set(&zi->cold_count, 0);
    
    return 0;
}

/**
 * update_segment_type - Update segment type when allocating or migrating
 * @sbi: super block info
 * @blkaddr: block address
 * @type: new segment type
 */
void update_segment_type(struct f2fs_sb_info *sbi, block_t blkaddr, int type)
{
    unsigned int segno = GET_SEGNO(sbi, blkaddr);
    struct seg_entry *se = get_seg_entry(sbi, segno);
    
    /* Update segment type */
    se->type = type;
    
    /* Update zone counters */
    switch (type) {
    case TRANSFER:
        atomic_inc(&sbi->zone_info.transfer_count);
        break;
    case HOT:
        atomic_inc(&sbi->zone_info.hot_count);
        break;
    case WARM:
        atomic_inc(&sbi->zone_info.warm_count);
        break;
    case GC_HOT:
        atomic_inc(&sbi->zone_info.gc_hot_count);
        break;
    case GC_WARM:
        atomic_inc(&sbi->zone_info.gc_warm_count);
        break;
    case COLD:
        atomic_inc(&sbi->zone_info.cold_count);
        break;
    }
}

/*
 * PART 8: Hook into Existing F2FS Functions
 * Integrate the optimizations with the existing F2FS code
 */

/* 
 * In fs/f2fs/data.c, modify __allocate_data_block to use zone-based allocation
 */
block_t __allocate_data_block(struct f2fs_sb_info *sbi,
                         struct inode *inode,
                         block_t blkaddr)
{
    struct f2fs_inode_info *fi = F2FS_I(inode);
    block_t new_blkaddr;
    int zone_type;
    
    /* Determine appropriate zone type for this block */
    zone_type = get_zone_type(inode, blkaddr);
    
    /* Allocate block from the determined zone */
    new_blkaddr = allocate_zone_block(sbi, zone_type);
    
    /* If zone allocation failed, fall back to original method */
    if (IS_ERR_VALUE(new_blkaddr))
        return __get_free_data_block(sbi, NULL, 0);
    
    /* Update segment type for the new block */
    update_segment_type(sbi, new_blkaddr, zone_type);
    
    return new_blkaddr;
}

/* 
 * In fs/f2fs/gc.c, modify f2fs_gc to use the enhanced victim selection
 */
int f2fs_gc(struct f2fs_sb_info *sbi, bool sync, bool background)
{
    unsigned int segno, i;
    int gc_type = sync ? FG_GC : BG_GC;
    int sec_freed = 0, seg_freed = 0, total_freed = 0;
    int ret = 0;
    struct cp_control cpc;
    
    /* Use our enhanced get_gc_victim function instead of original */
    ret = get_gc_victim(sbi, &segno, gc_type);
    if (ret)
        return ret;
    
    /* Rest of the function remains similar to original */
    cpc.reason = __get_cp_reason(sbi);
    sbi->cur_victim_sec = GET_SEC_FROM_SEG(sbi, segno);
    
    for (i = 0; i < sbi->segs_per_sec; i++) {
        /*
         * For each segment in the section, perform GC
         * using our enhanced block migration
         */
        ret = do_garbage_collect(sbi, segno + i, &sec_freed);
        if (ret)
            break;
        
        if (sec_freed)
            break;
        
        seg_freed++;
    }
    
    if (!ret)
        total_freed = sec_freed ? free_sections(sbi, sbi->cur_victim_sec, 
                                              sbi->segs_per_sec) : seg_freed;
    
    return total_freed;
}

/* 
 * In fs/f2fs/f2fs_format.h, add constants for zone thresholds
 */
#define F2FS_DEF_HOT_PERIOD_MS      60000   /* 1 minute = hot data */
#define F2FS_GC_HOT_THRESHOLD(sbi)  msecs_to_jiffies(60000)
#define F2FS_GC_WARM_THRESHOLD(sbi) 10      /* 10 accesses = warm data */
