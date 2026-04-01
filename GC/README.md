# GC Directory Structure

This directory contains the core garbage collection implementation for F2FS, an experimental defragmentation-aware GC path, several workload scripts for triggering and evaluating GC behavior, and one baseline result note. The current folder includes `f2fs.h`, `gc.c`, `gc.h`, `gc_defrag.c`, `gc_defrag.h`, `gc_multi_read.sh`, `gc_multi_write.sh`, `gc_original.md`, `gc_rand_write.sh`, `gc_read.sh`, `gc_write.sh`, and `README.md`.

## 1. Core source files

### `gc.c`
This is the **main/original F2FS garbage collection implementation**. It contains the background GC thread (`gc_thread_func`), GC thread start/stop logic (`f2fs_start_gc_thread`, `f2fs_stop_gc_thread`), GC policy selection, and victim-selection-related logic. In short, this file is the primary execution path for the standard F2FS GC mechanism. 

### `gc.h`
This header provides **GC-related definitions and shared declarations** used by the GC implementation. It acts as the interface and supporting definition file for the logic in `gc.c`. 

### `gc_defrag.c`
This file is the **modified/experimental GC implementation**. Compared with `gc.c`, it is intended to add a defragmentation-aware optimization path on top of normal GC. It should be read as the customized version of the original GC logic.

### `gc_defrag.h`
This header is the **interface file for the modified GC path** in `gc_defrag.c`. It is used to expose the declarations needed by the defrag-aware GC implementation. 

### `f2fs.h`
This is a **shared F2FS header file** copied into this folder to support the GC-related source files. It provides common F2FS-level structures, macros, and definitions required by the implementation.

## 2. Workload / evaluation scripts

### `gc_write.sh`
This script is used to **generate write-heavy workloads** that can trigger or stress garbage collection. It is part of the experimental setup for observing GC behavior under write pressure.

### `gc_read.sh`
This script is used to **run read-side workloads** after or alongside GC experiments, mainly for evaluating read performance under the tested filesystem state. 

### `gc_rand_write.sh`
This script is used to **generate random-write workloads**, which are useful for creating fragmentation and stressing the filesystem in a way that is different from purely sequential writes.

### `gc_multi_write.sh`
This script is a **multi-file write workload driver**. It is intended to create more complex write patterns across multiple files so that GC behavior can be tested under a heavier or more fragmented workload scenario.

### `gc_multi_read.sh`
This script is the **multi-file read-side companion** to `gc_multi_write.sh`. It is used to observe the read performance of the filesystem after multi-file write/fragmentation experiments.

## 3. Notes / result file

### `gc_original.md`
This file stores the **baseline notes or recorded results for the original GC implementation**. It is useful as a reference point when comparing the original design against the modified defrag-aware version.

### `structure.md`
This file documents the purpose of each file in the `GC/` directory so that readers can quickly understand how the folder is organized and where to start reading. 

## 4. Suggested reading order

For someone reading this directory for the first time, a reasonable order is:

1. `gc.h` — understand the shared GC definitions first.  
2. `gc.c` — read the original F2FS GC implementation.  
3. `gc_defrag.h` and `gc_defrag.c` — compare the modified design with the original one.  
4. `gc_write.sh`, `gc_rand_write.sh`, and `gc_multi_write.sh` — see how workloads are generated.  
5. `gc_read.sh` and `gc_multi_read.sh` — see how performance is evaluated after the workloads.  
6. `gc_original.md` — check the baseline notes/results for comparison.

## 5. High-level relationship

At a high level, the directory can be viewed as four parts:

- **Original GC implementation:** `gc.c` + `gc.h`
- **Modified/optimized GC implementation:** `gc_defrag.c` + `gc_defrag.h`
- **Experiment scripts:** `gc_write.sh`, `gc_rand_write.sh`, `gc_multi_write.sh`, `gc_read.sh`, `gc_multi_read.sh`
- **Reference notes/results:** `gc_original.md`

This structure makes it easier to separate the implementation itself from the workload generation scripts and the experimental comparison materials.

```bash

Cleaning is done by SECTIONS.

section  (32M): 16 segments
segments (2M) : 512 blocks
```

**Defragmentation Idea:**
```cpp
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
```

