#include <ultra64.h>

#include "functions.h"
#include "variables.h"


// whats wrong with bcopy?
u8* func_16001AD0(u8 *arg0, u8 *arg1, u32 arg2) {
    u8 *tmp0 = arg0;
    u8 *tmp1 = arg1;

    while (arg2 > 0) {
        *tmp0++ = *tmp1++;
        arg2 -= 1;
    }

    return arg0;
}

s32 func_16001B00(u8 *arg0) {
    s32 var_v1;
    u8 *var_v0;
    u8 temp_t7;

    var_v0 = arg0;
    var_v1 = 0;
    if (*arg0 != 0) {
        do {
            temp_t7 = *(var_v0 + 1) ^ 0;
            var_v1 += 1;
            var_v0 += 1;
        } while (temp_t7 != 0);
    }
    return var_v1;
}

s32 func_16001BB4(s32 (*arg0)(u8 *, u8 *, u32), u8 *arg1, u8 *arg2, char *arg3);

s32 func_16001B34(u8 *dst, u8 *fmt, ...) {
    s32 ans;
    char *ap;

    ap = (char *)&fmt + sizeof(fmt);
    ans = func_16001BB4(func_16001B8C, dst, fmt, ap);
    if (ans >= 0) {
        dst[ans] = 0;
    }
    return ans;
}

s32 func_16001B8C(u8 *arg0, u8 *arg1, u32 arg2) {
    return func_16001AD0(arg0, arg1, arg2) + arg2;
}

#pragma GLOBAL_ASM("asm/nonmatchings/debugger_257350/func_16001BB4.s")
// uses jump table
#pragma GLOBAL_ASM("asm/nonmatchings/debugger_257350/func_160021FC.s")
// contains delay slot
#pragma GLOBAL_ASM("asm/nonmatchings/debugger_257350/func_1600288C.s")
#pragma GLOBAL_ASM("asm/nonmatchings/debugger_257350/func_16002D2C.s")
#pragma GLOBAL_ASM("asm/nonmatchings/debugger_257350/func_16002DE4.s")
#pragma GLOBAL_ASM("asm/nonmatchings/debugger_257350/func_160033A8.s")
