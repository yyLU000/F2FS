```bash
sequential_write:
IOPS=740, BW=92.5MiB/s
util=98.09%

rand_write:
IOPS=263, BW=4210KiB/s
99.00th=[574620], 99.50th=[583009], 99.90th=[666895], 99.95th=[734004], 99.99th=[876610]
util=44.37%



Utilization: 69% (787208 valid blocks, 1764 discard blocks)
  - Node: 775 (Inode: 2, Other: 773)
  - Data: 786433
  - Inline_xattr Inode: 1
  - Inline_data Inode: 0
  - Inline_dentry Inode: 0
  - Compressed Inode: 0, Blocks: 0
  - Orphan/Append/Update Inode: 0, 0, 0

Main area: 2512 segs, 157 secs 157 zones
    TYPE            segno    secno   zoneno  dirty_seg   full_seg  valid_blk
  - COLD   data:      611       38       38          1          3       1764
  - WARM   data:      953       59       59       1109       1029     784668
  - HOT    data:       48        3        3          1          0          1
  - Dir   dnode:        0        0        0          1          0          1
  - File  dnode:       20        1        1          5          0        773
  - Indir nodes:       32        2        2          1          0          1
  - Pinned file:       -1       -1       -1
  - ATGC   data:       -1       -1       -1

  - Valid: 1038
  - Dirty: 1112
  - Prefree: 16
  - Free: 346 (17)

GC calls: 21082 (BG: 2)
  - data segments : 337312 (16)
  - node segments : 0 (0)
  - Reclaimed segs : Normal (337312), Idle CB (0), Idle Greedy (0), Idle AT (0), Urgent High (0), Urgent Low (0)
Try to move 1764 blocks (BG: 0)
  - data blocks : 1764 (0)
  - node blocks : 0 (0)

dirty_segments: 1112
free_segments: 346
```



```bash
sequential write:
IOPS=556, BW=69.5MiB/s

random write:
IOPS=263, BW=4210KiB/s

99.00th=[566232], 99.50th=[583009], 99.90th=[641729], 99.95th=[692061], 99.99th=[775947]

Utilization: 69% (787208 valid blocks, 1780 discard blocks)
  - Node: 775 (Inode: 2, Other: 773)
  - Data: 786433
  - Inline_xattr Inode: 1
  - Inline_data Inode: 0
  - Inline_dentry Inode: 0
  - Compressed Inode: 0, Blocks: 0
  - Orphan/Append/Update Inode: 0, 0, 0

Main area: 2512 segs, 157 secs 157 zones
    TYPE            segno    secno   zoneno  dirty_seg   full_seg  valid_blk
  - COLD   data:      614       38       38          1          6       3536
  - WARM   data:      953       59       59       1096       1026     782896
  - HOT    data:       48        3        3          1          0          1
  - Dir   dnode:        0        0        0          1          0          1
  - File  dnode:       20        1        1          5          0        773
  - Indir nodes:       32        2        2          1          0          1
  - Pinned file:       -1       -1       -1
  - ATGC   data:       -1       -1       -1

  - Valid: 1038
  - Dirty: 1099
  - Prefree: 16
  - Free: 359 (18)

GC calls: 21339 (BG: 5)
  - data segments : 341430 (54)
  - node segments : 0 (0)
  - Reclaimed segs : Normal (341430), Idle CB (0), Idle Greedy (0), Idle AT (0), Urgent High (0), Urgent Low (0)


dirty_segments: 1131
free_segments: 349

```
