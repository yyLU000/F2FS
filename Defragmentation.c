struct f2fs_gc_defrag_info {
    unsigned int invalid_blocks;     /* 无效块数量 */
    unsigned int invalid_clusters;   /* 无效块集群数量 */
    unsigned int fragmentation_score; /* 碎片化分数 (0-100) */
    bool high_fragmentation;         /* 是否高度碎片化 */
};

fragmentation_score = invalid_clusters * 100 / total_invalid;    /* 无效集群数 / 无效块总数 */
high_fragmentation: fragmentation > 50 ? true : false

fragmentation_score     weight 
      70-100              60
      40-70               80 
      0-40                110

cost *= weight / 100;
