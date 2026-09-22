#include "emu_cycle.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void emulate_cycle(Chip8 *chip8)
{
    uint16_t opcode = (chip8->memory[chip8->pc & 0x0FFF] << 8) | chip8->memory[(chip8->pc + 1) & 0x0FFF];
    chip8->pc += 2;

    switch (opcode & 0xF000)
    {
    case 0x0000:
        if (opcode == 0x00E0)
        {
            memset(chip8->gfx, 0, sizeof(chip8->gfx));
        }
        if (opcode == 0x00EE)
        {
            if (chip8->sp > 0)
            {
                chip8->sp--;
                chip8->pc = chip8->stack[chip8->sp];
            }
        }
        break;

    case 0x1000:
    {

        uint16_t nnn = (opcode & 0x0FFF);
        chip8->pc = nnn;
    }
    break;

    case 0x2000:
    {

        uint16_t nnn = (opcode & 0x0FFF);

        if (chip8->sp < 16)
        {
            chip8->stack[chip8->sp] = chip8->pc;
            chip8->sp++;
        }

        chip8->pc = nnn;
    }
    break;

    case 0x3000:
    {
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t nn = (opcode & 0x00FF);

        if (chip8->V[x] == nn)
        {
            chip8->pc += 2;
        }
    }
    break;

    case 0x4000:
        // Hier landen alle Befehle vom Typ 4XNN (z.B. 0x4005, 0x41FF etc.)
        {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t nn = (opcode & 0x00FF);

            if (chip8->V[x] != nn)
            {
                chip8->pc += 2;
            }
        }
        break;

    case 0x5000:
    {
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t y = (opcode & 0x00F0) >> 4;

        if (chip8->V[x] == chip8->V[y])
        {
            chip8->pc += 2;
        }
    }
    break;

    case 0x6000:
    {
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t nn = (opcode & 0x00FF);

        chip8->V[x] = nn;
    }
    break;

    case 0x7000:
    {
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t nn = (opcode & 0x00FF);

        chip8->V[x] += nn;
    }
    break;

    case 0x8000:
    {
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t y = (opcode & 0x00F0) >> 4;
        uint8_t lastDig = (opcode & 0x000F);

        if (lastDig == 0)
        {
            chip8->V[x] = chip8->V[y];
        }
        if (lastDig == 1)
        {
            chip8->V[x] = chip8->V[x] | chip8->V[y];
        }
        if (lastDig == 2)
        {
            chip8->V[x] = chip8->V[x] & chip8->V[y];
        }
        if (lastDig == 3)
        {
            chip8->V[x] = chip8->V[x] ^ chip8->V[y];
        }
        if (lastDig == 4)
        {
            uint16_t sum = chip8->V[x] + chip8->V[y];

            // Erst Ergebnis schreiben, dann Flag (falls x == F)
            chip8->V[x] = sum & 0xFF;
            chip8->V[F] = (sum > 255) ? 1 : 0;
        }
        if (lastDig == 5)
        {
            uint8_t flag = (chip8->V[x] >= chip8->V[y]) ? 1 : 0;

            chip8->V[x] = chip8->V[x] - chip8->V[y];
            chip8->V[F] = flag;
        }
        if (lastDig == 6)
        {
            uint8_t flag = chip8->V[x] & 1;

            chip8->V[x] /= 2;
            chip8->V[F] = flag;
        }
        if (lastDig == 7)
        {
            uint8_t flag = (chip8->V[y] >= chip8->V[x]) ? 1 : 0;

            chip8->V[x] = chip8->V[y] - chip8->V[x];
            chip8->V[F] = flag;
        }
        // 0001 0000 = 16
        // << 1
        // 0010 0000 = 32
        // 0x80 => 8 in hex ist bin : 1000
        //  0 => 0 in hex ist bin : 0000
        // zsm 0x80 in bin = 1000 0000
        // also ganz links das msb
        if (lastDig == 0xE)
        {
            uint8_t flag = chip8->V[x] >> 7;

            chip8->V[x] <<= 1;
            // chip8->V[x] = chip8->V[x] * 2; ist dasselbe
            chip8->V[F] = flag;
        }
    }
    break;

    case 0x9000:
    {
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t y = (opcode & 0x00F0) >> 4;

        if (chip8->V[x] != chip8->V[y])
        {
            chip8->pc += 2;
        }
    }
    break;

    case 0xA000:
    {
        uint16_t nnn = (opcode & 0x0FFF);
        chip8->I = nnn;
    }
    break;

    case 0xB000:
    {
        uint16_t nnn = (opcode & 0x0FFF);
        chip8->pc = nnn + chip8->V[0];
    }
    break;

    case 0xC000:
    {
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t nn = (opcode & 0x00FF);

        uint8_t randomNum = rand() % 256;
        chip8->V[x] = randomNum & nn;
    }
    break;

    case 0xD000:
    {
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t y = (opcode & 0x00F0) >> 4;
        uint8_t n = (opcode & 0x000F);

        uint8_t xCoord = chip8->V[x] % 64;
        uint8_t yCoord = chip8->V[y] % 32;

        chip8->V[F] = 0;

        for (int row = 0; row < n; row++)
        {
            uint8_t spriteByte = chip8->memory[chip8->I + row];

            for (int col = 0; col < 8; col++)
            {
                if ((spriteByte << col) & 0x80)
                {
                    uint32_t screenX = (xCoord + col) % 64;
                    uint32_t screenY = (yCoord + row) % 32;

                    uint32_t index = (screenY * 64) + screenX;

                    if (chip8->gfx[index] == 1)
                    {
                        chip8->V[F] = 1;
                    }

                    chip8->gfx[index] ^= 1;
                }
            }
        }
    }
    break;

    case 0xE000:
    {
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t lastBytes = opcode & 0x00FF;

        if (lastBytes == 0x9E)
        {
            if (chip8->key[chip8->V[x] & 0xF])
            {
                chip8->pc += 2;
            }
        }

        if (lastBytes == 0xA1)
        {
            if (!(chip8->key[chip8->V[x] & 0xF]))
            {
                chip8->pc += 2;
            }
        }
    }
    break;

    case 0xF000:
    {
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t lastDigits = (opcode & 0x00FF);

        if (lastDigits == 0x07)
        {
            chip8->V[x] = chip8->delay_timer;
        }

        if (lastDigits == 0x0A)
        {
            int keyPressed = 0;

            for (int i = 0; i < 16; i++)
            {
                if (chip8->key[i])
                {
                    chip8->V[x] = i;
                    keyPressed = 1;
                    break;
                }
            }

            if (!keyPressed)
            {
                chip8->pc -= 2;
            }
        }

        if (lastDigits == 0x15)
        {
            chip8->delay_timer = chip8->V[x];
        }

        if (lastDigits == 0x18)
        {
            chip8->sound_timer = chip8->V[x];
        }

        if (lastDigits == 0x1E)
        {
            chip8->I += chip8->V[x];
            // chip8->I = chip8->I + chip8->V[x];
        }

        if (lastDigits == 0x29)
        {
            chip8->I = 0x050 + ((chip8->V[x] & 0xF) * 5);
        }

        if (lastDigits == 0x33)
        {
            uint8_t value = chip8->V[x];

            uint8_t hundreds = value / 100;
            uint8_t tens = (value / 10) % 10;
            uint8_t ones = value % 10;

            chip8->memory[chip8->I & 0x0FFF] = hundreds;
            chip8->memory[(chip8->I + 1) & 0x0FFF] = tens;
            chip8->memory[(chip8->I + 2) & 0x0FFF] = ones;
        }
        if (lastDigits == 0x55)
        {
            for (int i = 0; i <= x; i++)
            {
                chip8->memory[(chip8->I + i) & 0x0FFF] = chip8->V[i];
            }
        }
        if (lastDigits == 0x65)
        {
            for (int i = 0; i <= x; i++)
            {
                chip8->V[i] = chip8->memory[(chip8->I + i) & 0x0FFF];
            }
        }
    }
    break;

    default:
        printf("Unbekannter Opcode: 0x%04X\n", opcode);
        break;
    }
}