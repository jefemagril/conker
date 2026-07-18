#include <n_libaudio.h>


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
