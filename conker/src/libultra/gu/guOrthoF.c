#include <ultra64.h>

void guOrthoF(f32 mf[4][4], f32 l, f32 r, f32 b, f32 t, f32 n, f32 f, f32 scale) {
    s32 i, j;

    guMtxIdentF2(mf);

    mf[0][0] = 2.0f / (r - l);
    mf[1][1] = 2.0f / (t - b);
    mf[2][2] = -2.0f / (f - n);
    mf[3][0] = -(r + l) / (r - l);
    mf[3][1] = -(t + b) / (t - b);
    mf[3][2] = -(f + n) / (f - n);
    mf[3][3] = 1.0f;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            mf[i][j] *= scale;
        }
    }
}

void guOrtho(Mtx *m, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f, f32 scale) {
    f32 mf[4][4];

    guOrthoF(mf, l, r, b, t, n, f, scale);
    guMtxF2L2(mf, m);
}
