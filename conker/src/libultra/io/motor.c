#include <os_internal.h>
#include "controller.h"
#include "siint.h"

/*
 * Conker names are swapped vs stock libultra / DKR:
 *   _MakeMotorData == stock osMotorInit (public rumble init; game calls this)
 *   osMotorInit    == stock _MakeMotorData (PIF pack helper)
 *
 * Closest sibling: DKR libultra motor.c (#else / pre-J path):
 *   no __osMotorinitialized gate; datacrc vs 0 / 0xEB; init probes with 0x80 only.
 *
 * Match status (-O2):
 *   osMotorInit     MATCHING (pack helper)
 *   osMotorStop     NON-MATCHING — body matches; prologue jal/sw vs sw/jal/nop;
 *                   epilogue lw ra/v0 order
 *   osMotorStart    NON-MATCHING — same as stop
 *   _MakeMotorData  NON-MATCHING — early s0=channel / sw s0 vs sw a2
 */

/* BSS via undefined_syms / symbol_addrs (not emitted from this TU) */
extern OSPifRam _MotorStopData[MAXCONTROLLERS];  /* D_8003BC30 */
extern OSPifRam _MotorStartData[MAXCONTROLLERS]; /* D_8003BD30 */
extern u8 _motorstopbuf[32];                     /* D_8003BE30 */
extern u8 _motorstartbuf[32];                    /* D_8003BE50 */

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/io/motor/osMotorStop.s")
#pragma GLOBAL_ASM("asm/nonmatchings/libultra/io/motor/osMotorStart.s")
#pragma GLOBAL_ASM("asm/nonmatchings/libultra/io/motor/_MakeMotorData.s")

void osMotorInit(int channel, u16 address, u8 *buffer, OSPifRam *mdata)
{
    u8 *ptr = (u8 *)mdata->ramarray;
    __OSContRamReadFormat ramreadformat;
    int i;

    for (i = 0; i < ARRLEN(mdata->ramarray); i++)
    {
        mdata->ramarray[i] = 0;
    }

    mdata->pifstatus = CONT_CMD_EXE;
    ramreadformat.dummy = CONT_CMD_NOP;
    ramreadformat.txsize = CONT_CMD_WRITE_MEMPACK_TX;
    ramreadformat.rxsize = CONT_CMD_WRITE_MEMPACK_RX;
    ramreadformat.cmd = CONT_CMD_WRITE_MEMPACK;

    ramreadformat.address = (address << 0x5) | __osContAddressCrc(address);
    ramreadformat.datacrc = CONT_CMD_NOP;

    for (i = 0; i < ARRLEN(ramreadformat.data); i++)
    {
        ramreadformat.data[i] = *buffer++;
    }

    if (channel != 0)
    {
        for (i = 0; i < channel; i++)
        {
            *ptr++ = 0;
        }
    }

    *(__OSContRamReadFormat *)ptr = ramreadformat;
    ptr += sizeof(__OSContRamReadFormat);
    ptr[0] = CONT_CMD_END;
}

#if 0
/* NON-MATCHING attempts for the remaining three (DKR-shaped, -O2). */

s32 osMotorStop(OSPfs *pfs)
{
    int i;
    s32 ret;
    u8 *ptr;
    __OSContRamReadFormat ramreadformat;

    __osSiGetAccess();
    __osContLastCmd = CONT_CMD_WRITE_MEMPACK;
    __osSiRawStartDma(OS_WRITE, &_MotorStopData[pfs->channel]);
    osRecvMesg(pfs->queue, NULL, OS_MESG_BLOCK);
    __osSiRawStartDma(OS_READ, &__osPfsPifRam);
    osRecvMesg(pfs->queue, NULL, OS_MESG_BLOCK);
    ptr = (u8 *)&__osPfsPifRam;
    if (pfs->channel != 0)
        for (i = 0; i < pfs->channel; i++)
            ptr++;
    ramreadformat = *(__OSContRamReadFormat *)ptr;
    ret = CHNL_ERR(ramreadformat);
    if (ret == 0 && ramreadformat.datacrc != 0)
        ret = PFS_ERR_CONTRFAIL;
    __osSiRelAccess();
    return ret;
}

s32 osMotorStart(OSPfs *pfs)
{
    int i;
    s32 ret;
    u8 *ptr;
    __OSContRamReadFormat ramreadformat;

    __osSiGetAccess();
    __osContLastCmd = CONT_CMD_WRITE_MEMPACK;
    __osSiRawStartDma(OS_WRITE, &_MotorStartData[pfs->channel]);
    osRecvMesg(pfs->queue, NULL, OS_MESG_BLOCK);
    __osSiRawStartDma(OS_READ, &__osPfsPifRam);
    osRecvMesg(pfs->queue, NULL, OS_MESG_BLOCK);
    ptr = (u8 *)&__osPfsPifRam;
    if (pfs->channel != 0)
        for (i = 0; i < pfs->channel; i++)
            ptr++;
    ramreadformat = *(__OSContRamReadFormat *)ptr;
    ret = CHNL_ERR(ramreadformat);
    if (ret == 0 && ramreadformat.datacrc != 0xEB)
        ret = PFS_ERR_CONTRFAIL;
    __osSiRelAccess();
    return ret;
}

s32 _MakeMotorData(OSMesgQueue *mq, OSPfs *pfs, int channel)
{
    int i;
    s32 ret;
    u8 temp[32];

    pfs->queue = mq;
    pfs->channel = channel;
    pfs->status = 0;
    pfs->activebank = 128;
    for (i = 0; i < ARRLEN(temp); i++)
        temp[i] = 128;
    ret = __osContRamWrite(mq, channel, 1024, temp, FALSE);
    if (ret == PFS_ERR_NEW_PACK)
        ret = __osContRamWrite(mq, channel, 1024, temp, FALSE);
    if (ret != 0)
        return ret;
    ret = __osContRamRead(mq, channel, 1024, temp);
    if (ret != 0)
        return ret;
    if (temp[31] != 0x80)
        return PFS_ERR_DEVICE;
    for (i = 0; i < ARRLEN(_motorstartbuf); i++)
    {
        _motorstartbuf[i] = 1;
        _motorstopbuf[i] = 0;
    }
    osMotorInit(channel, 1536, _motorstartbuf, &_MotorStartData[channel]);
    osMotorInit(channel, 1536, _motorstopbuf, &_MotorStopData[channel]);
    return 0;
}
#endif
