#include <os_internal.h>
#include "osint.h"

/* BSS globals (__osThreadTail, run/active/running/faulted queues) live outside this TU. */

void __osDequeueThread(OSThread **queue, OSThread *t)
{
	register OSThread *pred;
	register OSThread *succ;

	pred = (OSThread *)queue;
	succ = pred->next;
	while (succ != NULL)
	{
		if (succ == t)
		{
			pred->next = t->next;
			return;
		}
		pred = succ;
		succ = pred->next;
	}
}
