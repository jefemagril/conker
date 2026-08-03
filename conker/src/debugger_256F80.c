#include <ultra64.h>

#include "functions.h"
#include "variables.h"
#include "controller.h"

// NON-MATCHING: JUSTREG 74/76 — tip justreg_park; only fill-loop addiu lo16 order (v0/a0) vs target
// s32 func_16001700(void) {
//     s32 sp24;
//     u32 *var_v0;
//     u32 *a0;
//     s32 var_s0;
//     s32 var_s0_2;
//     u32 temp_s1;
//     u32 temp_s1_2;
//
//     if (__osContLastCmd != CONT_CMD_READ_BUTTON) {
//         func_160018BC();
//         func_160019A8(OS_WRITE, &__osContPifRam);
//         var_s0 = 0;
//         temp_s1 = osGetCount() + 0x30D40;
//         if (osGetCount() < temp_s1) {
//             do {
//                 var_s0 = func_160016F4(var_s0);
//             } while (osGetCount() < temp_s1);
//         }
//         func_160016F4(var_s0);
//     }
//     var_v0 = (u32 *)&__osContPifRam;
//     a0 = (u32 *)&__osContLastCmd;
//     do {
//         var_v0++;
//         *(var_v0 - 1) = 0xFF;
//     } while (var_v0 < a0);
//     D_80042A4C = 0; /* __osContPifRam.pifstatus */
//     sp24 = func_160019A8(OS_READ, &__osContPifRam);
//     __osContLastCmd = CONT_CMD_READ_BUTTON;
//     var_s0_2 = 0;
//     temp_s1_2 = osGetCount() + 0xC3500;
//     if (osGetCount() < temp_s1_2) {
//         do {
//             var_s0_2 = func_160016F4(var_s0_2);
//         } while (osGetCount() < temp_s1_2);
//     }
//     func_160016F4(var_s0_2);
//     return sp24;
// }
#pragma GLOBAL_ASM("asm/nonmatchings/debugger_256F80/func_16001700.s")

void func_16001830(OSContPad *data) {
    u8 *ptr;
    __OSContReadFormat readformat;
    s32 i;

    ptr = (u8 *)&__osContPifRam.ramarray;
    for (i = 0; i < __osMaxControllers; i++, ptr += sizeof(__OSContReadFormat), data++) {
        readformat = *(__OSContReadFormat *)ptr;
        data->errno = CHNL_ERR(readformat);
        if (data->errno == 0) {
            data->button = readformat.button;
            data->stick_x = readformat.stick_x;
            data->stick_y = readformat.stick_y;
        }
    }
}

void func_160018BC(void) {
    u8 *ptr;
    __OSContReadFormat readformat;
    s32 i;

    ptr = (u8 *)&__osContPifRam.ramarray;
    for (i = 0; i < ARRLEN(__osContPifRam.ramarray) + 1; i++) {
        __osContPifRam.ramarray[i] = 0;
    }
    __osContPifRam.pifstatus = CONT_CMD_EXE;
    readformat.dummy = CONT_CMD_NOP;
    readformat.txsize = CONT_CMD_READ_BUTTON_TX;
    readformat.rxsize = CONT_CMD_READ_BUTTON_RX;
    readformat.cmd = CONT_CMD_READ_BUTTON;
    readformat.button = 0xFFFF;
    readformat.stick_x = -1;
    readformat.stick_y = -1;
    for (i = 0; i < __osMaxControllers; i++) {
        *(__OSContReadFormat *)ptr = readformat;
        ptr += sizeof(__OSContReadFormat);
    }
    *ptr = CONT_CMD_END;
}

// another __osSiDeviceBusy function
s32 func_16001984()
{
    register u32 stat = IO_READ(SI_STATUS_REG);
    if (stat & (SI_STATUS_DMA_BUSY | SI_STATUS_RD_BUSY))
        return 1;
    return 0;
}

// very similar to __osSiRawStartDma
s32 func_160019A8(s32 direction, void *dramAddr) {
    if ((s32)dramAddr & 3) { // what is this checking?
        return -1;
    }
    if (func_16001984()) {
        return -1;
    }

    if (direction == OS_WRITE) {
        osWritebackDCache(dramAddr, 64);
    }

    IO_WRITE(SI_DRAM_ADDR_REG, osVirtualToPhysical(dramAddr));

    if (direction == OS_READ) {
        IO_WRITE(SI_PIF_ADDR_RD64B_REG, 0x1FC007C0);
    } else {
        IO_WRITE(SI_PIF_ADDR_WR64B_REG, 0x1FC007C0);
    }
    if (direction == OS_READ) {
        osInvalDCache(dramAddr, 64);
    }

    return 0;
}

void func_16001A64(void) {
}

s32 func_16001A6C(f32 arg0) {
    s32 tmp = *(s32*) &arg0;

    if ((tmp * 2) == 0) {
        return 0;
    }
    tmp = (tmp & 0x7F800000) >> 0x17;
    if ((tmp <= 0) || (tmp >= 255)) {
        return 1;
    }
    return 0;
}
