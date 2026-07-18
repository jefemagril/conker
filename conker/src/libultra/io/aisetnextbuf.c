#include <os_internal.h>
#include <rcp.h>
#include "../os/osint.h"

/*
 * AI DMA hardware bug workaround.
 * When (vaddr + nbytes) & AI_BUG_ADDR_MASK == AI_BUG_ADDR_VALUE, the next
 * transfer reads from vaddr + AI_BUG_ADDR_VALUE instead of vaddr. Compensate
 * by subtracting AI_BUG_ADDR_VALUE from the following buffer pointer.
 * See osAiSetNextBuffer man page / Nintendo Developer News 1.2.
 */
#define AI_BUG_ADDR_MASK  0x3fff
#define AI_BUG_ADDR_VALUE 0x2000

s32 osAiSetNextBuffer(void *bufPtr, u32 size)
{
	static u8 hdwrBugFlag = 0;
	char *bptr = bufPtr;
	if (hdwrBugFlag != 0)
		bptr -= AI_BUG_ADDR_VALUE;

	if ((((s32)bufPtr + size) & AI_BUG_ADDR_MASK) == AI_BUG_ADDR_VALUE)
		hdwrBugFlag = 1;
	else
		hdwrBugFlag = 0;

	if (__osAiDeviceBusy())
		return -1;

	IO_WRITE(AI_DRAM_ADDR_REG, osVirtualToPhysical(bptr));
	IO_WRITE(AI_LEN_REG, size);
	return 0;
}
