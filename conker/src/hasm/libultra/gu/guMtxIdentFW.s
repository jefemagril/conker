.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

/*
 * Identity float matrix with mf[3][0..2] taken from a1/a2/a3 (raw word stores).
 *
 * Same unrolled $f4 + delay-slot-final-swc1 schedule as guMtxIdentF — IDO cannot
 * recreate that from C (see src/hasm/libultra/gu/guMtxIdentF.s). Sibling of the
 * pure-identity hasm; only the three W-row stores differ (args vs $zero).
 *
 * a0 = float mf[4][4]
 * a1/a2/a3 = word values written to mf[3][0], mf[3][1], mf[3][2]
 */
nonmatching func_150A7DA0, 0x4C

glabel func_150A7DA0
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
    sw          $a1, 0x30($a0)              /* mf[3][0] = a1 */
    sw          $a2, 0x34($a0)              /* mf[3][1] = a2 */
    sw          $a3, 0x38($a0)              /* mf[3][2] = a3 */
    jr          $ra
     swc1       $f4, 0x3C($a0)              /* mf[3][3] = 1 (delay slot) */
    nop
