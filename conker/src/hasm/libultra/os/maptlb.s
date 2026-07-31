.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

/*
 * osMapTLB — program one TLB entry by index.
 *
 * Leaving this in assembly is the most likely match of the original codebase:
 * the CP0 TLB registers and tlbwi have no C spelling, so SGI/libreultra also
 * ship maptlb as a .s leaf.
 *
 * An asid of -1 means "global": TLBLO_G goes into the entry attributes instead
 * of an ASID being folded into EntryHi. Either physical address may be -1 to
 * leave that half of the even/odd pair unmapped, in which case the half gets
 * TLBLO_G alone (no valid bit). When the odd half is mapped but the even half
 * is not, EntryHi is retargeted to K0BASE so the half-live entry can never
 * match a translation.
 *
 * Tracked under src/hasm/ (gitignored asm/ is splat output only).
 *
 * a0 = index, a1 = pagemask, a2 = vaddr, a3 = evenpaddr,
 * 0x10(sp) = oddpaddr, 0x14(sp) = asid
 */
nonmatching osMapTLB, 0xB4

glabel osMapTLB
    mfc0        $t0, $10                    /* save C0_ENTRYHI */
    mtc0        $a0, $0                     /* C0_INX = index */
    mtc0        $a1, $5                     /* C0_PAGEMASK = pagemask */

    lw          $t1, 0x14($sp)              /* asid */
    addiu       $at, $zero, -0x1
    beq         $t1, $at, .Lglobal
     addiu      $t4, $zero, 0x1             /* TLBLO_G */
    addiu       $t2, $zero, 0x1E            /* TLBLO_NONCOHRNT | TLBLO_D | TLBLO_V */
    b           .Lset_entryhi
     or         $a2, $a2, $t1               /* vaddr |= asid */
.Lglobal:
    addiu       $t2, $zero, 0x1F            /* ...| TLBLO_G */
.Lset_entryhi:
    mtc0        $a2, $10                    /* C0_ENTRYHI = vaddr (| asid) */

    /* even half */
    addiu       $at, $zero, -0x1
    beq         $a3, $at, .Leven_unmapped
     nop
    srl         $t3, $a3, 6                 /* TLBLO_PFNSHIFT */
    or          $t3, $t3, $t2
    mtc0        $t3, $2                     /* C0_ENTRYLO0 */
    b           .Lodd
     nop
.Leven_unmapped:
    mtc0        $t4, $2                     /* C0_ENTRYLO0 = TLBLO_G */

.Lodd:
    lw          $t3, 0x10($sp)              /* oddpaddr */
    addiu       $at, $zero, -0x1
    beq         $t3, $at, .Lodd_unmapped
     nop
    srl         $t3, $t3, 6                 /* TLBLO_PFNSHIFT */
    or          $t3, $t3, $t2
    mtc0        $t3, $3                     /* C0_ENTRYLO1 */
    b           .Lwrite
     nop
.Lodd_unmapped:
    mtc0        $t4, $3                     /* C0_ENTRYLO1 = TLBLO_G */
    addiu       $at, $zero, -0x1
    bne         $a3, $at, .Lwrite
     nop
    /* both halves unmapped: park EntryHi somewhere that can never match */
    lui         $t3, (K0BASE >> 16)
    mtc0        $t3, $10                    /* C0_ENTRYHI */

.Lwrite:
    nop
    tlbwi
    nop
    nop
    nop
    nop
    mtc0        $t0, $10                    /* restore C0_ENTRYHI */
    jr          $ra
     nop
    nop
    nop
    nop
