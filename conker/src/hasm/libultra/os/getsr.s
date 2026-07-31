.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

/*
 * __osGetSR — read the CPU Status register (CP0 $12).
 *
 * Leaving this in assembly is the most likely match of the original codebase:
 * mfc0 has no C spelling, so SGI/libreultra also ship getsr as a tiny .s leaf.
 *
 * Tracked under src/hasm/ (gitignored asm/ is splat output only).
 *
 * returns the current Status value in v0
 */
nonmatching __osGetSR, 0xC

glabel __osGetSR
    mfc0        $v0, $12                    /* C0_SR */
    jr          $ra
     nop
    nop
