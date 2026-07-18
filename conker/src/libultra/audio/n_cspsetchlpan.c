#include <libaudio.h>
#include "n_libaudio.h"

void n_alCSPSetChlPan(N_ALCSPlayer *seqp, u8 chan, ALPan pan)
{
    N_ALEvent evt;

    evt.type            = AL_SEQP_MIDI_EVT;
    evt.msg.midi.ticks  = 0;
    evt.msg.midi.status = AL_MIDI_ControlChange | chan;
    evt.msg.midi.byte1  = AL_MIDI_PAN_CTRL;
    evt.msg.midi.byte2  = pan;

    n_alEvtqPostEvent(&seqp->evtq, &evt, 0, 2);
}
