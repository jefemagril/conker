.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

/*
 * osMapTLBRdb — install the fixed TLB entry for the rdb (host debug) port.
 *
 * Leaving this in assembly is the most likely match of the original codebase:
 * the CP0 TLB registers and tlbwi have no C spelling, so SGI/libreultra also
 * ship maptlbrdb as a .s leaf.
 *
 * Maps a single uncached, dirty, valid, global 4K page. This build differs from
 * the libreultra sibling in the entry it burns and the window it maps: index 1
 * with EntryHi 0xB8000000, rather than index 31 at K2BASE. The odd half is left
 * global-only (not valid), and the caller's EntryHi is saved and restored.
 *
 * Tracked under src/hasm/ (gitignored asm/ is splat output only).
 *
 * no arguments
 */
nonmatching osMapTLBRdb, 0x58

glabel osMapTLBRdb
    mfc0        $t0, $10                    /* save C0_ENTRYHI */
    addiu       $t1, $zero, 0x1
    mtc0        $t1, $0                     /* C0_INX = 1 */
    mtc0        $zero, $5                   /* C0_PAGEMASK = 4K */
    addiu       $t2, $zero, 0x17            /* TLBLO_UNCACHED | TLBLO_D | TLBLO_V | TLBLO_G */
    lui         $t1, (0xB8000000 >> 16)
    mtc0        $t1, $10                    /* C0_ENTRYHI = rdb window */
    lui         $t1, (K0BASE >> 16)
    srl         $t3, $t1, 6                 /* TLBLO_PFNSHIFT */
    or          $t3, $t3, $t2
    mtc0        $t3, $2                     /* C0_ENTRYLO0 */
    addiu       $t1, $zero, 0x1             /* TLBLO_G */
    mtc0        $t1, $3                     /* C0_ENTRYLO1 */
    nop
    tlbwi
    nop
    nop
    nop
    nop
    mtc0        $t0, $10                    /* restore C0_ENTRYHI */
    jr          $ra
     nop
    nop
    nop
