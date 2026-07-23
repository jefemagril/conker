#include <PR/sched.h>

#include "functions.h"
#include "variables.h"

/* PRE-NMI reset thread (PD resetproc). C match needs non-CSE access to D_8003BC20. */
#pragma GLOBAL_ASM("asm/nonmatchings/init_52A0/func_100052A0.s")
