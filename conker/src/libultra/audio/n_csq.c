#include <n_libaudio.h>

#define CONKER_AL_TRACK_END 0x13

u32 __readVarLen(ALCSeq *seq, s32 track);
u32 __n_alCSeqGetTrackEvent(ALCSeq *seq, u32 track, N_ALEvent *event, s32 arg3);

void n_alCSeqNew(ALCSeq *seq, u8 *ptr) {
    u32 i;
    u32 tmpOff;
    u32 flagTmp;

    seq->base = (ALCMidiHdr *)ptr;
    seq->validTracks = 0;
    seq->lastDeltaTicks = 0;
    seq->lastTicks = 0;
    seq->deltaFlag = 1;

    for (i = 0; i < 16; i++) {
        seq->lastStatus[i] = 0;
        seq->curBUPtr[i] = 0;
        seq->curBULen[i] = 0;
        tmpOff = seq->base->trackOffset[i];

        if (tmpOff != 0) {
            flagTmp = 1 << i;
            seq->validTracks |= flagTmp;
            seq->curLoc[i] = ptr + tmpOff;
            seq->evtDeltaTicks[i] = __readVarLen(seq, i);
        } else {
            seq->curLoc[i] = 0;
        }
    }

    seq->qnpt = 1.0f / (f32)seq->base->division;
}
void n_alCSeqNextEvent(ALCSeq *seq, N_ALEvent *evt, s32 arg2) {
    u32 i;
    u32 firstTime = -1;
    u32 firstTrack = -1;
    u32 lastTicks = seq->lastDeltaTicks;

    for (i = 0; i < 16; i++) {
        if ((seq->validTracks >> i) & 1) {
            if (seq->deltaFlag != 0) {
                seq->evtDeltaTicks[i] -= lastTicks;
            }

            if (seq->evtDeltaTicks[i] < firstTime) {
                firstTime = seq->evtDeltaTicks[i];
                firstTrack = i;
            }
        }
    }

    if (firstTrack != -1) {
        __n_alCSeqGetTrackEvent(seq, firstTrack, evt, arg2);
    } else {
        evt->type = CONKER_AL_TRACK_END;
    }

    evt->msg.midi.ticks = firstTime;
    seq->lastTicks += firstTime;
    seq->lastDeltaTicks = firstTime;

    if (evt->type != CONKER_AL_TRACK_END) {
        seq->evtDeltaTicks[firstTrack] += __readVarLen(seq, firstTrack);
    }

    seq->deltaFlag = 1;
}
#pragma GLOBAL_ASM("asm/nonmatchings/libultra/audio/n_csq/__n_alCSeqGetTrackEvent.s")
#pragma GLOBAL_ASM("asm/nonmatchings/libultra/audio/n_csq/func_100186DC.s")
// void func_100186DC(void *arg0, void *arg1) {
//     s32 sp4;
//     s32 temp_t0;
//     s32 temp_t4;
//     s32 temp_t5;
//     s32 temp_t8;
//
//     arg0->unk4 = (s32) arg1->unk0;
//     arg0->unkC = (s32) arg1->unk4;
//     arg0->unk10 = (s32) arg1->unk8;
//     sp4 = 0;
// loop_1:
//     temp_t0 = sp4 * 4;
//     (arg0 + temp_t0)->unk18 = (s32) (arg1 + temp_t0)->unkC;
//     temp_t5 = sp4 * 4;
//     (arg0 + temp_t5)->unk58 = (s32) (arg1 + temp_t5)->unk4C;
//     (arg0 + sp4)->unk98 = (u8) (arg1 + sp4)->unk8C;
//     (arg0 + sp4)->unkA8 = (u8) (arg1 + sp4)->unk9C;
//     temp_t8 = sp4 * 4;
//     (arg0 + temp_t8)->unkB8 = (s32) (arg1 + temp_t8)->unkAC;
//     temp_t4 = sp4 + 1;
//     sp4 = temp_t4;
//     if (temp_t4 < 0x10) {
//         goto loop_1;
//     }
// }

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/audio/n_csq/func_10018790.s")

u8 __getTrackByte(ALCSeq *seq, s32 track) {
    u8 theByte;

    if (seq->curBULen[track]) {
        theByte = *seq->curBUPtr[track];
        seq->curBUPtr[track]++;
        seq->curBULen[track]--;
    } else {
        theByte = *seq->curLoc[track];
        seq->curLoc[track]++;
        if (theByte == AL_CMIDI_BLOCK_CODE) {
            u8 loBackUp, hiBackUp, theLen, nextByte;
            u32 backup;

            nextByte = *seq->curLoc[track];
            seq->curLoc[track]++;
            if (nextByte != AL_CMIDI_BLOCK_CODE) {
                hiBackUp = nextByte;
                loBackUp = *seq->curLoc[track];
                seq->curLoc[track]++;
                theLen = *seq->curLoc[track];
                seq->curLoc[track]++;
                backup = (u32) hiBackUp;
                backup <<= 8;
                backup += loBackUp;
                seq->curBUPtr[track] = seq->curLoc[track] - (backup + 4);
                seq->curBULen[track] = (u32)theLen;
                theByte = *seq->curBUPtr[track];
                seq->curBUPtr[track]++;
                seq->curBULen[track]--;
            }
        }
    }
    return theByte;
}

u32 __readVarLen(ALCSeq *seq, s32 track) {
    u32 value;
    u32 c;

    value = __getTrackByte(seq, track);
    if (value & 0x80) {
        value &= 0x7F;
        do
        {
            c = __getTrackByte(seq, track);
            value = (value << 7) + (c & 0x7F);
        } while (c & 0x80);
    }
    return value;
}
