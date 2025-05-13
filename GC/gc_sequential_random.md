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


## Original
```bash
sequential write:
IOPS=1026, BW=128MiB/s

random write:
IOPS=869, BW=3476/s

99.00th=[17171], 99.50th=[19530], 99.90th=[20841], 99.95th=[23987], 99.99th=[69731]


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


dirty_segments: 1122
free_segments: 354

sequential read:
read_test: (groupid=0, jobs=1): err= 0: pid=294: Mon May 12 15:20:20 2025
  read: IOPS=5664, BW=22.1MiB/s (23.2MB/s)(2655MiB/120001msec)
    clat (usec): min=104, max=24847, avg=168.18, stdev=105.10
     lat (usec): min=105, max=24848, avg=169.08, stdev=105.18
    clat percentiles (usec):
     |  1.00th=[  115],  5.00th=[  118], 10.00th=[  121], 20.00th=[  125],
     | 30.00th=[  130], 40.00th=[  139], 50.00th=[  147], 60.00th=[  157],
     | 70.00th=[  167], 80.00th=[  184], 90.00th=[  237], 95.00th=[  326],
     | 99.00th=[  400], 99.50th=[  457], 99.90th=[  857], 99.95th=[ 1205],
     | 99.99th=[ 3949]
   bw (  KiB/s): min=19432, max=26108, per=100.00%, avg=22685.31, stdev=1077.42, samples=239
   iops        : min= 4858, max= 6527, avg=5671.19, stdev=269.37, samples=239
  lat (usec)   : 250=90.75%, 500=8.90%, 750=0.23%, 1000=0.06%
  lat (msec)   : 2=0.03%, 4=0.03%, 10=0.01%, 50=0.01%
  cpu          : usr=11.39%, sys=42.11%, ctx=694343, majf=0, minf=19
  IO depths    : 1=100.0%, 2=0.0%, 4=0.0%, 8=0.0%, 16=0.0%, 32=0.0%, >=64=0.0%
     submit    : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.0%, 64=0.0%, >=64=0.0%
     complete  : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.0%, 64=0.0%, >=64=0.0%
     issued rwts: total=679704,0,0,0 short=0,0,0,0 dropped=0,0,0,0
     latency   : target=0, window=0, percentile=100.00%, depth=1

random read:
read_test: (groupid=0, jobs=4): err= 0: pid=298: Mon May 12 15:22:21 2025
  read: IOPS=5641, BW=22.0MiB/s (23.1MB/s)(2644MiB/120001msec)
    clat (usec): min=113, max=20338, avg=679.38, stdev=213.08
     lat (usec): min=115, max=20340, avg=681.94, stdev=213.51
    clat percentiles (usec):
     |  1.00th=[  453],  5.00th=[  498], 10.00th=[  594], 20.00th=[  619],
     | 30.00th=[  627], 40.00th=[  627], 50.00th=[  635], 60.00th=[  644],
     | 70.00th=[  668], 80.00th=[  725], 90.00th=[  824], 95.00th=[  922],
     | 99.00th=[ 1221], 99.50th=[ 1385], 99.90th=[ 4178], 99.95th=[ 4490],
     | 99.99th=[ 5080]
   bw (  KiB/s): min=19472, max=23256, per=100.00%, avg=22590.87, stdev=137.35, samples=956
   iops        : min= 4866, max= 5814, avg=5647.05, stdev=34.34, samples=956
  lat (usec)   : 250=0.02%, 500=5.01%, 750=78.32%, 1000=13.25%
  lat (msec)   : 2=3.19%, 4=0.07%, 10=0.13%, 20=0.01%, 50=0.01%
  cpu          : usr=3.34%, sys=16.86%, ctx=711155, majf=0, minf=75
  IO depths    : 1=100.0%, 2=0.0%, 4=0.0%, 8=0.0%, 16=0.0%, 32=0.0%, >=64=0.0%
     submit    : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.0%, 64=0.0%, >=64=0.0%
     complete  : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.0%, 64=0.0%, >=64=0.0%
     issued rwts: total=676947,0,0,0 short=0,0,0,0 dropped=0,0,0,0
     latency   : target=0, window=0, percentile=100.00%, depth=1


```
