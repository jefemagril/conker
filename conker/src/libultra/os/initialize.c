#include <os_internal.h>
#include <rcp.h>
#include <R4300.h>
#include "piint.h"
#include "osint.h"

/*
 * Size of buffer that retains contents after NMI
 */
#define OS_APP_NMI_BUFSIZE 64

typedef struct {
    /* 0x0 */ u32 inst1;
    /* 0x4 */ u32 inst2;
    /* 0x8 */ u32 inst3;
    /* 0xC */ u32 inst4;
} __osExceptionVector;

extern __osExceptionVector __osExceptionPreamble[];

/*
 * .data at 0x8002BD10. These have to be defined here rather than pulled in via
 * undefined_syms: IDO only shares one %hi between the two halves of a 64-bit
 * store when the object is defined in the same TU, which is what the original
 * osClockRate stores do. gLeoFound (0x8002BD20) belongs to this TU as well but
 * is left in the data blob so this .data stays exactly 0x10 bytes.
 */
OSTime osClockRate = OS_CLOCK_RATE;
u32 __osShutdown = 0;
OSIntMask __OSGlobalIntMask = OS_IM_ALL;

extern u32 __osFinalrom;
extern s32 gLeoFound;
extern s32 __osLeoInterrupt(void);
extern s32 osPiRawReadIo(u32, u32 *);

void __osInitialize_common(void) {
    u32 pifdata;
    u32 clock = 0;
    u32 leostatus;
    u32 pistatus;

    __osFinalrom = TRUE;

    __osSetSR(__osGetSR() | SR_CU1);                  /* enable fpu */
    __osSetFpcCsr(FPCSR_FS | FPCSR_EV | FPCSR_RM_RN); /* flush denorm to zero, enable invalid operation */

    while (__osSiRawReadIo(PIF_RAM_END - 3, &pifdata)) { /* last byte of joychannel ram */
        ;
    }
    while (__osSiRawWriteIo(PIF_RAM_END - 3, pifdata | 8)) {
        ;
    }

    *(__osExceptionVector *)UT_VEC = *__osExceptionPreamble;
    *(__osExceptionVector *)XUT_VEC = *__osExceptionPreamble;
    *(__osExceptionVector *)ECC_VEC = *__osExceptionPreamble;
    *(__osExceptionVector *)E_VEC = *__osExceptionPreamble;
    osWritebackDCache((void *)UT_VEC, E_VEC - UT_VEC + sizeof(__osExceptionVector));
    osInvalICache((void *)UT_VEC, E_VEC - UT_VEC + sizeof(__osExceptionVector));

    osMapTLBRdb();
    osPiRawReadIo(4, &clock); /* Read clock rate from the ROM header */
    clock &= ~0xF;
    if (clock != 0) {
        osClockRate = clock;
    }
    osClockRate = osClockRate * 3 / 4;

    if (osResetType == 0) { /* cold reset */
        bzero(osAppNMIBuffer, OS_APP_NMI_BUFSIZE);
    }

    WAIT_ON_IOBUSY(pistatus);

    if (!((leostatus = IO_READ(LEO_STATUS)) & LEO_STATUS_PRESENCE_MASK)) {
        gLeoFound = TRUE;
        __osSetHWIntrRoutine(1, __osLeoInterrupt);
    } else {
        gLeoFound = FALSE;
    }
}
