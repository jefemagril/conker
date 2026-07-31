.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

/*
 * __osProbeTLB — translate a virtual address through the TLB by hand.
 *
 * Leaving this in assembly is the most likely match of the original codebase:
 * tlbp/tlbr and the CP0 TLB registers have no C spelling, so SGI/libreultra
 * also ship probetlb as a .s leaf.
 *
 * Sets EntryHi to the probed VPN2 (keeping the current ASID), runs tlbp, and
 * bails out with -1 if the probe missed. On a hit, tlbr reads the entry back;
 * the PageMask decides the page size, whose top bit picks the even or odd half
 * of the pair and whose remaining bits are the in-page offset. If the chosen
 * half is not valid the probe also fails. The caller's EntryHi is restored on
 * every path, and the nop runs cover the tlbp/tlbr CP0 hazards.
 *
 * Tracked under src/hasm/ (gitignored asm/ is splat output only).
 *
 * a0 = virtual address, returns the physical address in v0 or -1 on failure
 */
nonmatching __osProbeTLB, 0xB8

glabel __osProbeTLB
    mfc0        $t0, $10                    /* save C0_ENTRYHI */
    andi        $t1, $t0, 0xFF              /* TLBHI_PIDMASK: keep current ASID */
    addiu       $at, $zero, -0x2000         /* TLBHI_VPN2MASK */
    and         $t2, $a0, $at
    or          $t1, $t1, $t2
    mtc0        $t1, $10                    /* C0_ENTRYHI */
    nop
    nop
    nop
    tlbp
    nop
    nop
    mfc0        $t3, $0                     /* C0_INX */
    lui         $at, (0x80000000 >> 16)     /* TLBINX_PROBE: set on miss */
    and         $t3, $t3, $at
    bnez        $t3, .Lfail
     nop

    tlbr
    nop
    nop
    nop
    /* page size = (pagemask + 0x2000) >> 1; its top bit selects even/odd */
    mfc0        $t3, $5                     /* C0_PAGEMASK */
    addi        $t3, $t3, 0x2000
    srl         $t3, $t3, 1
    and         $t4, $t3, $a0
    bnez        $t4, .Lodd
     addi       $t3, $t3, -0x1              /* now the in-page offset mask */
    mfc0        $v0, $2                     /* C0_ENTRYLO0 */
    b           .Lhave_entrylo
     nop
.Lodd:
    mfc0        $v0, $3                     /* C0_ENTRYLO1 */

.Lhave_entrylo:
    andi        $t5, $v0, 0x2               /* TLBLO_V */
    beqz        $t5, .Lfail
     nop
    lui         $at, (0x3FFFFFC0 >> 16)     /* TLBLO_PFNMASK */
    ori         $at, $at, (0x3FFFFFC0 & 0xFFFF)
    and         $v0, $v0, $at
    sll         $v0, $v0, 6                 /* TLBLO_PFNSHIFT */
    and         $t5, $a0, $t3
    add         $v0, $v0, $t5               /* + in-page offset */
    b           .Ldone
     nop
.Lfail:
    addiu       $v0, $zero, -0x1
.Ldone:
    mtc0        $t0, $10                    /* restore C0_ENTRYHI */
    jr          $ra
     nop
    nop
    nop
