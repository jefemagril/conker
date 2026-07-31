#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/*
 * NON-MATCHING: DKR-shaped attempt (clock from ROM, PI DMA|IO busy wait, Leo
 * presence) came out 0x26c vs 0x2A0 — exception-vector copy / schedule diverge
 * from Conker's fully unrolled stores. Left as GLOBAL_ASM rather than thrash.
 *
 * Matching attempt notes:
 *   - named __osInitialize_common (not osInitialize)
 *   - no osTvType / osViClock assignment
 *   - PI wait uses DMA_BUSY|IO_BUSY (0x3), not PI_STATUS_ERROR alone
 *   - Leo: IO_READ(LEO_STATUS) & PRESENCE_MASK == 0 → gLeoFound + HW intr
 */
#pragma GLOBAL_ASM("asm/nonmatchings/libultra/os/initialize/__osInitialize_common.s")
