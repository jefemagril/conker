#include <os_internal.h>
#include "n_synthInternals.h"


ALFxRef n_alSynGetFXRef(s16 bus) {
    N_ALMainBus *mainBus;

    mainBus = n_syn->mainBus;
    if (mainBus->filter.handler == (N_ALCmdHandler) n_alFxPull) {
        return n_syn->auxBus[bus].sources;
    } else {
        return 0;
    }
}

ALFxRef n_alSynGetOutputLPRef(s16 bus) {
    N_ALMainBus *mainBus;

    mainBus = n_syn->mainBus;
    if (mainBus->filter.handler == (N_ALCmdHandler) n_alFxPull) {
        return n_syn->auxBus[bus].fx_array[7];
    } else {
        return 0;
    }
}
