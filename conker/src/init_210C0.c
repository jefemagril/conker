#include <math.h>
#include "n_synthInternals.h"

extern f32 D_8002C820; /* 26755.0f */

/* n_alEnvmixerPull, still GLOBAL_ASM in init_20000.c */
Acmd *func_10020000(N_PVoice *f, s32 sampleOffset, Acmd *p);

Acmd *n_alAuxBusPull(s32 sampleOffset, Acmd *cmdptr, s32 fxBus, s32 *numpulls)
{
    Acmd            *cmd = cmdptr;
    N_ALAuxBus      *bus = &n_syn->auxBus[fxBus];
    N_PVoice        *pvoice;
    s32             pulls = 0;
    u32             maxLevel = 1;
    u32             gain;

    *numpulls = 0;

    /* the bus list threads &pvoice->voiceLink, so step back to the voice */
    pvoice = (N_PVoice *)((ALLink *)&bus->sourceCount)->next;

    while (pvoice) {
        pvoice = (N_PVoice *)((ALLink *)pvoice - 1);

        if (pvoice->vvoice && pvoice->vvoice->pvoice && pvoice->vvoice->pvoice->unk99 >= 64) {
            cmd = func_10020000(pvoice, sampleOffset, cmd);
            *numpulls = *numpulls + 1;
            pulls++;

            if (pvoice->vvoice->pvoice->unk99 > maxLevel) {
                maxLevel = pvoice->vvoice->pvoice->unk99;
            }
        }

        pvoice = (N_PVoice *)pvoice->voiceLink.next;
    }

    if (pulls) {
        maxLevel -= 62;

        if (maxLevel >= 7) {
            gain = D_8002C820 / sqrtf(maxLevel);
        } else {
            gain = 65536.0f / maxLevel;
        }

        n_aNoop(cmd++, N_AL_MAIN_L_OUT, gain, maxLevel);
        n_aNoop(cmd++, N_AL_MAIN_R_OUT, gain, maxLevel);
    }

    pvoice = (N_PVoice *)((ALLink *)&bus->sourceCount)->next;

    while (pvoice) {
        pvoice = (N_PVoice *)((ALLink *)pvoice - 1);

        if ((pvoice->vvoice && (pvoice->vvoice->pvoice == 0 || pvoice->vvoice->pvoice->unk99 < 64))
                || (pvoice->vvoice == 0 && !fxBus)) {
            Acmd *prev = cmd;
            cmd = func_10020000(pvoice, sampleOffset, cmd);

            if (cmd != prev) {
                *numpulls = *numpulls + 1;
            }
        }

        pvoice = (N_PVoice *)pvoice->voiceLink.next;
    }

    return cmd;
}
