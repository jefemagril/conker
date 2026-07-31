#include <os_internal.h>
#include <rcp.h>

int __osSpDeviceBusy(void)
{
	register s32 status = IO_READ(SP_STATUS_REG);
	if (status & (SP_STATUS_DMA_BUSY | SP_STATUS_DMA_FULL | SP_STATUS_IO_FULL))
		return 1;
	return 0;
}
