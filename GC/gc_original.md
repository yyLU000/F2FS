## Original

### sequential write
```bash
(ep1):
IOPS=1078, BW=135MiB/s (141MB/s)(3072MiB/22786msec);
    clat (usec): min=658, max=198935, avg=895.59, stdev=1285.26
90.00th=[ 1029], 95.00th=[ 1123],
     | 99.00th=[ 1680], 99.50th=[ 2311], 99.90th=[ 4113], 99.95th=[ 4490],
     | 99.99th=[ 5342]
bw (  KiB/s): min=74496, max=154880, per=100.00%, avg=138161.47, stdev=11758.72, samples=45
iops        : min=  582, max= 1210, avg=1078.98, stdev=91.87, samples=45
cpu          : usr=4.09%, sys=64.72%, ctx=52959, majf=0, minf=21

(ep2):


```

### random update
```bash
(ep1):
IOPS=1924, BW=15.0MiB/s (15.8MB/s)(2048MiB/136212msec);
clat (usec): min=130, max=29018k, avg=478.70, stdev=78069.36
90.00th=[  289], 95.00th=[  343], 99.00th=[  482], 99.50th=[  578], 99.90th=[ 2147], 99.95th=[ 3097], 99.99th=[ 4817]
bw (  KiB/s): min= 3203, max=30544, per=100.00%, avg=26806.67, stdev=3482.52, samples=156
iops        : min=  400, max= 3818, avg=3350.76, stdev=435.36, samples=156
cpu          : usr=8.55%, sys=24.15%, ctx=273471, majf=0, minf=23

(ep2):

```

### GC
```bash
Utilization: 61% (787208 valid blocks, 156 discard blocks)
  - Node: 775 (Inode: 2, Other: 773)
  - Data: 786433
  - Inline_xattr Inode: 1
  - Inline_data Inode: 0
  - Inline_dentry Inode: 0
  - Compressed Inode: 0, Blocks: 0
  - Orphan/Append/Update Inode: 0, 0, 0


Main area: 2538 segs, 2538 secs 2538 zones
    TYPE            segno    secno   zoneno  dirty_seg   full_seg  valid_blk
  - COLD   data:      159      159      159          0          0          0
  - WARM   data:       21       21       21          0       1536     786432
  - HOT    data:        3        3        3          1          0          1
  - Dir   dnode:        0        0        0          1          0          1
  - File  dnode:       22       22       22          2          0        773
  - Indir nodes:        2        2        2          1          0          1
  - Pinned file:       -1       -1       -1
  - ATGC   data:       -1       -1       -1

  - Valid: 1542
  - Dirty: 1
  - Prefree: 0
  - Free: 995 (995)

GC calls: 252 (BG: 253)
  - data segments : 248 (248)
  - node segments : 4 (4)
  - Reclaimed segs : Normal (252), Idle CB (0), Idle Greedy (0), Idle AT (0), Urgent High (0), Urgent Low (0)
Try to move 1585 blocks (BG: 1585)
  - data blocks : 910 (910)
  - node blocks : 675 (675)


```

### sequential read
```bash
IOPS=196, BW=24.6MiB/s (25.8MB/s)(3312MiB/134533msec)
clat (usec): min=127, max=31823k, avg=5066.25, stdev=376354.71
90.00th=[     635], 95.00th=[     799], 99.00th=[    1172], 99.50th=[    1418], 99.90th=[    3359], 99.95th=[    4359], 99.99th=[17112761]
bw (  KiB/s): min=51353, max=419257, per=100.00%, avg=260514.46, stdev=97607.60, samples=26
iops        : min=  401, max= 3275, avg=2034.88, stdev=762.57, samples=26
cpu          : usr=0.51%, sys=3.62%, ctx=28047, majf=0, minf=53
```

### random read
```bash
IOPS=5434, BW=21.2MiB/s (22.3MB/s)(2048MiB/96473msec)
clat (usec): min=92, max=43205, avg=331.15, stdev=149.76
90.00th=[  424], 95.00th=[  498], 99.00th=[  668], 99.50th=[  766], 99.90th=[ 1614], 99.95th=[ 3032], 99.99th=[ 3687]
bw (  KiB/s): min=18181, max=26192, per=100.00%, avg=21770.51, stdev=425.29, samples=384
iops        : min= 4545, max= 6548, avg=5442.52, stdev=106.36, samples=384
cpu          : usr=8.99%, sys=30.46%, ctx=547085, majf=0, minf=41
```





