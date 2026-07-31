.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

/*
 * osWritebackDCacheAll — flush the entire data cache to RDRAM.
 *
 * Leaving this in assembly is the most likely match of the original codebase:
 * the cache instruction has no C spelling, so SGI/libreultra also ship
 * writebackdcacheall as a .s leaf.
 *
 * Index-writeback-invalidate every one of the 8 KiB / 16-byte-line entries,
 * addressed by index off K0BASE.
 *
 * Tracked under src/hasm/ (gitignored asm/ is splat output only).
 *
 * no arguments
 */
nonmatching osWritebackDCacheAll, 0x28

glabel osWritebackDCacheAll
    lui         $t0, (K0BASE >> 16)
    addiu       $t2, $zero, 0x2000          /* DCACHE_SIZE */
    addu        $t1, $t0, $t2
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
