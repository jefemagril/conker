#include <os_internal.h>

extern u32 __osSiAccessQueueEnabled;
extern OSMesgQueue __osSiAccessQueue;
extern OSMesg siAccessBuf2[1];

void __osSiCreateAccessQueue(void);

void __osSiCreateAccessQueue2(void)
{
	__osSiAccessQueueEnabled = 1;
	osCreateMesgQueue(&__osSiAccessQueue, siAccessBuf2, 1);
	osSendMesg(&__osSiAccessQueue, NULL, OS_MESG_NOBLOCK);
}

void __osSiGetAccess2(void)
{
	OSMesg dummyMesg;
	if (!__osSiAccessQueueEnabled)
		__osSiCreateAccessQueue();
	osRecvMesg(&__osSiAccessQueue, &dummyMesg, OS_MESG_BLOCK);
}

void __osSiRelAccess2(void)
{
	osSendMesg(&__osSiAccessQueue, NULL, OS_MESG_NOBLOCK);
}
