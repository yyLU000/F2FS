```bash
[+] Starting sequential write test...
seq_write: (g=0): rw=write, bs=(R) 128KiB-128KiB, (W) 128KiB-128KiB, (T) 128KiB-128KiB, ioengine=libaio, iodepth=1
fio-3.38
Starting 1 process
seq_write: Laying out IO file (1 file / 3072MiB)
[  118.017299] random: crng init done/s][w=1024 IOPS][eta 00m:12s]
Jobs: 1 (f=1): [W(1)][100.0%][w=123MiB/s][w=987 IOPS][eta 00m:00s]
seq_write: (groupid=0, jobs=1): err= 0: pid=163: Wed Apr 23 13:29:49 2025
  write: IOPS=981, BW=123MiB/s (129MB/s)(3072MiB/25040msec); 0 zone resets
    slat (usec): min=678, max=13319, avg=982.32, stdev=428.69
    clat (usec): min=20, max=1346, avg=25.50, stdev=18.13
     lat (usec): min=699, max=13407, avg=1007.82, stdev=431.19
    clat percentiles (usec):
     |  1.00th=[   22],  5.00th=[   23], 10.00th=[   23], 20.00th=[   23],
     | 30.00th=[   23], 40.00th=[   24], 50.00th=[   24], 60.00th=[   25],
     | 70.00th=[   25], 80.00th=[   26], 90.00th=[   28], 95.00th=[   30],
     | 99.00th=[   46], 99.50th=[  153], 99.90th=[  227], 99.95th=[  269],
     | 99.99th=[  482]
   bw (  KiB/s): min=88399, max=139008, per=100.00%, avg=126170.71, stdev=10378.18, samples=49
   iops        : min=  690, max= 1086, avg=985.39, stdev=81.06, samples=49
  lat (usec)   : 50=99.13%, 100=0.18%, 250=0.63%, 500=0.05%
  lat (msec)   : 2=0.01%
  cpu          : usr=3.74%, sys=57.50%, ctx=54072, majf=0, minf=19
  IO depths    : 1=100.0%, 2=0.0%, 4=0.0%, 8=0.0%, 16=0.0%, 32=0.0%, >=64=0.0%
     submit    : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.0%, 64=0.0%, >=64=0.0%
     complete  : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.0%, 64=0.0%, >=64=0.0%
     issued rwts: total=0,24576,0,0 short=0,0,0,0 dropped=0,0,0,0
     latency   : target=0, window=0, percentile=100.00%, depth=1

Run status group 0 (all jobs):
  WRITE: bw=123MiB/s (129MB/s), 123MiB/s-123MiB/s (129MB/s-129MB/s), io=3072MiB (3221MB), run=25040-25040msec

Disk stats (read/write):
  vdb: ios=0/24430, sectors=0/6254080, merge=0/0, ticks=0/8922, in_queue=8922, util=99.65%
[+] Starting random update test...
rand_update: (g=0): rw=randwrite, bs=(R) 128KiB-128KiB, (W) 128KiB-128KiB, (T) 128KiB-128KiB, ioengine=sync, iodepth=1
...


fio-3.38
Starting 4 processes
Jobs: 4 (f=4): [w(4)][100.0%][w=329MiB/s][w=2634 IOPS][eta 00m:00s]
rand_update: (groupid=0, jobs=4): err= 0: pid=168: Wed Apr 23 13:34:49 2025
  write: IOPS=1698, BW=212MiB/s (223MB/s)(62.2GiB/300048msec); 0 zone resets
    clat (usec): min=88, max=30819k, avg=1840.04, stdev=90746.07
     lat (usec): min=96, max=30819k, avg=1867.22, stdev=90749.36
    clat percentiles (usec):
     |  1.00th=[    92],  5.00th=[    94], 10.00th=[    95], 20.00th=[   100],
     | 30.00th=[   111], 40.00th=[   130], 50.00th=[   143], 60.00th=[   172],
     | 70.00th=[   210], 80.00th=[   247], 90.00th=[   742], 95.00th=[  8291],
     | 99.00th=[ 20317], 99.50th=[ 34866], 99.90th=[104334], 99.95th=[204473],
     | 99.99th=[413139]
   bw (  KiB/s): min= 1013, max=738521, per=100.00%, avg=321858.31, stdev=38321.60, samples=1622
   iops        : min=    4, max= 5768, avg=2512.82, stdev=299.49, samples=1622
  lat (usec)   : 100=19.94%, 250=60.73%, 500=8.85%, 750=0.48%, 1000=0.15%
  lat (msec)   : 2=0.25%, 4=0.72%, 10=5.28%, 20=2.55%, 50=0.71%
  lat (msec)   : 100=0.23%, 250=0.09%, 500=0.01%, 750=0.01%, 1000=0.01%
  lat (msec)   : 2000=0.01%, >=2000=0.01%
  cpu          : usr=1.45%, sys=10.53%, ctx=134810, majf=0, minf=83
  IO depths    : 1=100.0%, 2=0.0%, 4=0.0%, 8=0.0%, 16=0.0%, 32=0.0%, >=64=0.0%
     submit    : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.0%, 64=0.0%, >=64=0.0%
     complete  : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.0%, 64=0.0%, >=64=0.0%
     issued rwts: total=0,509633,0,0 short=0,0,0,0 dropped=0,0,0,0
     latency   : target=0, window=0, percentile=100.00%, depth=1

Run status group 0 (all jobs):
  WRITE: bw=212MiB/s (223MB/s), 212MiB/s-212MiB/s (223MB/s-223MB/s), io=62.2GiB (66.8GB), run=300048-300048msec

Disk stats (read/write):
  vdb: ios=2/28690, sectors=16/67805296, merge=0/0, ticks=1/415260, in_queue=453091, util=50.00%
[+] Done.
```

### status

```bash
=====[ partition info(vda). #0, RW, CP: Good]=====
[SB: 1] [CP: 2] [SIT: 2] [NAT: 2] [SSA: 1] [MAIN: 120(OverProv:18 Resv:13)]

Current Time Sec: 2033 / Mounted Time Sec: 0

Utilization: 41% (21797 valid blocks, 39632 discard blocks)
  - Node: 2149 (Inode: 2149, Other: 0)
  - Data: 19648
  - Inline_xattr Inode: 37
  - Inline_data Inode: 11
  - Inline_dentry Inode: 3
  - Compressed Inode: 0, Blocks: 0
  - Orphan/Append/Update Inode: 0, 0, 0

Main area: 120 segs, 120 secs 120 zones
    TYPE            segno    secno   zoneno  dirty_seg   full_seg  valid_blk
  - COLD   data:       49       49       49          0          0          0
  - WARM   data:       48       48       48          1         37      19155
  - HOT    data:        4        4        4          1          0        493
  - Dir   dnode:       29       29       29          1          1        877
  - File  dnode:        2        2        2          1          2       1271
  - Indir nodes:        1        1        1          0          0          0
  - Pinned file:       -1       -1       -1
  - ATGC   data:       -1       -1       -1

  - Valid: 46
  - Dirty: 0
  - Prefree: 0
  - Free: 74 (74)

CP calls: 7 (BG: 8)
  - cp blocks : 35
  - sit blocks : 0
  - nat blocks : 0
  - ssa blocks : 0
GC calls: 0 (BG: 6)
  - data segments : 0 (0)
  - node segments : 0 (0)
Try to move 0 blocks (BG: 0)
  - data blocks : 0 (0)
  - node blocks : 0 (0)
Skipped : atomic write 0 (0)
BG skip : IO: 2, Other: 0

Extent Cache:
  - Hit Count: L1-1:147 L1-2:0 L2:0
  - Hit Ratio: 98% (147 / 150)
  - Inner Struct Count: tree: 7(0), node: 3

Balancing F2FS Async:
  - DIO (R:    0, W:    0)
  - IO_R (Data:    0, Node:    0, Meta:    0
  - IO_W (CP:    0, Data:    0, Flush: (   0    0    1), Discard: (   0   10)) cmd:    0 undiscard:   0
  - inmem:    0, atomic IO:    0 (Max.    0), volatile IO:    0 (Max.    0)
  - nodes:    3 in   37
  - dents:    1 in dirs:   1 (   1)
  - datas:    1 in files:   0
  - quota datas:    0 in quota files:   0
  - meta:    0 in   12
  - imeta:    1
  - NATs:         1/       37
  - SITs:         0/      120
  - free_nids:      3308/   230808
  - alloc_nids:         0

Distribution of User Blocks: [ valid | invalid | free ]
  [--------------------|---|---------------------------]

IPU: 9 blocks
SSR: 4 blocks in 0 segments
LFS: 8 blocks in 0 segments

BDF: 97, avg. vblocks: 0

Memory: 387 KB
  - static: 103 KB
  - cached: 87 KB
  - paged : 196 KB

=====[ partition info(vdb). #1, RW, CP: Good]=====
[SB: 1] [CP: 2] [SIT: 2] [NAT: 12] [SSA: 16] [MAIN: 2512(OverProv:304 Resv:272)]

Current Time Sec: 2033 / Mounted Time Sec: 105

Utilization: 69% (787208 valid blocks, 231 discard blocks)
  - Node: 775 (Inode: 2, Other: 773)
  - Data: 786433
  - Inline_xattr Inode: 1
  - Inline_data Inode: 0
  - Inline_dentry Inode: 0
  - Compressed Inode: 0, Blocks: 0
  - Orphan/Append/Update Inode: 0, 0, 0

Main area: 2512 segs, 157 secs 157 zones
    TYPE            segno    secno   zoneno  dirty_seg   full_seg  valid_blk
  - COLD   data:      683       42       42          1         75      38869
  - WARM   data:      462       28       28         67       1404     747563
  - HOT    data:       48        3        3          1          0          1
  - Dir   dnode:        0        0        0          1          0          1
  - File  dnode:      533       33       33          3          0        773
  - Indir nodes:       32        2        2          1          0          1
  - Pinned file:       -1       -1       -1
  - ATGC   data:       -1       -1       -1
  - Valid: 1485
  - Dirty: 68
  - Prefree: 0
  - Free: 959 (56)

CP calls: 70 (BG: 29)
  - cp blocks : 223
  - sit blocks : 1110
  - nat blocks : 62
  - ssa blocks : 18510
GC calls: 26 (BG: 26)
  - data segments : 179 (179)
  - node segments : 2 (2)
Try to move 39384 blocks (BG: 39384)
  - data blocks : 38869 (38869)
  - node blocks : 515 (515)
Skipped : atomic write 0 (0)
BG skip : IO: 4, Other: 1

Extent Cache:
  - Hit Count: L1-1:0 L1-2:50341 L2:721
  - Hit Ratio: 49% (51062 / 102314)
  - Inner Struct Count: tree: 1(0), node: 1909

Balancing F2FS Async:
  - DIO (R:    0, W:    0)
  - IO_R (Data:    0, Node:    0, Meta:    0
  - IO_W (CP:    0, Data:    0, Flush: (   0    0    1), Discard: (   0 2034)) cmd:    0 undiscard:   0
  - inmem:    0, atomic IO:    0 (Max.    0), volatile IO:    0 (Max.    0)
  - nodes:    0 in  775
  - dents:    0 in dirs:   0 (   0)
  - datas:    0 in files:   0
  - quota datas:    0 in quota files:   0
  - meta:    0 in 2550
  - imeta:    0
  - NATs:         0/      775
  - SITs:         0/     2512
  - free_nids:      3317/  1396982
  - alloc_nids:         0

Distribution of User Blocks: [ valid | invalid | free ]
  [----------------------------------|--|--------------]

IPU: 0 blocks
SSR: 0 blocks in 0 segments
LFS: 9478525 blocks in 18510 segments

BDF: 97, avg. vblocks: 4538

Memory: 14348 KB
  - static: 819 KB
  - cached: 229 KB
  - paged : 13300 KB


