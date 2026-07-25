#include <ultra64.h>
#include <os_internal.h>

#include "n_synthInternals.h"
#include "variables.h"

#define gStreamState               D_800E0E04
#define gAudioOutputLeftMixFlags   D_800428C4
#define gAudioOutputRightMixFlags  D_800428C6

s32 n_alStreamMakeSamples(s32 samples, Acmd **cmd);
void n_alFxInitlpfilter_mono(ALLowPass *lp, f32 outputRate);

Acmd *n_alMainBusPull(s32 sampleOffset, Acmd *p)
{
    Acmd *ptr = p;
    s32 i;
    s32 bus;

    if (!gStreamState || !n_alStreamMakeSamples(FIXED_SAMPLE, &ptr)) {
        aClearBuffer(ptr++, N_AL_MAIN_L_OUT, N_AL_DIVIDED << 1);
        aClearBuffer(ptr++, N_AL_AUX_L_OUT, N_AL_DIVIDED << 1);
    }

    /*
     * start from the last bus that still has output filtering enabled
     */
    bus = 0;

    for (i = 1; i < n_syn->maxAuxBusses; i++) {
        if (((ALLowPass *) n_syn->auxBus[i].fx_array[7])->fgain > 0) {
            bus = i;
        }
    }

    for (i = 0; i < n_syn->maxAuxBusses; i++, bus++) {
        if (bus >= n_syn->maxAuxBusses) {
            bus = 0;
        }

        if (i) {
            aClearBuffer(ptr++, N_AL_AUX_L_OUT, N_AL_DIVIDED << 1);
        }

        ptr = (n_syn->mainBus->filter.handler)(sampleOffset, ptr, bus);

        if (gAudioOutputLeftMixFlags[bus]) {
            if (gAudioOutputRightMixFlags[bus]) {
                aMix(ptr++, 0, 0x8000, N_AL_AUX_L_OUT, N_AL_MAIN_L_OUT);
            } else {
                aMix(ptr++, 0, 0x7fff, N_AL_AUX_L_OUT, N_AL_MAIN_R_OUT);
            }
        } else {
            if (gAudioOutputRightMixFlags[bus]) {
                aMix(ptr++, 0, 0x8000, N_AL_AUX_L_OUT, N_AL_MAIN_R_OUT);
            } else {
                aMix(ptr++, 0, 0x7fff, N_AL_AUX_L_OUT, N_AL_MAIN_R_OUT);
            }

            aMix(ptr++, 0, 0x7fff, N_AL_AUX_L_OUT, N_AL_MAIN_L_OUT);
        }

        if (((ALLowPass *) n_syn->auxBus[bus].fx_array[7])->fgain > 0) {
            ALLowPass *lp = (ALLowPass *) n_syn->auxBus[bus].fx_array[7];

            if (lp->first) {
                n_alFxInitlpfilter_mono(lp, n_syn->outputRate);
            }

            n_aLoadADPCM(ptr++, 32, osVirtualToPhysical(lp->fcvec.fccoef));

            n_aPoleFilter(ptr++, 0, N_AL_MAIN_L_OUT, 0, osVirtualToPhysical(lp->fstate[0]) & 0xffffff);
            n_aPoleFilter(ptr++, 0, N_AL_MAIN_R_OUT, 0, osVirtualToPhysical(lp->fstate[1]) & 0xffffff);

            lp->first = 0;
        }
    }

    return ptr;
}
