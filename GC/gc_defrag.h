/* SPDX-License-Identifier: GPL-2.0 */
/*
 * f2fs_gc_defrag.h
 */
#ifndef _F2FS_GC_DEFRAG_H
#define _F2FS_GC_DEFRAG_H

int f2fs_gc_with_defrag(struct f2fs_sb_info *sbi, bool sync,
                       bool background, bool force, unsigned int segno);
void trigger_defrag_gc(struct f2fs_sb_info *sbi);

#endif /* _F2FS_GC_DEFRAG_H */
