.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

/*
 * osGetCount — read the free-running CP0 Count register (CP0 $9).
 *
 * Leaving this in assembly is the most likely match of the original codebase:
 * mfc0 has no C spelling, so SGI/libreultra also ship getcount as a tiny .s
 * leaf.
 *
 * Tracked under src/hasm/ (gitignored asm/ is splat output only).
 *
 * returns the current Count value in v0
 */
nonmatching osGetCount, 0xC

glabel osGetCount
    mfc0        $v0, $9                     /* C0_COUNT */
    jr          $ra
     nop
    nop
