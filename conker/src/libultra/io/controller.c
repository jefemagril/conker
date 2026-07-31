#include <os_internal.h>
#include <rcp.h>
#include "controller.h"
#include "siint.h"

#define HALF_MIL_CYCLES 500000U
#define ONE_MIL_CYCLES 1000000U
#define HALF_A_SECOND (HALF_MIL_CYCLES * osClockRate / ONE_MIL_CYCLES)

/* data via symbol_addrs (not emitted from this TU) */
extern u32 __osContInitialized;
extern u64 osClockRate;

s32 osContInit(OSMesgQueue *mq, u8 *bitpattern, OSContStatus *data)
{
	OSMesg dummy;
	s32 ret;
	OSTime t;
	OSTimer mytimer;
	OSMesgQueue timerMesgQueue;

	ret = 0;
	if (__osContInitialized)
	{
		return 0;
	}

	__osContInitialized = 1;
	t = osGetTime();
	if (HALF_A_SECOND > t)
	{
		osCreateMesgQueue(&timerMesgQueue, &dummy, 1);
		osSetTimer(&mytimer, HALF_A_SECOND - t, 0, &timerMesgQueue, &dummy);
		osRecvMesg(&timerMesgQueue, &dummy, OS_MESG_BLOCK);
	}

	__osMaxControllers = MAXCONTROLLERS;
	__osPackRequestData(CONT_CMD_REQUEST_STATUS);

	ret = __osSiRawStartDma(OS_WRITE, &__osContPifRam);
	osRecvMesg(mq, &dummy, OS_MESG_BLOCK);

	ret = __osSiRawStartDma(OS_READ, &__osContPifRam);
	osRecvMesg(mq, &dummy, OS_MESG_BLOCK);

	__osContGetInitData(bitpattern, data);
	__osContLastCmd = CONT_CMD_REQUEST_STATUS;
	__osSiCreateAccessQueue();
	osCreateMesgQueue(&__osEepromTimerQ, &__osEepromTimerMsg, 1);

	return ret;
}

void __osContGetInitData(u8 *pattern, OSContStatus *data)
{
    u8 *ptr;
    __OSContRequesFormat requestformat;
    int i;
    u8 bits;

    bits = 0;
    ptr = (u8 *)&__osContPifRam;
    for (i = 0; i < __osMaxControllers; i++, ptr += sizeof(__OSContRequesFormat), data++)
    {
        requestformat = *(__OSContRequesFormat *)ptr;
        data->errno = CHNL_ERR(requestformat);
        if (data->errno == 0)
        {
            data->type = (requestformat.typel << 8) | requestformat.typeh;
            data->status = requestformat.status;
            bits |= 1 << i;
        }
    }
    *pattern = bits;
}

void __osPackRequestData(u8 cmd)
{
    u8 *ptr;
    __OSContRequesFormat requestformat;
    int i;

    for (i = 0; i < ARRLEN(__osContPifRam.ramarray) + 1; i++)
    {
        __osContPifRam.ramarray[i] = 0;
    }

    __osContPifRam.pifstatus = CONT_CMD_EXE;
    ptr = (u8 *)&__osContPifRam.ramarray;
    requestformat.dummy = CONT_CMD_NOP;
    requestformat.txsize = CONT_CMD_REQUEST_STATUS_TX;
    requestformat.rxsize = CONT_CMD_REQUEST_STATUS_RX;
    requestformat.cmd = cmd;
    requestformat.typeh = CONT_CMD_NOP;
    requestformat.typel = CONT_CMD_NOP;
    requestformat.status = CONT_CMD_NOP;
    requestformat.dummy1 = CONT_CMD_NOP;

    for (i = 0; i < __osMaxControllers; i++)
    {
        *(__OSContRequesFormat *)ptr = requestformat;
        ptr += sizeof(__OSContRequesFormat);
    }
    *ptr = CONT_CMD_END;
}
