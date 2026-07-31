.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

/*
 * osWritebackDCache — flush dirty data-cache lines covering [vaddr, vaddr+nbytes)
 * out to RDRAM so the RCP or a DMA engine sees the caller's writes.
 *
 * Leaving this in assembly is the most likely match of the original codebase:
 * the cache instruction has no C spelling, so SGI/libreultra also ship
 * writebackdcache as a .s leaf.
 *
 * A request at or above DCACHE_SIZE is cheaper to sweep by index, so it falls
 * through to an index-writeback-invalidate of every line from K0BASE.
 * Otherwise the range is rounded down to a line boundary and each line is
 * hit-written-back; unlike the invalidate path this never discards data, so
 * partial lines at the ends need no special case.
 *
 * Tracked under src/hasm/ (gitignored asm/ is splat output only).
 *
 * a0 = vaddr, a1 = nbytes
 */
nonmatching osWritebackDCache, 0x74

glabel osWritebackDCache
    blez        $a1, .Ldone
     nop
    addiu       $t3, $zero, 0x2000          /* DCACHE_SIZE */
    sltu        $at, $a1, $t3
    beqz        $at, .Lwhole_cache
     nop

    or          $t0, $a0, $zero
    addu        $t1, $a0, $a1
    sltu        $at, $t0, $t1
    beqz        $at, .Ldone
     nop

    andi        $t2, $t0, 0xF               /* DCACHE_LINEMASK */
    addiu       $t1, $t1, -0x10             /* -DCACHE_LINESIZE */
    subu        $t0, $t0, $t2               /* round start down to a line */
.Lhit_loop:
    cache       0x19, 0x0($t0)              /* C_HWB | CACH_PD */
    sltu        $at, $t0, $t1
    bnez        $at, .Lhit_loop
     addiu      $t0, $t0, 0x10
.Ldone:
    jr          $ra
     nop

/* Range covers the whole cache: sweep every line by index instead. */
.Lwhole_cache:
    lui         $t0, (K0BASE >> 16)
    addu        $t1, $t0, $t3
    addiu       $t1, $t1, -0x10             /* -DCACHE_LINESIZE */
.Lindex_loop:
    cache       0x01, 0x0($t0)              /* C_IWBINV | CACH_PD */
    sltu        $at, $t0, $t1
    bnez        $at, .Lindex_loop
     addiu      $t0, $t0, 0x10
    jr          $ra
     nop
    nop
    nop
    nop
