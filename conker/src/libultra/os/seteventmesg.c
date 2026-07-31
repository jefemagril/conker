#include <os_internal.h>
#include "osint.h"

/* BSS via undefined_syms */
extern __OSEventState __osEventStateTab[];

void osSetEventMesg(OSEvent event, OSMesgQueue *mq, OSMesg msg)
{
	register u32 saveMask = __osDisableInt();
	__OSEventState *es;

	es = &__osEventStateTab[event];
	es->messageQueue = mq;
	es->message = msg;
	__osRestoreInt(saveMask);
}
