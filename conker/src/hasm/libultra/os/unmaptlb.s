.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

/*
 * osUnmapTLB — clear one TLB entry by index.
 *
 * Leaving this in assembly is the most likely match of the original codebase:
 * the CP0 TLB registers and tlbwi have no C spelling, so SGI/libreultra also
 * ship unmaptlb as a .s leaf.
 *
 * EntryHi is parked at K0BASE (unmapped) so the dead entry can never match a
 * translation, and both EntryLo halves are zeroed to clear the valid bits. The
 * caller's EntryHi is saved and restored around the write, and the run of nops
 * covers the CP0 hazards before and after tlbwi.
 *
 * Tracked under src/hasm/ (gitignored asm/ is splat output only).
 *
 * a0 = TLB index to unmap
 */
nonmatching osUnmapTLB, 0x3C

glabel osUnmapTLB
    mfc0        $t0, $10                    /* save C0_ENTRYHI */
    mtc0        $a0, $0                     /* C0_INX = index */
    lui         $t1, (K0BASE >> 16)
    mtc0        $t1, $10                    /* C0_ENTRYHI */
    mtc0        $zero, $2                   /* C0_ENTRYLO0 */
    mtc0        $zero, $3                   /* C0_ENTRYLO1 */
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
