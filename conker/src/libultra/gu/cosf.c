#include <ultra64.h>
#include "variables.h"

#define ROUND(d) ((s32)(((d) >= 0.0f) ? ((d) + 0.5f) : ((d) - 0.5f)))
#define ABS(d) ((d) > 0) ? (d) : -(d)

f32 cosf(f32 x) {
    f32 absx, dx, xsq, poly, dn;
    s32 n, ix, xpt;

    ix = *(s32 *)&x;
    xpt = (ix >> 22) & 0x1ff;

    if (xpt < 0x136) {
        absx = ABS(x);
        dx = absx;
        dn = dx * D_80085EA0 + 0.5f;
        n = ROUND(dn);
        dn = n;
        dn -= 0.5f;
        dx = dx - dn * D_80085EA4;
        dx = dx - dn * D_80085EA8;
        xsq = dx * dx;
        poly = ((D_80098D70[4] * xsq + D_80098D70[3]) * xsq + D_80098D70[2]) * xsq
               + D_80098D70[1];
        if ((n & 1) == 0)
            return dx + (dx * xsq) * poly;
        return -(dx + (dx * xsq) * poly);
    }

    if (x != x)
        return D_8002C920;
    return D_80085EAC;
}
