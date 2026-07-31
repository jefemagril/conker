.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

/*
 * osInvalDCache — invalidate data-cache lines covering [vaddr, vaddr+nbytes).
 *
 * Leaving this in assembly is the most likely match of the original codebase:
 * the cache instruction has no C spelling, so SGI/libreultra also ship
 * invaldcache as a .s leaf.
 *
 * The VR4300 D-cache is 8 KiB with 16-byte lines. A request at or above
 * DCACHE_SIZE is cheaper to sweep by index, so it falls through to an
 * index-writeback-invalidate of every line from K0BASE. Otherwise the interior
 * lines are plain hit-invalidated, but a partial line at either end is
 * hit-writeback-invalidated instead: those lines also hold bytes outside the
 * requested range, and discarding dirty data the caller did not ask about
 * would lose someone else's writes.
 *
 * Tracked under src/hasm/ (gitignored asm/ is splat output only).
 *
 * a0 = vaddr, a1 = nbytes
 */
nonmatching osInvalDCache, 0xAC

glabel osInvalDCache
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

    /* Unaligned start: write back the shared leading line before invalidating. */
    andi        $t2, $t0, 0xF               /* DCACHE_LINEMASK */
    beqz        $t2, .Lcheck_end
     addiu      $t1, $t1, -0x10             /* -DCACHE_LINESIZE */
    subu        $t0, $t0, $t2
    cache       0x15, 0x0($t0)              /* C_HWBINV | CACH_PD */
    sltu        $at, $t0, $t1
    beqz        $at, .Ldone
     nop
    addiu       $t0, $t0, 0x10

.Lcheck_end:
    /* Unaligned end: same treatment for the shared trailing line. */
    andi        $t2, $t1, 0xF               /* DCACHE_LINEMASK */
    beqz        $t2, .Lhit_loop
     nop
    subu        $t1, $t1, $t2
    cache       0x15, 0x10($t1)             /* C_HWBINV | CACH_PD */
    sltu        $at, $t1, $t0
    bnez        $at, .Ldone
     nop

.Lhit_loop:
    cache       0x11, 0x0($t0)              /* C_HINV | CACH_PD */
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
