#include <os_internal.h>
#include <ultraerror.h>
#include "n_synthInternals.h"


/* Conker Filter12 (type 18); was misnamed n_alSynSetPan. */
void n_alSynFilter12(N_ALVoice *v, u8 filter12)
{
    ALParam  *update;

    if (v->pvoice) {

        /*
         * get new update struct from the free list
         */
        update = __n_allocParam();
        ALFailIf(update == 0, ERR_ALSYN_NO_UPDATE);

        /*
         * set offset and pan data
         */
#ifdef SAMPLE_ROUND
	update->delta  = SAMPLE184( n_syn->paramSamples + v->pvoice->offset);
#else
        update->delta  = n_syn->paramSamples + v->pvoice->offset;
#endif
        update->type   = 18; /* custom Filter12 */
        update->data.i = filter12;
        update->next   = 0;

	n_alEnvmixerParam(v->pvoice, AL_FILTER_ADD_UPDATE, update);
    }
}
