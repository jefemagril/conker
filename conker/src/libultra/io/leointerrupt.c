#include <os_internal.h>
#include <rcp.h>
#include "piint.h"
#include "../os/osint.h"

/*
 * Conker leointerrupt differs substantially from libreultra / ultralib:
 *   - early-out if D_8002BD20 == 0 ("init ok?" gate; always 0 in data)
 *   - DMA-busy path: PI reset|clr_intr, optional mechanic clear, err 0x4B,
 *     AbnormalResume (not mask cart IRQ / err 29 / Resume)
 *   - WAIT loops check PI_STATUS_IO_BUSY only (not DMA_BUSY)
 *   - different internal errStatus values (3/6/0x11/0x4B vs 22/23/24/29/4)
 *   - no cmdType==2 early return; C2 path does not set cmdType=2
 * DKR stubs __osLeoInterrupt to return 0.
 *
 * NON-MATCHING: full bodies left as GLOBAL_ASM (too divergent to thrash).
 */

/* BSS / data via undefined_syms */
extern OSPiHandle *__osDiskHandle; /* D_80043B34 */
extern s32 D_8002BD20;             /* init gate */

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/io/leointerrupt/__osLeoInterrupt.s")
#pragma GLOBAL_ASM("asm/nonmatchings/libultra/io/leointerrupt/__osLeoAbnormalResume.s")
#pragma GLOBAL_ASM("asm/nonmatchings/libultra/io/leointerrupt/__osLeoResume.s")
