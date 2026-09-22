#pragma once
#include <stdint.h>

#define F 0xF

typedef struct
{
    uint8_t memory[4096];
    uint8_t V[16];
    uint16_t I;
    uint16_t pc;
    uint16_t stack[16];
    uint8_t sp;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t gfx[64 * 32];
    uint8_t key[16];
} Chip8;

extern uint8_t fontset[80];

void emulate_cycle(Chip8 *chip8);