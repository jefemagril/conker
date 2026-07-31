#include <os_internal.h>
#include "osint.h"

/* __osTimerList lives elsewhere; its address comes from undefined_syms. */
extern OSTimer *__osTimerList;

/*
 * These four must be defined here, not declared extern. When they are extern,
 * `__osCurrentTime = 0` materialises a separate %hi for each half of the u64
 * and costs an extra lui; defining them locally lets both stores share one
 * `lui $at`, which is what the original does. The addresses still come from
 * undefined_syms (0x800429B0..0x800429C0) — this .bss is discarded at link.
 */
OSTime __osCurrentTime;
u32 __osBaseCounter;
u32 __osViIntrCount;
u32 __osTimerCounter;

void __osTimerServicesInit(void)
{
	__osCurrentTime = 0;
	__osBaseCounter = 0;
	__osViIntrCount = 0;

	__osTimerList->next = __osTimerList->prev = __osTimerList;
	__osTimerList->interval = __osTimerList->value = 0;
	__osTimerList->mq = NULL;
	__osTimerList->msg = NULL;
}

void __osTimerInterrupt(void)
{
	OSTimer *t;
	u32 count;
	u32 elapsed_cycles;

	if (__osTimerList->next == __osTimerList)
		return;
	while (1)
	{
		t = __osTimerList->next;
		if (t == __osTimerList)
		{
			__osSetCompare(0);
			__osTimerCounter = 0;
			break;
		}
		count = osGetCount();
		elapsed_cycles = count - __osTimerCounter;
		__osTimerCounter = count;
		if (elapsed_cycles < t->value)
		{
			t->value -= elapsed_cycles;
			__osSetTimerIntr(t->value);
			return;
		}
		else
		{
			t->prev->next = t->next;
			t->next->prev = t->prev;
			t->next = NULL;
			t->prev = NULL;
			if (t->mq != NULL)
			{
				osSendMesg(t->mq, t->msg, OS_MESG_NOBLOCK);
			}
			if (t->interval != 0)
			{
				t->value = t->interval;
				__osInsertTimer(t);
			}
		}
	}
}

void __osSetTimerIntr(OSTime tim)
{
	OSTime NewTime;
	u32 savedMask;
	savedMask = __osDisableInt();
	__osTimerCounter = osGetCount();
	NewTime = tim + __osTimerCounter;
	__osSetCompare(NewTime);
	__osRestoreInt(savedMask);
}

OSTime __osInsertTimer(OSTimer *t)
{
	OSTimer *timep;
	OSTime tim;
	u32 savedMask;
	savedMask = __osDisableInt();
	for (timep = __osTimerList->next, tim = t->value;
		 timep != __osTimerList && tim > timep->value;
		 tim -= timep->value, timep = timep->next)
	{
		;
	}
	t->value = tim;
	if (timep != __osTimerList)
		timep->value -= tim;
	t->next = timep;
	t->prev = timep->prev;
	timep->prev->next = t;
	timep->prev = t;
	__osRestoreInt(savedMask);
	return tim;
}
