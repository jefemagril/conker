#include <ultra64.h>
#include "functions.h"
#include "variables.h"


void func_151927C0(s8 *arg0)
{
  s16 new_var;

  new_var = (s16) (((s32) ((*((s32 *) (((s8 *) arg0) + 0x14))) & 0xFF)) >> 1);
  *((s16 *) (((s8 *) arg0) + 0x38)) = 0x12C;
  *((s8 *) (((s8 *) arg0) + 0x3A)) = 0xA;
  *((s32 *) (((s8 *) arg0) + 0x14)) = (s32) ((new_var << 16) + new_var);
  *((s8 *) (((s8 *) arg0) + 0x3B)) = 0;
}

