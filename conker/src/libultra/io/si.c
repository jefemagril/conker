#include <os_internal.h>
#include "siint.h"

int __osSiDeviceBusy(void)
{
	register s32 status = IO_READ(SI_STATUS_REG);
	if (status & (SI_STATUS_DMA_BUSY | SI_STATUS_RD_BUSY))
		return 1;
	return 0;
}
