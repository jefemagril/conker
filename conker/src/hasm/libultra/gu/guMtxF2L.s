.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

/*
 * guMtxF2L — convert a 4x4 float matrix to an N64 fixed-point Mtx.
 *
 * Leaving this in assembly is the most likely match of Rare's original source:
 * sibling libultra trees only have the packed-word loop (matched here as
 * guMtxF2L2). This early copy is a fully unrolled, hand-scheduled halfword
 * path: scale by 65536, bare cvt.w.s, then sh of low/high halves into the Mtx
 * frac/int sections. IDO from C emits trunc.w.s (or cvt.w.s wrapped in FCSR
 * save/restore for unsigned casts) and will not produce this interleave.
 *
 * Tracked under src/hasm/ (gitignored asm/ is splat output only).
 *
 * For each float: fixed = (s32)(f * 65536)
 *   - low 16 bits  -> Mtx fractional half  (m->m[2]…, byte offset +0x20)
 *   - high 16 bits -> Mtx integer half     (m->m[0]…, byte offset +0x00)
 *
 * a0 = float mf[4][4]
 * a1 = Mtx *m
 */
nonmatching guMtxF2L, 0x1CC

glabel guMtxF2L
    lui         $at, 0x4780                 /* 65536.0f */
    mtc1        $at, $f18

    /* mf[0][0], mf[0][1] */
    lwc1        $f2, 0x0($a0)
    mul.s       $f2, $f2, $f18
    lwc1        $f4, 0x4($a0)
    cvt.w.s     $f2, $f2
    mfc1        $t0, $f2
    mul.s       $f4, $f4, $f18
    sh          $t0, 0x20($a1)              /* frac [0][0] */
    cvt.w.s     $f4, $f4
    srl         $t0, $t0, 16
    mfc1        $t1, $f4
    sh          $t0, 0x0($a1)               /* int  [0][0] */
    sh          $t1, 0x22($a1)              /* frac [0][1] */
    srl         $t1, $t1, 16
    sh          $t1, 0x2($a1)               /* int  [0][1] */

    /* mf[0][2], mf[0][3] */
    lwc1        $f2, 0x8($a0)
    mul.s       $f2, $f2, $f18
    lwc1        $f4, 0xC($a0)
    cvt.w.s     $f2, $f2
    mfc1        $t0, $f2
    mul.s       $f4, $f4, $f18
    sh          $t0, 0x24($a1)
    cvt.w.s     $f4, $f4
    srl         $t0, $t0, 16
    mfc1        $t1, $f4
    sh          $t0, 0x4($a1)
    sh          $t1, 0x26($a1)
    srl         $t1, $t1, 16
    sh          $t1, 0x6($a1)

    /* mf[1][0], mf[1][1] */
    lwc1        $f2, 0x10($a0)
    mul.s       $f2, $f2, $f18
    lwc1        $f4, 0x14($a0)
    cvt.w.s     $f2, $f2
    mfc1        $t0, $f2
    mul.s       $f4, $f4, $f18
    sh          $t0, 0x28($a1)
    cvt.w.s     $f4, $f4
    srl         $t0, $t0, 16
    mfc1        $t1, $f4
    sh          $t0, 0x8($a1)
    sh          $t1, 0x2A($a1)
    srl         $t1, $t1, 16
    sh          $t1, 0xA($a1)

    /* mf[1][2], mf[1][3] */
    lwc1        $f2, 0x18($a0)
    mul.s       $f2, $f2, $f18
    lwc1        $f4, 0x1C($a0)
    cvt.w.s     $f2, $f2
    mfc1        $t0, $f2
    mul.s       $f4, $f4, $f18
    sh          $t0, 0x2C($a1)
    cvt.w.s     $f4, $f4
    srl         $t0, $t0, 16
    mfc1        $t1, $f4
    sh          $t0, 0xC($a1)
    sh          $t1, 0x2E($a1)
    srl         $t1, $t1, 16
    sh          $t1, 0xE($a1)

    /* mf[2][0], mf[2][1] */
    lwc1        $f2, 0x20($a0)
    mul.s       $f2, $f2, $f18
    lwc1        $f4, 0x24($a0)
    cvt.w.s     $f2, $f2
    mfc1        $t0, $f2
    mul.s       $f4, $f4, $f18
    sh          $t0, 0x30($a1)
    cvt.w.s     $f4, $f4
    srl         $t0, $t0, 16
    mfc1        $t1, $f4
    sh          $t0, 0x10($a1)
    sh          $t1, 0x32($a1)
    srl         $t1, $t1, 16
    sh          $t1, 0x12($a1)

    /* mf[2][2], mf[2][3] */
    lwc1        $f2, 0x28($a0)
    mul.s       $f2, $f2, $f18
    lwc1        $f4, 0x2C($a0)
    cvt.w.s     $f2, $f2
    mfc1        $t0, $f2
    mul.s       $f4, $f4, $f18
    sh          $t0, 0x34($a1)
    cvt.w.s     $f4, $f4
    srl         $t0, $t0, 16
    mfc1        $t1, $f4
    sh          $t0, 0x14($a1)
    sh          $t1, 0x36($a1)
    srl         $t1, $t1, 16
    sh          $t1, 0x16($a1)

    /* mf[3][0], mf[3][1] */
    lwc1        $f2, 0x30($a0)
    mul.s       $f2, $f2, $f18
    lwc1        $f4, 0x34($a0)
    cvt.w.s     $f2, $f2
    mfc1        $t0, $f2
    mul.s       $f4, $f4, $f18
    sh          $t0, 0x38($a1)
    cvt.w.s     $f4, $f4
    srl         $t0, $t0, 16
    mfc1        $t1, $f4
    sh          $t0, 0x18($a1)
    sh          $t1, 0x3A($a1)
    srl         $t1, $t1, 16
    sh          $t1, 0x1A($a1)

    /* mf[3][2], mf[3][3] */
    lwc1        $f2, 0x38($a0)
    mul.s       $f2, $f2, $f18
    lwc1        $f4, 0x3C($a0)
    cvt.w.s     $f2, $f2
    mfc1        $t0, $f2
    mul.s       $f4, $f4, $f18
    sh          $t0, 0x3C($a1)
    cvt.w.s     $f4, $f4
    srl         $t0, $t0, 16
    mfc1        $t1, $f4
    sh          $t0, 0x1C($a1)
    sh          $t1, 0x3E($a1)
    srl         $t1, $t1, 16
    jr          $ra
     sh         $t1, 0x1E($a1)              /* int [3][3] (delay slot) */
    nop
