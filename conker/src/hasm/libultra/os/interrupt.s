.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

/*
 * __osDisableInt / __osRestoreInt — the libultra critical-section pair.
 *
 * Leaving these in assembly is the most likely match of the original codebase:
 * they read-modify-write CP0 Status, which has no C spelling, so SGI/libreultra
 * also ship interrupt.s as a pair of .s leaves. The trailing nops cover the
 * CP0 hazard before the new interrupt-enable state takes effect.
 *
 * Tracked under src/hasm/ (gitignored asm/ is splat output only).
 */

/*
 * __osDisableInt — clear SR_IE, returning the previous SR_IE bit so the caller
 * can hand it straight back to __osRestoreInt.
 *
 * returns the old SR_IE bit (0 or 1) in v0
 */
nonmatching __osDisableInt, 0x20

glabel __osDisableInt
    mfc0        $t0, $12                    /* C0_SR */
    addiu       $at, $zero, -0x2            /* ~SR_IE */
    and         $t1, $t0, $at
    mtc0        $t1, $12
    andi        $v0, $t0, 0x1               /* SR_IE */
    nop
    jr          $ra
     nop

/*
 * __osRestoreInt — OR the saved SR_IE bit back into Status.
 *
 * a0 = value previously returned by __osDisableInt
 */
nonmatching __osRestoreInt, 0x1C

glabel __osRestoreInt
    mfc0        $t0, $12                    /* C0_SR */
    or          $t0, $t0, $a0
    mtc0        $t0, $12
    nop
    nop
    jr          $ra
     nop
    nop
