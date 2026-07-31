.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

/*
 * __osSetFpcCsr — swap the FPU control/status register (FCR31).
 *
 * Leaving this in assembly is the most likely match of the original codebase:
 * cfc1/ctc1 have no C spelling, so SGI/libreultra also ship setfpccsr as a
 * tiny .s leaf.
 *
 * Tracked under src/hasm/ (gitignored asm/ is splat output only).
 *
 * a0 = new FpcCsr value, returns the previous value in v0
 */
nonmatching __osSetFpcCsr, 0x10

glabel __osSetFpcCsr
    cfc1        $v0, $31
    ctc1        $a0, $31
    jr          $ra
     nop
