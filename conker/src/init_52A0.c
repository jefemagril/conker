#include <PR/sched.h>

#include "functions.h"
#include "variables.h"

/*
 * PRE-NMI reset thread (PD resetproc).
 * NON-MATCHING: D_8003BC20 address CSE — IDO keeps &D_8003BC20 in $s7 for the
 * u64 store and later waits; original uses $at for the store (%lo BC20/BC24) and
 * fresh lui pairs per wait load of D_8003BC24 then D_8003BC20. Volatile / BC24-4
 * / same-TU define / split-half shapes all still pull a saved reg. Quit.
 *
 * void func_100052A0(s32 arg0) {
 *     OSMesg msg = NULL;
 *     s32 i;
 *
 *     if (D_8002BD18 == 0) {
 *         osRecvMesg(&D_8003B9D0, &msg, OS_MESG_BLOCK);
 *     }
 *     D_8002AC5C = 1;
 *     osStopThread(&piThread);
 *     osStopThread(&D_80031AE0);
 *     func_100093CC();
 *     D_8003BC20 = osGetTime();
 *     __osViInit();
 *     D_8002AAE0 = 1;
 *     osSetThreadPri(0, 0xB);
 *     if (D_8002AAE4 != 0) {
 *         if (D_80084064 == 0) {
 *             osRecvMesg(&D_800BE900, &D_800BE990, OS_MESG_BLOCK);
 *         }
 *         for (i = 0; i < 4; i++) {
 *             if (D_800BE944[i] != 0) {
 *                 _MakeMotorData(&D_800BE900, (OSPfs *)&D_800BE760[i], i);
 *                 osMotorStop((OSPfs *)&D_800BE760[i]);
 *                 D_800BE948[i] = 0;
 *             }
 *         }
 *     }
 *     if (osGetTime() < D_8003BC20 + 0x22ADD7) {
 *         do {} while (osGetTime() < D_8003BC20 + 0x22ADD7);
 *     }
 *     if (osGetTime() < D_8003BC20 + 0x7270E0) {
 *         do {} while (osGetTime() < D_8003BC20 + 0x7270E0);
 *     }
 *     osWritebackDCacheAll();
 *     while (1) {}
 * }
 */
#pragma GLOBAL_ASM("asm/nonmatchings/init_52A0/func_100052A0.s")
