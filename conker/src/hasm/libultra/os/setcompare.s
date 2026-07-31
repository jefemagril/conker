.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

/*
 * __osSetCompare — arm the CP0 Compare register (CP0 $11).
 *
 * Writing Compare both schedules the next timer interrupt (when Count reaches
 * it) and clears the pending one. mtc0 has no C spelling, so SGI/libreultra
 * also ship setcompare as a tiny .s leaf.
 *
 * Tracked under src/hasm/ (gitignored asm/ is splat output only).
 *
 * a0 = Count value at which to fire the timer interrupt
 */
nonmatching __osSetCompare, 0xC

glabel __osSetCompare
    mtc0        $a0, $11                    /* C0_COMPARE */
    jr          $ra
     nop
    nop
