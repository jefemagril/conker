#include "n_libaudio.h"

/*
 * Stream CRC helpers.
 *
 * These two functions form their own translation unit because the original
 * game compiled them at -O1 (instruction scheduling, register-resident locals)
 * while the rest of n_streamplayer.c is -g/-O0. They are contiguous in the ROM
 * (0x151F27E0..0x151F2960); the -O0 stream code resumes at n_alStreamStart.
 */

/*
 * CRC-5 over a 16-bit word, processed MSB-first, using polynomial 0x15
 * (x^5 + x^2 + 1). The running remainder is kept in the low 5 bits of `crc`.
 *
 * The `(u8) bit` cast forces the `andi $a1, 0xFF` byte read of the
 * register-resident bit that -O1 otherwise elides.
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

/*
 * CRC-8 over a 32-byte block, processed MSB-first, using polynomial 0x85
 * (x^8 + x^7 + x^2 + 1). After the 32 data bytes, one final iteration shifts in
 * eight trailing zero bits to flush the remainder.
 */

#define CRC8_POLYNOMIAL 0x85   /* x^8 + x^7 + x^2 + 1 */
#define CRC8_MSB        0x80   /* top bit of the 8-bit remainder */
#define BLOCK_BYTES     32     /* data bytes; iteration BLOCK_BYTES is the flush */

u8 n_alStreamCrc8Block(u8 *data) {
    u8 crc;
    u8 feedback;
    s32 i;
    s32 j;
    register s32 bit;

    crc = 0;
    for (i = 0; i <= BLOCK_BYTES; i++) {
        for (j = 7; j >= 0; j--) {
            if (crc & CRC8_MSB) {
                feedback = CRC8_POLYNOMIAL;
            } else {
                feedback = 0;
            }
            crc <<= 1;
            if (i == BLOCK_BYTES) {
                /* Flush iteration: feed a trailing zero bit. Keep this `| 0`;
                 * folding it into the branch below changes -O1 scheduling. */
                crc |= 0;
            } else {
                if (data[0] & (1 << j)) {
                    bit = 1;
                } else {
                    bit = 0;
                }
                crc |= bit;
            }
            crc ^= feedback;
        }
        data++;
    }
    return crc;
}
