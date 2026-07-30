.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

/*
 * guMtxIdentF — write a 4x4 float identity matrix.
 *
 * Leaving this in assembly is the most likely match of Rare's original source:
 * sibling libultra trees (libreultra / PD / DK64) only have the small looped C
 * version, which Conker already matched separately as guMtxIdentF2. This early
 * copy is fully unrolled with a hand-picked FP register ($f4) and a delay-slot
 * final store — the same style as Rare's handwritten matrix helpers (e.g. PD
 * mtxasm.s). IDO cannot recreate that schedule from C.
 *
 * Tracked under src/hasm/ (gitignored asm/ is splat output only).
 *
 * a0 = float mf[4][4]
 */
nonmatching guMtxIdentF, 0x4C

glabel guMtxIdentF
    lui         $at, 0x3F80                 /* 1.0f bit pattern */
    mtc1        $at, $f4
    sw          $zero, 0x4($a0)             /* mf[0][1] = 0 */
    swc1        $f4, 0x0($a0)               /* mf[0][0] = 1 */
    sw          $zero, 0x8($a0)             /* mf[0][2] = 0 */
    sw          $zero, 0xC($a0)             /* mf[0][3] = 0 */
    sw          $zero, 0x10($a0)            /* mf[1][0] = 0 */
    swc1        $f4, 0x14($a0)              /* mf[1][1] = 1 */
    sw          $zero, 0x18($a0)            /* mf[1][2] = 0 */
    sw          $zero, 0x1C($a0)            /* mf[1][3] = 0 */
    sw          $zero, 0x20($a0)            /* mf[2][0] = 0 */
    sw          $zero, 0x24($a0)            /* mf[2][1] = 0 */
    swc1        $f4, 0x28($a0)              /* mf[2][2] = 1 */
    sw          $zero, 0x2C($a0)            /* mf[2][3] = 0 */
    sw          $zero, 0x30($a0)            /* mf[3][0] = 0 */
    sw          $zero, 0x34($a0)            /* mf[3][1] = 0 */
    sw          $zero, 0x38($a0)            /* mf[3][2] = 0 */
    jr          $ra
     swc1       $f4, 0x3C($a0)              /* mf[3][3] = 1 (delay slot) */
    nop
