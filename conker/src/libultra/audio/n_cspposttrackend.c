#include <libaudio.h>
#include "n_libaudio.h"
#include "n_seqp.h"


/* Posts AL_TRACK_END — Conker begin-stop (PD STOPPING slot). */
void n_alCSPPostTrackEnd(N_ALCSPlayer *seqp) {
    N_ALEvent evt;

    evt.type = AL_TRACK_END;
    n_alEvtqPostEvent(&seqp->evtq, &evt, 0, 2);
}
