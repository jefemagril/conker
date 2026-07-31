.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

/*
 * osInvalICache — invalidate instruction-cache lines covering [vaddr, vaddr+nbytes).
 *
 * Leaving this in assembly is the most likely match of the original codebase:
 * the cache instruction has no C spelling, so SGI/libreultra also ship
 * invalicache as a .s leaf.
 *
 * The VR4300 I-cache is 16 KiB with 32-byte lines. A request that spans the
 * whole cache is cheaper to walk by index than by address, so anything at or
 * above ICACHE_SIZE falls through to an index-invalidate sweep of all lines
 * starting at K0BASE. Otherwise the range is rounded down to a line boundary
 * and hit-invalidated line by line.
 *
 * Tracked under src/hasm/ (gitignored asm/ is splat output only).
 *
 * a0 = vaddr, a1 = nbytes
 */
nonmatching osInvalICache, 0x74

glabel osInvalICache
    blez        $a1, .Ldone
     nop
    addiu       $t3, $zero, 0x4000          /* ICACHE_SIZE */
    sltu        $at, $a1, $t3
    beqz        $at, .Lwhole_cache
     nop

    or          $t0, $a0, $zero
    addu        $t1, $a0, $a1
    sltu        $at, $t0, $t1
    beqz        $at, .Ldone
     nop

    andi        $t2, $t0, 0x1F              /* ICACHE_LINEMASK */
    addiu       $t1, $t1, -0x20             /* -ICACHE_LINESIZE */
    subu        $t0, $t0, $t2               /* round start down to a line */
.Lhit_loop:
    cache       0x10, 0x0($t0)              /* C_HINV | CACH_PI */
    sltu        $at, $t0, $t1
    bnez        $at, .Lhit_loop
     addiu      $t0, $t0, 0x20
.Ldone:
    jr          $ra
     nop

/* Range covers the whole cache: sweep every line by index instead. */
.Lwhole_cache:
    lui         $t0, (K0BASE >> 16)
    addu        $t1, $t0, $t3
    addiu       $t1, $t1, -0x20             /* -ICACHE_LINESIZE */
.Lindex_loop:
    cache       0x00, 0x0($t0)              /* C_IINV | CACH_PI */
    sltu        $at, $t0, $t1
    bnez        $at, .Lindex_loop
     addiu      $t0, $t0, 0x20
    jr          $ra
     nop
    nop
    nop
    nop
