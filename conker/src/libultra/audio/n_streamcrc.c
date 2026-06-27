#include "n_libaudio.h"

/*
 * CRC-5 over a 16-bit word, processed MSB-first, using polynomial 0x15
 * (x^5 + x^2 + 1). The running remainder is kept in the low 5 bits of `crc`.
 *
 * Split into its own translation unit because the original game compiled this
 * helper at -O1 (instruction scheduling, register-resident locals) while the
 * rest of game_21FC90.c is -g/-O0. The `(u8) bit` cast forces the `andi $a1,
 * 0xFF` byte read of the register-resident bit that -O1 otherwise elides.
 */

#define CRC5_POLYNOMIAL 0x15   /* x^5 + x^2 + 1 (low 5 bits) */
#define CRC5_MSB        0x10   /* top bit of the 5-bit remainder */
#define CRC5_MASK       0x1F   /* keeps only the 5-bit result */
#define WORD_TEST_BIT   0x400  /* input bit consumed each step (bit 10) */
#define WORD_BITS       16     /* shift/feed iterations */

u8 n_alStreamCrc5Word(u16 value) {
    u8 crc;
    u8 feedback;
    s32 i;
    register s32 bit;

    crc = 0;
    for (i = 0; i < WORD_BITS; i++) {
        /* If the top bit is set, it falls out on the shift below, so fold the
         * polynomial back in afterwards. */
        if (crc & CRC5_MSB) {
            feedback = CRC5_POLYNOMIAL;
        } else {
            feedback = 0;
        }
        crc <<= 1;
        if (value & WORD_TEST_BIT) {
            bit = 1;
        } else {
            bit = 0;
        }
        crc |= (u8) bit;
        value <<= 1;
        crc ^= feedback;
    }
    return crc & CRC5_MASK;
}
