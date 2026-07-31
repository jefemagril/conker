#include <os_internal.h>
#include "osint.h"

/*
 * __osTimerServicesInit: GLOBAL_ASM — __osCurrentTime=0 at -O1 emits an extra
 * lui vs original (high/low halves with one lui). Left as asm rather than thrash.
 */
#pragma GLOBAL_ASM("asm/nonmatchings/libultra/os/timerintr/__osTimerServicesInit.s")

/* BSS via undefined_syms / symbol_addrs (not emitted from this TU) */
extern OSTimer *__osTimerList;
extern u32 __osTimerCounter;

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
