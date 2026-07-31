#include <os_internal.h>
#include <rcp.h>
#include "viint.h"
#include "../os/osint.h"

extern OSDevMgr __osViDevMgr;
extern OSThread viThread;
extern char viThreadStack[OS_VIM_STACKSIZE];
extern OSMesgQueue viEventQueue;
extern OSMesg viEventBuf[5];
extern OSIoMesg viRetraceMsg;
extern OSIoMesg viCounterMsg;

void viMgrMain(void *arg);

void osCreateViManager(OSPri pri)
{
	u32 savedMask;
	OSPri oldPri;
	OSPri myPri;
	if (__osViDevMgr.active == 0)
	{
		__osTimerServicesInit();
		osCreateMesgQueue(&viEventQueue, viEventBuf, 5);
		viRetraceMsg.hdr.type = OS_MESG_TYPE_VRETRACE;
		viRetraceMsg.hdr.pri = OS_MESG_PRI_NORMAL;
		viRetraceMsg.hdr.retQueue = NULL;
		viCounterMsg.hdr.type = OS_MESG_TYPE_COUNTER;
		viCounterMsg.hdr.pri = OS_MESG_PRI_NORMAL;
		viCounterMsg.hdr.retQueue = NULL;
		osSetEventMesg(OS_EVENT_VI, &viEventQueue, (OSMesg)&viRetraceMsg);
		osSetEventMesg(OS_EVENT_COUNTER, &viEventQueue, (OSMesg)&viCounterMsg);
		oldPri = -1;
		myPri = osGetThreadPri(NULL);
		if (myPri < pri)
		{
			oldPri = myPri;
			osSetThreadPri(NULL, pri);
		}
		savedMask = __osDisableInt();
		__osViDevMgr.active = 1;
		__osViDevMgr.thread = &viThread;
		__osViDevMgr.cmdQueue = &viEventQueue;
		__osViDevMgr.evtQueue = &viEventQueue;
		__osViDevMgr.acsQueue = NULL;
		__osViDevMgr.dma = NULL;
		__osViDevMgr.edma = NULL;
		osCreateThread(&viThread, 0, viMgrMain, &__osViDevMgr, &viThreadStack[OS_VIM_STACKSIZE], pri);
		__osViInit();
		osStartThread(&viThread);
		__osRestoreInt(savedMask);
		if (oldPri != -1)
		{
			osSetThreadPri(NULL, oldPri);
		}
	}
}

/*
 * NON-MATCHING / not currently linked (yaml still uses asm/libultra/io/vimgr.s).
 *
 * osCreateViManager matches at -O2. viMgrMain is sibling-shaped but IDO -O2
 * prologue is sw ra; sw s7; move s7,a0 vs original sw s7; move s7,a0; sw ra.
 * Re-promote when a prologue schedule fix is found (workflow step 10).
 *
 * void viMgrMain(void *arg) { ... see asm/libultra/io/vimgr.s ... }
 */
void viMgrMain(void *arg);
