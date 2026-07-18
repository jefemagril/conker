#include <os_internal.h>
#include "n_synthInternals.h"


extern Acmd *(func_1001E530)(s32, Acmd *);

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
