#include <ultra64.h>

#include "n_synthInternals.h"
#include "variables.h"

ALFxRef n_alSynAllocFX(s16 bus, ALSynConfig *c, ALHeap *hp)
{
    n_alFxNew(&D_8002BA44->unk48[bus].unk20, c, bus, hp);
    return (ALFxRef) D_8002BA44->unk48[bus].unk20;
}
