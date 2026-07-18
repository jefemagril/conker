#include <n_libaudio.h>


void n_alCSPSetFxMix(N_ALCSPlayer *csp, f32 fxmixmajor, f32 fxmixmega) {
    N_ALEvent event;

    event.type = 25; /* Conker FXMIX event (PD uses 0x18) */
    event.msg.unknown0.unk0 = fxmixmajor;
    event.msg.unknown0.unk4 = fxmixmega;

    n_alEvtqPostEvent(&csp->evtq, &event, 0, 2);
}

void n_alCSPSetChlFxBus(N_ALCSPlayer *csp, u8 chan, u8 fxbus) {
    N_ALEvent event;

    event.type = 2; /* AL_SEQP_MIDI_EVT */
    event.msg.midi.ticks = 0;
    event.msg.midi.status = chan | 0xB0;
    event.msg.midi.byte1 = 92; /* AL_MIDI_FXBUS_CTRL */
    event.msg.midi.byte2 = fxbus;

    n_alEvtqPostEvent(&csp->evtq, &event, 0, 2);
}
