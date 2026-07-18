#include <os_internal.h>
#include "n_synthInternals.h"


void n_alFxInitlpfilter_mono(ALLowPass *, f32);
void func_1001ED6C(ALFxRef fx, s16 paramID, void *param);

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
