#include <n_libaudio.h>


void n_alCSPSetFxParam(N_ALCSPlayer *seqp, u8 arg1, u8 arg2, u8 arg3, s32 arg4) {

    N_ALEvent evt;
    evt.type = 0x1A; /* Conker FXPARAM event (PD uses 0x19) */
    evt.msg.fxparam.bus = arg1;
    evt.msg.fxparam.paramId = arg2;
    evt.msg.fxparam.paramSub = arg3;
    evt.msg.fxparam.param = arg4;

    n_alEvtqPostEvent(&seqp->evtq, &evt, 0, 2);
}
