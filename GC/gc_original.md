## Original

### sequential write
```bash
IOPS=1096, BW=137MiB/s (144MB/s)(3584MiB/26141msec);
clat (usec): min=666, max=100814, avg=890.33, stdev=617.48
clat percentiles (usec):
90.00th=[ 1029], 95.00th=[ 1123], 99.00th=[ 1631], 99.50th=[ 2376], 99.90th=[ 4948], 99.95th=[ 5145], 99.99th=[ 8029]
cpu          : usr=3.79%, sys=65.50%, ctx=71992, majf=0, minf=20
bw (  KiB/s): min=97280, max=148480, per=100.00%, avg=140371.46, stdev=7927.91, samples=59
iops        : min=  760, max= 1160, avg=1096.32, stdev=61.88, samples=59

```

### random update
```bash
IOPS=164, BW=658KiB/s (674kB/s)(194MiB/301852msec);
clat (usec): min=169, max=2007.2k, avg=24222.42, stdev=185020.88
clat percentiles (usec):
90.00th=[  15926], 95.00th=[  16319], 99.00th=[ 977273], 99.50th=[1837106], 99.90th=[1887437], 99.95th=[1920992], 99.99th=[1954546]
cpu          : usr=0.14%, sys=23.54%, ctx=195322, majf=0, minf=75
bw (  KiB/s): min=   28, max= 4801, per=100.00%, avg=1506.87, stdev=381.44, samples=1052
iops        : min=    4, max= 1199, avg=375.96, stdev=95.25, samples=1052
```

### GC
```bash
Utilization: 92% (1049611 valid blocks, 7764 discard blocks)
  - Node: 1034 (Inode: 2, Other: 1032)
  - Data: 1048577
  - Inline_xattr Inode: 1
  - Inline_data Inode: 0
  - Inline_dentry Inode: 0
  - Compressed Inode: 0, Blocks: 0
  - Orphan/Append/Update Inode: 0, 0, 0


Main area: 2512 segs, 157 secs 157 zones
    TYPE            segno    secno   zoneno  dirty_seg   full_seg  valid_blk
  - COLD   data:      623       38       38          1         15       7764
  - WARM   data:      944       59       59       2123          6    1040812
  - HOT    data:       48        3        3          1          0          1
  - Dir   dnode:        0        0        0          1          0          1
  - File  dnode:      962       60       60          3          0       1031
  - Indir nodes:       32        2        2          1          0          2
  - Pinned file:       -1       -1       -1
  - ATGC   data:       -1       -1       -1

  - Valid: 27
  - Dirty: 2124
  - Prefree: 16
  - Free: 345 (17)

GC calls: 8757 (BG: 1)
  - data segments : 140096 (0)
  - node segments : 11 (0)
  - Reclaimed segs : Normal (140107), Idle CB (0), Idle Greedy (0), Idle AT (0), Urgent High (0), Urgent Low (0)
Try to move 8795 blocks (BG: 0)
  - data blocks : 7764 (0)
  - node blocks : 1031 (0)
```

### sequential read
```bash
IOPS=183, BW=23.0MiB/s (24.1MB/s)(2760MiB/120002msec)
clat (usec): min=162, max=32017k, avg=5422.98, stdev=368937.59
90.00th=[  725], 95.00th=[  971], 99.00th=[ 1991], 99.50th=[ 4047], 99.90th=[ 6980], 99.95th=[ 8586], 99.99th=[17112761]
bw ( KiB/s): min=766, max=328303, avg=200846.78, stdev=106795.52, samples=27
iops       : min=  5, max=  2564, avg=  1568.78, stdev=834.43
```

### random read
```bash
IOPS=5600, BW=21.9MiB/s (22.9MB/s)(2625MiB/120002msec)
clat (usec): min=95, max=457521, avg=669.40, stdev=1131.07
90.00th=[  857], 95.00th=[  971], 99.00th=[ 1319], 99.50th=[ 1663], 99.90th=[ 4686], 99.95th=[ 5604], 99.99th=[ 17957]
bw ( KiB/s): min=11891, max=24104, avg=22456.69, stdev=296.43, samples=956
iops       : min= 2971, max= 6026, avg=5613.38, stdev=74.12
```

