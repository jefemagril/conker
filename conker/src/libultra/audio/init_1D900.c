#include <os_internal.h>
#include <ultraerror.h>
#include "n_synthInternals.h"


void n_alFxInitlpfilter_mono(ALLowPass *, f32);
extern Acmd *(func_1001E530)(s32, Acmd *);
void func_1001ED6C(ALFxRef fx, s16 paramID, void *param);


void n_alSynSetFXMix( N_ALVoice *v, u8 fxmix) {
    ALParam  *update;

    if (v->pvoice) {
        update = __n_allocParam();
        ALFailIf(update == 0, ERR_ALSYN_NO_UPDATE);

        update->delta  = n_syn->paramSamples + v->pvoice->offset;
        update->type   = AL_FILTER_SET_FXAMT;
        update->data.i = fxmix;
        update->next   = 0;
        n_alEnvmixerParam(v->pvoice, AL_FILTER_ADD_UPDATE, update);
    }
}

ALFxRef n_alSynGetFXRef(s16 bus) {
    N_ALMainBus *mainBus;

    mainBus = n_syn->mainBus;
    if (mainBus->filter.handler == func_1001E530) {
        return n_syn->auxBus[bus].sources;
    } else {
        return 0;
    }
}

ALFxRef n_alSynGetOutputLPRef(s16 bus) {
    N_ALMainBus *mainBus;

    mainBus = n_syn->mainBus;
    if (mainBus->filter.handler == func_1001E530) {
        return n_syn->auxBus[bus].fx_array[7];
    } else {
        return 0;
    }
}

void n_alSynSetFXParam(ALFxRef fx, s16 paramID, void *param) {
    ALFxRef fxRef = fx;
    func_1001ED6C(fxRef, paramID, param);
}

void n_alSynSetOutputLPParam(ALLowPass *lp, s16 paramID, s32 *param) {
    if (paramID == 8) {
        lp->fgain = (f32) *param * 0.1f;
    } else if (paramID == 9) {
        lp->fc = *param;
    }
    n_alFxInitlpfilter_mono(lp, n_syn->outputRate);
}
