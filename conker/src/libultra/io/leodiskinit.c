#include <os_internal.h>
#include <rcp.h>
#include "piint.h"

/*
 * Conker omits domain/speed assignments present in later libultra (libreultra).
 *
 * NON-MATCHING at -O1 (IDO 5.3): pageSize/relDuration store scheduling vs
 * addiu sp differs (one extra insn). Left as GLOBAL_ASM rather than thrash.
 *
 * Matching attempt (libreultra-shaped, no domain/speed):
 *
 *   LeoDiskHandle.type = DEVICE_TYPE_64DD;
 *   LeoDiskHandle.baseAddress = PHYS_TO_K1(PI_DOM2_ADDR1);
 *   LeoDiskHandle.latency = 3;
 *   LeoDiskHandle.pulse = 6;
 *   LeoDiskHandle.pageSize = 6;
 *   LeoDiskHandle.relDuration = 2;
 *   IO_WRITE(PI_BSD_DOM2_LAT_REG, LeoDiskHandle.latency);
 *   IO_WRITE(PI_BSD_DOM2_PWD_REG, LeoDiskHandle.pulse);
 *   IO_WRITE(PI_BSD_DOM2_PGS_REG, LeoDiskHandle.pageSize);
 *   IO_WRITE(PI_BSD_DOM2_RLS_REG, LeoDiskHandle.relDuration);
 *   bzero(&LeoDiskHandle.transferInfo, sizeof(__OSTranxInfo));
 *   saveMask = __osDisableInt();
 *   LeoDiskHandle.next = __osPiTable;
 *   __osPiTable = &LeoDiskHandle;
 *   __osDiskHandle = &LeoDiskHandle;
 *   __osRestoreInt(saveMask);
 *   return &LeoDiskHandle;
 */

/* BSS via undefined_syms (not emitted from this TU) */
extern OSPiHandle LeoDiskHandle;   /* D_80043AC0 */
extern OSPiHandle *__osDiskHandle; /* D_80043B34 */
extern OSPiHandle *__osPiTable;    /* D_8002AB6C */

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/io/leodiskinit/osLeoDiskInit.s")
