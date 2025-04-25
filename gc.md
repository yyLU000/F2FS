# sync
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
```

# libaio

```bash
[+] Unmounting previous mount (if any)...
[+] Formatting /dev/vdb as F2FS (5GB)...

    F2FS-tools: mkfs.f2fs Ver: 1.16.0 (2023-04-11)

Info: Disable heap-based policy
Info: Debug level = 0
Info: Trim is enabled
Info: Segments per section = 16
Info: Sections per zone = 1
Info: sector size = 512
Info: total sectors = 10485760 (5120 MB)
Info: zone aligned segment0 blkaddr: 8192
Info: format version with
  "Linux version 5.10.99 (root@lawu) (aarch64-buildroot-linux-uclibc-gcc.br_real (Buildroot 2025.05-git) 13.3.0, GNU ld (GNU Binutils) 2.43.1) #2 SMP PREEMPT Fri Apr 18 17:09:20 +08 2025"
Info: [/dev/vdb] Discarding device
Info: This device doesn't support BLKSECDISCARD
Info: Discarded 5120 MB
Info: Overprovision ratio = 10.000%
Info: Overprovision segments = 304 (GC reserved = 272)
Info: format successful
[+] Creating mount point and mounting...
[ 4143.864594] F2FS-fs (vdb): Found nat_bits in checkpoint
[ 4143.909150] F2FS-fs (vdb): Mounted with checkpoint version = ed14b11
Filesystem                Size      Used Available Use% Mounted on
/dev/vdb                  5.0G    672.0M      4.3G  13% /mnt/f2fs-test
[+] Syncing and dropping caches...
[ 4143.948238] gc.sh (212): drop_caches: 3
[+] Starting sequential write test...
seq_write: (g=0): rw=write, bs=(R) 128KiB-128KiB, (W) 128KiB-128KiB, (T) 128KiB-128KiB, ioengine=libaio, iodepth=1
fio-3.38
Starting 1 process
seq_write: Laying out IO file (1 file / 3072MiB)
Jobs: 1 (f=1): [W(1)][100.0%][w=93.6MiB/s][w=748 IOPS][eta 00m:00s]
seq_write: (groupid=0, jobs=1): err= 0: pid=226: Wed Apr 23 14:37:12 2025
  write: IOPS=825, BW=103MiB/s (108MB/s)(3072MiB/29764msec); 0 zone resets
    slat (usec): min=694, max=76000, avg=1170.53, stdev=875.30
    clat (usec): min=20, max=3801, avg=28.26, stdev=44.56
     lat (usec): min=716, max=76060, avg=1198.80, stdev=879.30
    clat percentiles (usec):
     |  1.00th=[   23],  5.00th=[   23], 10.00th=[   24], 20.00th=[   24],
     | 30.00th=[   25], 40.00th=[   25], 50.00th=[   26], 60.00th=[   26],
     | 70.00th=[   27], 80.00th=[   27], 90.00th=[   29], 95.00th=[   34],
     | 99.00th=[   82], 99.50th=[  165], 99.90th=[  318], 99.95th=[  570],
     | 99.99th=[ 2671]
   bw (  KiB/s): min=61317, max=140032, per=100.00%, avg=106230.31, stdev=18573.68, samples=59
   iops        : min=  479, max= 1094, avg=829.63, stdev=145.18, samples=59
  lat (usec)   : 50=98.02%, 100=1.05%, 250=0.74%, 500=0.13%, 750=0.02%
  lat (usec)   : 1000=0.02%
  lat (msec)   : 2=0.01%, 4=0.01%
  cpu          : usr=3.65%, sys=51.47%, ctx=54350, majf=0, minf=20
  IO depths    : 1=100.0%, 2=0.0%, 4=0.0%, 8=0.0%, 16=0.0%, 32=0.0%, >=64=0.0%
     submit    : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.0%, 64=0.0%, >=64=0.0%
     complete  : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.0%, 64=0.0%, >=64=0.0%
     issued rwts: total=0,24576,0,0 short=0,0,0,0 dropped=0,0,0,0
     latency   : target=0, window=0, percentile=100.00%, depth=1

Run status group 0 (all jobs):
  WRITE: bw=103MiB/s (108MB/s), 103MiB/s-103MiB/s (108MB/s-108MB/s), io=3072MiB (3221MB), run=29764-29764msec

Disk stats (read/write):
  vdb: ios=0/24526, sectors=0/6278656, merge=0/0, ticks=0/12411, in_queue=12411, util=99.56%
[+] Starting random update test...
rand_update: (g=0): rw=randwrite, bs=(R) 128KiB-128KiB, (W) 128KiB-128KiB, (T) 128KiB-128KiB, ioengine=libaio, iodepth=1
...
fio-3.38
Starting 4 processes
Jobs: 4 (f=4): [w(4)][100.0%][w=303MiB/s][w=2421 IOPS][eta 00m:00s]
rand_update: (groupid=0, jobs=4): err= 0: pid=231: Wed Apr 23 14:42:13 2025
  write: IOPS=1874, BW=234MiB/s (246MB/s)(68.6GiB/300013msec); 0 zone resets
    slat (usec): min=102, max=26936k, avg=1504.07, stdev=65818.61
    clat (usec): min=17, max=94098, avg=67.82, stdev=711.24
     lat (usec): min=121, max=26936k, avg=1571.89, stdev=65821.84
    clat percentiles (usec):
     |  1.00th=[   19],  5.00th=[   19], 10.00th=[   19], 20.00th=[   20],
     | 30.00th=[   20], 40.00th=[   20], 50.00th=[   21], 60.00th=[   21],
     | 70.00th=[   22], 80.00th=[   23], 90.00th=[   27], 95.00th=[   32],
     | 99.00th=[   71], 99.50th=[ 4113], 99.90th=[12125], 99.95th=[15008],
     | 99.99th=[20579]
   bw (  KiB/s): min= 2287, max=1201307, per=100.00%, avg=351713.52, stdev=33765.74, samples=1633
   iops        : min=   14, max= 9383, avg=2746.16, stdev=263.83, samples=1633
  lat (usec)   : 20=49.88%, 50=48.40%, 100=0.92%, 250=0.15%, 500=0.03%
  lat (usec)   : 750=0.01%, 1000=0.01%
  lat (msec)   : 2=0.03%, 4=0.06%, 10=0.38%, 20=0.12%, 50=0.02%
  lat (msec)   : 100=0.01%
  cpu          : usr=1.71%, sys=13.26%, ctx=160525, majf=0, minf=75
  IO depths    : 1=100.0%, 2=0.0%, 4=0.0%, 8=0.0%, 16=0.0%, 32=0.0%, >=64=0.0%
     submit    : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.0%, 64=0.0%, >=64=0.0%
     complete  : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.0%, 64=0.0%, >=64=0.0%
     issued rwts: total=0,562380,0,0 short=0,0,0,0 dropped=0,0,0,0
     latency   : target=0, window=0, percentile=100.00%, depth=1

Run status group 0 (all jobs):
  WRITE: bw=234MiB/s (246MB/s), 234MiB/s-234MiB/s (246MB/s-246MB/s), io=68.6GiB (73.7GB), run=300013-300013msec

Disk stats (read/write):
  vdb: ios=2/30772, sectors=16/72103504, merge=0/0, ticks=3/467128, in_queue=536330, util=44.44%
[+] Done.

```
## status 
```bash
=====[ partition info(vdb). #1, RW, CP: Good]=====
[SB: 1] [CP: 2] [SIT: 2] [NAT: 12] [SSA: 16] [MAIN: 2512(OverProv:304 Resv:272)]

Current Time Sec: 4620 / Mounted Time Sec: 4143

Utilization: 69% (787208 valid blocks, 15637 discard blocks)
  - Node: 775 (Inode: 2, Other: 773)
  - Data: 786433
  - Inline_xattr Inode: 1
  - Inline_data Inode: 0
  - Inline_dentry Inode: 0
  - Compressed Inode: 0, Blocks: 0
  - Orphan/Append/Update Inode: 0, 0, 0

Main area: 2512 segs, 157 secs 157 zones
    TYPE            segno    secno   zoneno  dirty_seg   full_seg  valid_blk
  - COLD   data:      608       38       38          0          0          0
  - WARM   data:      852       53       53        340       1378     786432
  - HOT    data:       48        3        3          1          0          1
  - Dir   dnode:        0        0        0          1          0          1
  - File  dnode:      502       31       31          2          0        773
  - Indir nodes:       32        2        2          1          0          1
  - Pinned file:       -1       -1       -1
  - ATGC   data:       -1       -1       -1

  - Valid: 1384
  - Dirty: 340
  - Prefree: 0
  - Free: 788 (42)

CP calls: 68 (BG: 3)
  - cp blocks : 204
  - sit blocks : 1198
  - nat blocks : 70
  - ssa blocks : 19450
GC calls: 1 (BG: 1)
  - data segments : 0 (0)
  - node segments : 2 (2)
Try to move 515 blocks (BG: 515)
  - data blocks : 0 (0)
  - node blocks : 515 (515)
Skipped : atomic write 0 (0)
BG skip : IO: 7, Other: 0

Extent Cache:
  - Hit Count: L1-1:0 L1-2:0 L2:0
  - Hit Ratio: 0% (0 / 24576)
  - Inner Struct Count: tree: 1(0), node: 1512

Balancing F2FS Async:
  - DIO (R:    0, W:    0)
  - IO_R (Data:    0, Node:    0, Meta:    0
  - IO_W (CP:    0, Data:    0, Flush: (   0    0    1), Discard: (   1 1525)) cmd:    1 undiscard:8192
  - inmem:    0, atomic IO:    0 (Max.    0), volatile IO:    0 (Max.    0)
  - nodes:    0 in  775
  - dents:    0 in dirs:   0 (   0)
  - datas:    0 in files:   0
  - quota datas:    0 in quota files:   0
  - meta:    0 in 2532
  - imeta:    0
  - NATs:         0/      775
  - SITs:         0/     2512
  - free_nids:      3317/  1396982
  - alloc_nids:         0

```


## libaio + fsync=1

```bash
Starting 4 processes
Jobs: 4 (f=4): [w(4)][100.0%][w=81.6MiB/s][w=653 IOPS][eta 00m:00s]
rand_update: (groupid=0, jobs=4): err= 0: pid=311: Wed Apr 23 16:42:53 2025
  write: IOPS=728, BW=91.0MiB/s (95.5MB/s)(26.7GiB/300014msec); 0 zone resets
    slat (usec): min=112, max=191230, avg=604.19, stdev=1891.45
    clat (usec): min=19, max=16365, avg=42.03, stdev=203.71
     lat (usec): min=132, max=191261, avg=646.22, stdev=1907.89
    clat percentiles (usec):
     |  1.00th=[   22],  5.00th=[   23], 10.00th=[   23], 20.00th=[   24],
     | 30.00th=[   25], 40.00th=[   25], 50.00th=[   26], 60.00th=[   26],
     | 70.00th=[   26], 80.00th=[   27], 90.00th=[   29], 95.00th=[   35],
     | 99.00th=[  174], 99.50th=[ 1319], 99.90th=[ 3097], 99.95th=[ 3621],
     | 99.99th=[ 6194]
   bw (  KiB/s): min=  992, max=124927, per=100.00%, avg=94030.74, stdev=4377.34, samples=2379
   iops        : min=    5, max=  975, avg=733.47, stdev=34.24, samples=2379
  lat (usec)   : 20=0.02%, 50=97.30%, 100=1.15%, 250=0.67%, 500=0.12%
  lat (usec)   : 750=0.07%, 1000=0.04%
  lat (msec)   : 2=0.30%, 4=0.30%, 10=0.03%, 20=0.01%
  fsync/fdatasync/sync_file_range:
    sync (usec): min=401, max=568367, avg=4798.99, stdev=4878.62
    sync percentiles (usec):
     |  1.00th=[  1450],  5.00th=[  2278], 10.00th=[  2737], 20.00th=[  3294],
     | 30.00th=[  3687], 40.00th=[  3982], 50.00th=[  4228], 60.00th=[  4555],
     | 70.00th=[  4883], 80.00th=[  5473], 90.00th=[  6718], 95.00th=[  8586],
     | 99.00th=[ 15795], 99.50th=[ 22676], 99.90th=[ 47449], 99.95th=[ 63701],
     | 99.99th=[168821]
  cpu          : usr=1.52%, sys=15.92%, ctx=982128, majf=0, minf=87
  IO depths    : 1=200.0%, 2=0.0%, 4=0.0%, 8=0.0%, 16=0.0%, 32=0.0%, >=64=0.0%
     submit    : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.0%, 64=0.0%, >=64=0.0%
     complete  : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.0%, 64=0.0%, >=64=0.0%
     issued rwts: total=0,218489,0,218488 short=0,0,0,0 dropped=0,0,0,0
     latency   : target=0, window=0, percentile=100.00%, depth=1

Run status group 0 (all jobs):
  WRITE: bw=91.0MiB/s (95.5MB/s), 91.0MiB/s-91.0MiB/s (95.5MB/s-95.5MB/s), io=26.7GiB (28.6GB), run=300014-300014msec

Disk stats (read/write):
  vdb: ios=3378/444460, sectors=417248/59262712, merge=48778/3, ticks=2741/185416, in_queue=246058, util=99.09%
```


```bash
=====[ partition info(vdb). #1, RW, CP: Good]=====
[SB: 1] [CP: 2] [SIT: 2] [NAT: 12] [SSA: 16] [MAIN: 2512(OverProv:304 Resv:272)]

Current Time Sec: 11839 / Mounted Time Sec: 11384

Utilization: 69% (787208 valid blocks, 6078 discard blocks)
  - Node: 775 (Inode: 2, Other: 773)
  - Data: 786433
  - Inline_xattr Inode: 1
  - Inline_data Inode: 0
  - Inline_dentry Inode: 0
  - Compressed Inode: 0, Blocks: 0
  - Orphan/Append/Update Inode: 0, 0, 0

Main area: 2512 segs, 157 secs 157 zones
    TYPE            segno    secno   zoneno  dirty_seg   full_seg  valid_blk
  - COLD   data:      350       21       21          6          5       3948
  - WARM   data:      463       28       28       1058       1024     782427
  - HOT    data:       52        3        3          2          0         58
  - Dir   dnode:        0        0        0          1          0          1
  - File  dnode:     1519       94       94          7          0        773
  - Indir nodes:       32        2        2          1          0          1
  - Pinned file:       -1       -1       -1
  - ATGC   data:       -1       -1       -1

  - Valid: 1035
  - Dirty: 1069
  - Prefree: 16
  - Free: 392 (20)

CP calls: 559 (BG: 1)
  - cp blocks : 2025
  - sit blocks : 3621
  - nat blocks : 491
  - ssa blocks : 15964
GC calls: 375 (BG: 2)
  - data segments : 1238 (17)
  - node segments : 140 (0)
Try to move 146567 blocks (BG: 2171)
  - data blocks : 130178 (2171)
  - node blocks : 16389 (0)
Skipped : atomic write 0 (0)
BG skip : IO: 5, Other: 0

Extent Cache:
  - Hit Count: L1-1:64 L1-2:98813 L2:7156
  - Hit Ratio: 37% (106033 / 284932)
  - Inner Struct Count: tree: 1(0), node: 8549

Balancing F2FS Async:
  - DIO (R:    0, W:    0)
  - IO_R (Data:    0, Node:    0, Meta:    0
  - IO_W (CP:    0, Data:    0, Flush: (   0 173387    1), Discard: (   0 5565)) cmd:    0 undiscard:   0
  - inmem:    0, atomic IO:    0 (Max.    0), volatile IO:    0 (Max.    0)
  - nodes:   47 in  775
  - dents:    0 in dirs:   0 (   0)
  - datas:    0 in files:   0
  - quota datas:    0 in quota files:   0
  - meta:    4 in 2556
  - imeta:    0
  - NATs:         0/      775
  - SITs:        21/     2512
  - free_nids:      3317/  1396982
  - alloc_nids:         0
```

