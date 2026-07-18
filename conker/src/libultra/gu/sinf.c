#include <ultra64.h>

#include "variables.h"

#define ROUND(d) ((s32)(((d) >= 0.0f) ? ((d) + 0.5f) : ((d) - 0.5f)))

f32 sinf(f32 x) {
    f32 dx;
    f32 xsq;
    f32 poly;
    f32 dn;
    s32 n;
    s32 ix;
    s32 xpt;

    ix = *(s32 *)&x;
    xpt = (ix >> 22) & 0x1ff;

    if (xpt < 0xff) {
        dx = x;
        if (xpt >= 0xe6) {
            xsq = dx * dx;
            poly = ((D_80098D90[4] * xsq + D_80098D90[3]) * xsq + D_80098D90[2]) * xsq + D_80098D90[1];
            return dx + (dx * xsq) * poly;
        }
        return x;
    }

    if (xpt < 0x136) {
        dx = x;
        dn = dx * D_80085EB0;
        n = ROUND(dn);
        dn = n;
        dx = dx - dn * D_80085EB4;
        dx = dx - dn * D_80085EB8;
        xsq = dx * dx;
        poly = ((D_80098D90[4] * xsq + D_80098D90[3]) * xsq + D_80098D90[2]) * xsq + D_80098D90[1];
        if ((n & 1) == 0) {
            return dx + (dx * xsq) * poly;
        }
        return -(dx + (dx * xsq) * poly);
    }

    if (x != x) {
        return D_8002C920;
    }
    return D_80085EBC;
}
