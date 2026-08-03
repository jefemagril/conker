#include <ultra64.h>

#include "functions.h"
#include "variables.h"

void func_1508EF80(f32 *arg0, f32 *arg1, f32 arg2, f32 *arg3) {
    f32 sp3C;
    f32 sp38;
    f32 pad0;
    f32 pad1;
    f32 pad2;
    f32 sp28;
    f32 sp24;
    f32 temp_f20;
    f32 temp_f8;
    f32 outz;

    sp3C = arg0[0] - arg1[0];
    sp38 = arg0[2] - arg1[2];
    temp_f20 = arg2 * D_8009DC80;
    sp24 = cosf(temp_f20);
    sp28 = arg1[0] + ((sinf(temp_f20) * sp38) + (sp24 * sp3C));
    sp24 = sinf(temp_f20);
    temp_f8 = cosf(temp_f20) * sp38;
    outz = temp_f8;
    outz += (-sp24 * sp3C);
    outz += arg1[2];
    arg3[0] = sp28;
    arg3[2] = outz;
}
