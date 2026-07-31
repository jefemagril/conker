.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

/*
 * __osSetSR — write the CPU Status register (CP0 $12).
 *
 * Leaving this in assembly is the most likely match of the original codebase:
 * mtc0 has no C spelling, so SGI/libreultra also ship setsr as a tiny .s leaf.
 * The nop after the mtc0 is the hazard delay before the new SR takes effect.
 *
 * Tracked under src/hasm/ (gitignored asm/ is splat output only).
 *
 * a0 = new Status value
 */
nonmatching __osSetSR, 0x10

glabel __osSetSR
    mtc0        $a0, $12                    /* C0_SR */
    nop
    jr          $ra
     nop
