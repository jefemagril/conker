.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

/*
 * sqrtf — single-precision square root.
 *
 * Leaving this in assembly is the most likely match of the original codebase:
 * SGI/libreultra also ships sqrtf as a tiny .s leaf (sqrt.s + jr), not C.
 * IDO would normally inline or call a libm helper; this ROM has an explicit
 * two-instruction leaf.
 *
 * f12 = x, returns in f0
 */
nonmatching sqrtf, 0x8

glabel sqrtf
    jr          $ra
     sqrt.s     $f0, $f12
    nop
    nop
