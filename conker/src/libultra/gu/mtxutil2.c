#include <ultra64.h>

void guMtxF2L2(f32 mf[4][4], Mtx *m) {
    s32 i, j;
    s32 e1, e2;
    s32 *ai, *af;

    ai = (s32 *)&m->m[0][0];
    af = (s32 *)&m->m[2][0];

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 2; j++) {
            e1 = mf[i][j * 2] * 65536.0f;
            e2 = mf[i][j * 2 + 1] * 65536.0f;
            *(ai++) = (e1 & 0xffff0000) | ((e2 >> 16) & 0xffff);
            *(af++) = ((e1 << 16) & 0xffff0000) | (e2 & 0xffff);
        }
    }
}

void guMtxIdentF2(f32 mf[4][4]) {
    s32 i, j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            mf[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

void guMtxIdent(Mtx *m) {
    f32 mf[4][4];

    guMtxIdentF2(mf);
    guMtxF2L2(mf, m);
}

void guMtxL2F(f32 mf[4][4], Mtx *m) {
    s32 i, j;
    u32 e1, e2;
    u32 *ai, *af;
    s32 q1, q2;

    ai = (u32 *)&m->m[0][0];
    af = (u32 *)&m->m[2][0];

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 2; j++) {
            e1 = (*ai & 0xffff0000) | ((*af >> 16) & 0xffff);
            e2 = ((*(ai++) << 16) & 0xffff0000) | (*(af++) & 0xffff);
            q1 = *((s32 *)&e1);
            q2 = *((s32 *)&e2);

            mf[i][j * 2] = q1 / 65536.0f;
            mf[i][j * 2 + 1] = q2 / 65536.0f;
        }
    }
}
