#include <n_libaudio.h>


void n_alCSPSetQueue(N_ALCSPlayer *seqp, s32 queue) {
    seqp->unk84 = queue;
}

void n_alCSPSetChlNoteMesgFlags(N_ALCSPlayer *seqp, s32 chan, u8 flags) {
    seqp->chanState[chan].unk17 = flags;
}

void n_alCSPAllChanOn(N_ALCSPlayer *seqp) {
    s32 chan;

    seqp->chanMask = 0xFFFF;
    for (chan = 0; chan < seqp->maxChannels; chan++)
    {
        seqp->chanState[chan].unkE = 0xFF; /* fadevoltarget */
        seqp->chanState[chan].unkD = 0xFF; /* fadevolcurrent */
    }
}

void n_alCSPChanOff(N_ALCSPlayer *seqp, s32 chan) {
    n_alCSPSendMidi(seqp, 0, chan | 0xB0, 252, 0);
}

void n_alCSPChanOn(N_ALCSPlayer *seqp, s32 chan) {
    seqp->chanMask |= 1 << chan;
    n_alCSPSendMidi(seqp, 0, chan | 0xB0, 0xFC, 0xFF);
}

void n_alCSPChanFade(N_ALCSPlayer *seqp, s32 chan, u8 targetvol, u8 incvol) {
    n_alCSPSendMidi(seqp, 0, chan | 0xB0, 0xFD, incvol);
    n_alCSPSendMidi(seqp, 0, chan | 0xB0, 0xFF, targetvol);
}

void n_alCSPChanSurround(N_ALCSPlayer *seqp, s32 chan, u8 fxmix80) {
    n_alCSPSendMidi(seqp, 0, chan | 0xB0, 0x41, fxmix80);
}

void n_alCSPChanFadeForce(N_ALCSPlayer *seqp, s32 chan, u8 vol) {
    n_alCSPSendMidi(seqp, 0, chan | 0xB0, 0xFC, vol);
}
