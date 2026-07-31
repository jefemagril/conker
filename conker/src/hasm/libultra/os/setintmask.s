.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

/*
 * osSetIntMask — set the OS interrupt mask, returning the previous one.
 *
 * Leaving this in assembly is the most likely match of the original codebase:
 * it read-modify-writes CP0 Status and pokes MI_INTR_MASK_REG directly, so
 * SGI/libreultra also ship setintmask as a .s leaf.
 *
 * An OSIntMask packs the CP0 side in the low half (SR_IMASK plus SR_IE) and the
 * RCP side in the high half (MI_INTR_MASK bits). Both halves are intersected
 * with __OSGlobalIntMask so a caller can never enable an interrupt
 * the OS has globally disabled; the returned mask reports globally-masked
 * sources as enabled so save/restore round-trips cleanly.
 *
 * Tracked under src/hasm/ (gitignored asm/ is splat output only).
 *
 * __OSGlobalIntMask (0x8002BD1C) is defined by libultra/os/initialize.c
 * D_8002C850 = __osRcpImTable, the 64-entry MI_INTR -> MI_INTR_MASK set/clr LUT
 * D_A430000C = MI_INTR_MASK_REG (K1 window)
 *
 * a0 = new OSIntMask, returns the previous OSIntMask in v0
 */
nonmatching osSetIntMask, 0xA0

glabel osSetIntMask
    mfc0        $t4, $12                    /* C0_SR */

    /* old mask, CPU half: SR_IMASK | SR_IE from Status... */
    andi        $v0, $t4, 0xFF01            /* OS_IM_CPU */

    lui         $t0, %hi(__OSGlobalIntMask)
    addiu       $t0, $t0, %lo(__OSGlobalIntMask)
    lw          $t3, 0x0($t0)

    /* ...plus the CPU bits the global mask forbids, reported as enabled */
    addiu       $at, $zero, -0x1
    xor         $t0, $t3, $at
    andi        $t0, $t0, 0xFF00            /* SR_IMASK */
    or          $v0, $v0, $t0

    /* old mask, RCP half: MI_INTR_MASK_REG plus the globally-forbidden bits */
    lui         $t2, %hi(D_A430000C)        /* MI_INTR_MASK_REG */
    lw          $t2, %lo(D_A430000C)($t2)
    beqz        $t2, .Lrcp_done
     srl        $t1, $t3, 16
    addiu       $at, $zero, -0x1
    xor         $t1, $t1, $at
    andi        $t1, $t1, 0x3F              /* MI_INTR_MASK */
    or          $t2, $t2, $t1
.Lrcp_done:
    sll         $t2, $t2, 16
    or          $v0, $v0, $t2

    /* new RCP mask: index the set/clr LUT by (a0 & global) MI bits */
    lui         $at, (0x3F0000 >> 16)       /* MI_INTR_MASK << 16 */
    and         $t0, $a0, $at
    and         $t0, $t0, $t3
    srl         $t0, $t0, 15                /* >> 16, then * 2 for u16 entries */
    lui         $t2, %hi(D_8002C850)        /* __osRcpImTable */
    addu        $t2, $t2, $t0
    lhu         $t2, %lo(D_8002C850)($t2)
    lui         $at, %hi(D_A430000C)
    sw          $t2, %lo(D_A430000C)($at)

    /* new CPU mask: (a0 & OS_IM_CPU) & global, spliced over SR_IMASK */
    andi        $t0, $a0, 0xFF01            /* OS_IM_CPU */
    andi        $t1, $t3, 0xFF00            /* SR_IMASK */
    and         $t0, $t0, $t1
    lui         $at, (0xFFFF00FF >> 16)     /* ~SR_IMASK */
    ori         $at, $at, (0xFFFF00FF & 0xFFFF)
    and         $t4, $t4, $at
    or          $t4, $t4, $t0

    mtc0        $t4, $12
    nop
    nop
    jr          $ra
     nop
