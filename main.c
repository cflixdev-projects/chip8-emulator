
// #define F 0xF

// typedef struct
// {
//     uint8_t memory[4096]; // 4 KB Gesamtspeicher
//     uint8_t V[16];        // 16 8-Bit-Register (V0 bis VF)
//     uint16_t I;           // 16-Bit Index-Register
//     uint16_t pc;          // 16-Bit Program Counter
//     uint16_t stack[16];   // 16-stufiger Stack für Rücksprungadressen
//     uint8_t sp;           // Stack Pointer (zeigt auf das aktuelle Stack-Level)
//     uint8_t delay_timer;  // 60Hz Delay-Timer
//     uint8_t sound_timer;  // 60Hz Sound-Timer
//     uint8_t gfx[64 * 32]; // Display-Puffer (64x32 Pixel, 1 Byte pro Pixel für den Anfang)
//     uint8_t key[16];      // Tastatur-Zustand (0 bis F)
// } Chip8;

// uint8_t fontset[80] = {
//     0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
//     0x20, 0x60, 0x20, 0x20, 0x70, // 1
//     0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
//     0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
//     0x90, 0x90, 0xF0, 0x10, 0x10, // 4
//     0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
//     0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
//     0xF0, 0x10, 0x20, 0x40, 0x40, // 7
//     0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
//     0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
//     0xF0, 0x90, 0xF0, 0x90, 0x90, // A
//     0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
//     0xF0, 0x80, 0x80, 0x80, 0xF0, // C
//     0xE0, 0x90, 0x90, 0x90, 0xE0, // D
//     0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
//     0xF0, 0x80, 0xF0, 0x80, 0x80  // F
// };

// void initialize(Chip8 *chip8)
// {
//     memset(chip8->memory, 0, sizeof(chip8->memory));
//     memset(chip8->gfx, 0, sizeof(chip8->gfx));
//     memset(chip8->key, 0, sizeof(chip8->key));

//     chip8->pc = 0x200; // startadresse für roms 512
//     // pc auf wert 512 gesetzt
//     chip8->sp = 0;
//     chip8->I = 0;
//     chip8->delay_timer = 0;
//     chip8->sound_timer = 0;

//     for (int i = 0; i < sizeof(fontset); i++)
//     {
//         chip8->memory[0x050 + i] = fontset[i];
//     }
// }

// bool load_rom(Chip8 *chip8, const char *filename)
// {
//     FILE *data = fopen(filename, "rb");
//     if (data == NULL)
//     {
//         printf("Fehler beim öffnen der Datei!\n");
//         return false;
//     }

//     fseek(data, 0, SEEK_END);
//     long size = ftell(data);
//     rewind(data);

//     if (size > (4096 - 0x200))
//     {
//         printf("Rom zu groß!\n");
//         fclose(data);
//         return false;
//     }

//     size_t bytes_read = fread(&chip8->memory[0x200], size, 1, data);

//     fclose(data);
//     printf("ROM erfolgreich geladen! Bytes: %ld\n", size); // Hier eine Ausgabe einbauen!

//     return true;
// }

// void emulate_cycle(Chip8 *chip8)
// {
//     uint16_t opcode = (chip8->memory[chip8->pc & 0x0FFF] << 8) | chip8->memory[(chip8->pc + 1) & 0x0FFF];
//     chip8->pc += 2;

//     switch (opcode & 0xF000)
//     {
//     case 0x0000:
//         if (opcode == 0x00E0)
//         {
//             memset(chip8->gfx, 0, sizeof(chip8->gfx));
//         }
//         if (opcode == 0x00EE)
//         {
//             if (chip8->sp > 0)
//             {
//                 chip8->sp--;
//                 chip8->pc = chip8->stack[chip8->sp];
//             }
//         }
//         break;

//     case 0x1000:
//     {

//         uint16_t nnn = (opcode & 0x0FFF);
//         chip8->pc = nnn;
//     }
//     break;

//     case 0x2000:
//     {

//         uint16_t nnn = (opcode & 0x0FFF);

//         if (chip8->sp < 16)
//         {
//             chip8->stack[chip8->sp] = chip8->pc;
//             chip8->sp++;
//         }

//         chip8->pc = nnn;
//     }
//     break;

//     case 0x3000:
//     {
//         uint8_t x = (opcode & 0x0F00) >> 8;
//         uint8_t nn = (opcode & 0x00FF);

//         if (chip8->V[x] == nn)
//         {
//             chip8->pc += 2;
//         }
//     }
//     break;

//     case 0x4000:
//         // Hier landen alle Befehle vom Typ 4XNN (z.B. 0x4005, 0x41FF etc.)
//         {
//             uint8_t x = (opcode & 0x0F00) >> 8;
//             uint8_t nn = (opcode & 0x00FF);

//             if (chip8->V[x] != nn)
//             {
//                 chip8->pc += 2;
//             }
//         }
//         break;

//     case 0x5000:
//     {
//         uint8_t x = (opcode & 0x0F00) >> 8;
//         uint8_t y = (opcode & 0x00F0) >> 4;

//         if (chip8->V[x] == chip8->V[y])
//         {
//             chip8->pc += 2;
//         }
//     }
//     break;

//     case 0x6000:
//     {
//         uint8_t x = (opcode & 0x0F00) >> 8;
//         uint8_t nn = (opcode & 0x00FF);

//         chip8->V[x] = nn;
//     }
//     break;

//     case 0x7000:
//     {
//         uint8_t x = (opcode & 0x0F00) >> 8;
//         uint8_t nn = (opcode & 0x00FF);

//         chip8->V[x] += nn;
//     }
//     break;

//     case 0x8000:
//     {
//         uint8_t x = (opcode & 0x0F00) >> 8;
//         uint8_t y = (opcode & 0x00F0) >> 4;
//         uint8_t lastDig = (opcode & 0x000F);

//         if (lastDig == 0)
//         {
//             chip8->V[x] = chip8->V[y];
//         }
//         if (lastDig == 1)
//         {
//             chip8->V[x] = chip8->V[x] | chip8->V[y];
//         }
//         if (lastDig == 2)
//         {
//             chip8->V[x] = chip8->V[x] & chip8->V[y];
//         }
//         if (lastDig == 3)
//         {
//             chip8->V[x] = chip8->V[x] ^ chip8->V[y];
//         }
//         if (lastDig == 4)
//         {
//             uint16_t sum = chip8->V[x] + chip8->V[y];

//             // Erst Ergebnis schreiben, dann Flag (falls x == F)
//             chip8->V[x] = sum & 0xFF;
//             chip8->V[F] = (sum > 255) ? 1 : 0;
//         }
//         if (lastDig == 5)
//         {
//             uint8_t flag = (chip8->V[x] >= chip8->V[y]) ? 1 : 0;

//             chip8->V[x] = chip8->V[x] - chip8->V[y];
//             chip8->V[F] = flag;
//         }
//         if (lastDig == 6)
//         {
//             uint8_t flag = chip8->V[x] & 1;

//             chip8->V[x] /= 2;
//             chip8->V[F] = flag;
//         }
//         if (lastDig == 7)
//         {
//             uint8_t flag = (chip8->V[y] >= chip8->V[x]) ? 1 : 0;

//             chip8->V[x] = chip8->V[y] - chip8->V[x];
//             chip8->V[F] = flag;
//         }
//         // 0001 0000 = 16
//         // << 1
//         // 0010 0000 = 32
//         // 0x80 => 8 in hex ist bin : 1000
//         //  0 => 0 in hex ist bin : 0000
//         // zsm 0x80 in bin = 1000 0000
//         // also ganz links das msb
//         if (lastDig == 0xE)
//         {
//             uint8_t flag = chip8->V[x] >> 7;

//             chip8->V[x] <<= 1;
//             // chip8->V[x] = chip8->V[x] * 2; ist dasselbe
//             chip8->V[F] = flag;
//         }
//     }
//     break;

//     case 0x9000:
//     {
//         uint8_t x = (opcode & 0x0F00) >> 8;
//         uint8_t y = (opcode & 0x00F0) >> 4;

//         if (chip8->V[x] != chip8->V[y])
//         {
//             chip8->pc += 2;
//         }
//     }
//     break;

//     case 0xA000:
//     {
//         uint16_t nnn = (opcode & 0x0FFF);
//         chip8->I = nnn;
//     }
//     break;

//     case 0xB000:
//     {
//         uint16_t nnn = (opcode & 0x0FFF);
//         chip8->pc = nnn + chip8->V[0];
//     }
//     break;

//     case 0xC000:
//     {
//         uint8_t x = (opcode & 0x0F00) >> 8;
//         uint8_t nn = (opcode & 0x00FF);

//         uint8_t randomNum = rand() % 256;
//         chip8->V[x] = randomNum & nn;
//     }
//     break;

//     case 0xD000:
//     {
//         uint8_t x = (opcode & 0x0F00) >> 8;
//         uint8_t y = (opcode & 0x00F0) >> 4;
//         uint8_t n = (opcode & 0x000F);

//         uint8_t xCoord = chip8->V[x] % 64;
//         uint8_t yCoord = chip8->V[y] % 32;

//         chip8->V[F] = 0;

//         for (int row = 0; row < n; row++)
//         {
//             uint8_t spriteByte = chip8->memory[chip8->I + row];

//             for (int col = 0; col < 8; col++)
//             {
//                 if ((spriteByte << col) & 0x80)
//                 {
//                     uint32_t screenX = (xCoord + col) % 64;
//                     uint32_t screenY = (yCoord + row) % 32;

//                     uint32_t index = (screenY * 64) + screenX;

//                     if (chip8->gfx[index] == 1)
//                     {
//                         chip8->V[F] = 1;
//                     }

//                     chip8->gfx[index] ^= 1;
//                 }
//             }
//         }
//     }
//     break;

//     case 0xE000:
//     {
//         uint8_t x = (opcode & 0x0F00) >> 8;
//         uint8_t lastBytes = opcode & 0x00FF;

//         if (lastBytes == 0x9E)
//         {
//             if (chip8->key[chip8->V[x] & 0xF])
//             {
//                 chip8->pc += 2;
//             }
//         }

//         if (lastBytes == 0xA1)
//         {
//             if (!(chip8->key[chip8->V[x] & 0xF]))
//             {
//                 chip8->pc += 2;
//             }
//         }
//     }
//     break;

//     case 0xF000:
//     {
//         uint8_t x = (opcode & 0x0F00) >> 8;
//         uint8_t lastDigits = (opcode & 0x00FF);

//         if (lastDigits == 0x07)
//         {
//             chip8->V[x] = chip8->delay_timer;
//         }

//         if (lastDigits == 0x0A)
//         {
//             int keyPressed = 0;

//             for (int i = 0; i < 16; i++)
//             {
//                 if (chip8->key[i])
//                 {
//                     chip8->V[x] = i;
//                     keyPressed = 1;
//                     break;
//                 }
//             }

//             if (!keyPressed)
//             {
//                 chip8->pc -= 2;
//             }
//         }

//         if (lastDigits == 0x15)
//         {
//             chip8->delay_timer = chip8->V[x];
//         }

//         if (lastDigits == 0x18)
//         {
//             chip8->sound_timer = chip8->V[x];
//         }

//         if (lastDigits == 0x1E)
//         {
//             chip8->I += chip8->V[x];
//             // chip8->I = chip8->I + chip8->V[x];
//         }

//         if (lastDigits == 0x29)
//         {
//             chip8->I = 0x050 + ((chip8->V[x] & 0xF) * 5);
//         }

//         if (lastDigits == 0x33)
//         {
//             uint8_t value = chip8->V[x];

//             uint8_t hundreds = value / 100;
//             uint8_t tens = (value / 10) % 10;
//             uint8_t ones = value % 10;

//             chip8->memory[chip8->I & 0x0FFF] = hundreds;
//             chip8->memory[(chip8->I + 1) & 0x0FFF] = tens;
//             chip8->memory[(chip8->I + 2) & 0x0FFF] = ones;
//         }
//         if (lastDigits == 0x55)
//         {
//             for (int i = 0; i <= x; i++)
//             {
//                 chip8->memory[(chip8->I + i) & 0x0FFF] = chip8->V[i];
//             }
//         }
//         if (lastDigits == 0x65)
//         {
//             for (int i = 0; i <= x; i++)
//             {
//                 chip8->V[i] = chip8->memory[(chip8->I + i) & 0x0FFF];
//             }
//         }
//     }
//     break;

//     default:
//         printf("Unbekannter Opcode: 0x%04X\n", opcode);
//         break;
//     }
// }

#include <SDL3/SDL.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "emu_cycle.h"
#include "load_rom.h"
#include "init_chip.h"


int main(int argc, char const *argv[])
{
    Chip8 myChip;
    initialize(&myChip);

    srand(time(NULL));

    const char *rom = argv[1];

    if (!load_rom(&myChip, rom))
    {
        printf("fehler: nicht bereit fuer die Emulation!\n");
        return 1;
    }
    // --- SDL3 Initialisierung ---
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        printf("SDL konnte nicht initialisiert werden! Fehler: %s\n", SDL_GetError());
        return 1;
    }

    // Fenster mit 640x320 Pixeln erstellen (10-fache Skalierung der 64x32 CHIP-8 Auflösung)
    SDL_Window *window = SDL_CreateWindow("CHIP-8 Emulator", 640, 320, 0);
    if (!window)
    {
        printf("Fenster konnte nicht erstellt werden! Fehler: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
    {
        printf("Renderer konnte nicht erstellt werden! Fehler: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    // VSync bewusst aus: Das Timing macht die 60-Hz-Schleife unten selbst
    SDL_SetRenderVSync(renderer, 0);

    // 1. Textur einmalig erstellen (z. B. 64x32 für die originale CHIP-8 Auflösung)
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);

    // 2. Direkt danach den Skalierungsmodus auf "pixelig/scharf" setzen:
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    bool running = true;
    SDL_Event event;

    // statt "static uint8_t brightness[...]" vor der while-Schleife:
    static uint8_t prev_gfx[64 * 32] = {0};
    // 60-Hz-Takt unabhängig von der Monitor-Frequenz
    const uint64_t frame_ns = 1000000000ULL / 60;
    uint64_t next_frame = SDL_GetTicksNS();

    // Mapping von Tastatur-Tasten auf das CHIP-8 Keypad (0x0 bis 0xF)
    // CHIP-8 Layout: 1 2 3 C / Q W E R / A S D F / Z X C V
    while (running)
    {
        // 1. Events abfangen (Tastatur & Fenster schließen)
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }

            // Tasten drücken (KeyDown) und loslassen (KeyUp)
            if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP)
            {
                bool isPressed = (event.type == SDL_EVENT_KEY_DOWN);
                switch (event.key.key)
                {
                case SDLK_1:
                    myChip.key[0x1] = isPressed;
                    break;
                case SDLK_2:
                    myChip.key[0x2] = isPressed;
                    break;
                case SDLK_3:
                    myChip.key[0x3] = isPressed;
                    break;
                case SDLK_4:
                    myChip.key[0xC] = isPressed;
                    break;
                case SDLK_Q:
                    myChip.key[0x4] = isPressed;
                    break;
                case SDLK_W:
                    myChip.key[0x5] = isPressed;
                    break;
                case SDLK_E:
                    myChip.key[0x6] = isPressed;
                    break;
                case SDLK_R:
                    myChip.key[0xD] = isPressed;
                    break;
                case SDLK_A:
                    myChip.key[0x7] = isPressed;
                    break;
                case SDLK_S:
                    myChip.key[0x8] = isPressed;
                    break;
                case SDLK_D:
                    myChip.key[0x9] = isPressed;
                    break;
                case SDLK_F:
                    myChip.key[0xE] = isPressed;
                    break;
                case SDLK_Z:
                    myChip.key[0xA] = isPressed;
                    break;
                case SDLK_X:
                    myChip.key[0x0] = isPressed;
                    break;
                case SDLK_C:
                    myChip.key[0xB] = isPressed;
                    break;
                case SDLK_V:
                    myChip.key[0xF] = isPressed;
                    break;
                case SDLK_ESCAPE:
                    SDL_DestroyWindow(window);
                    return 1;
                }
            }
        }

        // 2. CPU-Zyklen pro Frame ausführen (15 Zyklen * 60 Hz = 900 Instruktionen/s)
        for (int i = 0; i < 15; i++)
        {
            emulate_cycle(&myChip);
        }

        // Timer herunterzählen (60 Hz, einmal pro Frame)
        if (myChip.delay_timer > 0)
            myChip.delay_timer--;
        if (myChip.sound_timer > 0)
            myChip.sound_timer--;

        // 3. Grafik auf den Bildschirm bringen (GFX-Array in SDL-Textur konvertieren)
        // statt der alten Pixel-Schleife im Loop:
        uint32_t pixels[64 * 32];
        for (int i = 0; i < 64 * 32; i++)
        {
            uint8_t on = myChip.gfx[i] | prev_gfx[i];
            prev_gfx[i] = myChip.gfx[i];

            pixels[i] = on ? 0xFFFFFFFF : 0xFF000000;
        }

        SDL_UpdateTexture(texture, NULL, pixels, 64 * sizeof(uint32_t));

        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        // Feste 60-Hz-Taktung statt SDL_Delay(16)
        next_frame += frame_ns;
        uint64_t now = SDL_GetTicksNS();
        if (next_frame > now)
            SDL_DelayNS(next_frame - now);
        else
            next_frame = now; // hinterher: nicht aufholen
    }

    // --- Aufräumen ---
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}