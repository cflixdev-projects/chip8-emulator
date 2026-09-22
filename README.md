# CHIP-8 Emulator

A CHIP-8 interpreter written in C, using SDL3 for rendering and input.

<p align="center">
  <img src="https://img.shields.io/badge/language-C-blue?style=flat-square" alt="Language">
  <img src="https://img.shields.io/badge/graphics-SDL3-purple?style=flat-square" alt="SDL3">
  <img src="https://img.shields.io/badge/status-stable-brightgreen?style=flat-square" alt="Status">
  <img src="https://img.shields.io/badge/opcodes-35%2F35-success?style=flat-square" alt="Opcodes">
</p>

---

## Overview

This is a from-scratch implementation of the CHIP-8 virtual machine, targeting the original COSMAC VIP specification. It runs standard `.ch8` ROMs and aims for correctness and clarity over feature creep.

---

## Features

| | |
|---|---|
| **Opcode Coverage** | All 35 standard CHIP-8 instructions implemented. |
| **Rendering** | SDL3 texture streaming with `NEAREST` pixel scaling. |
| **Timing** | 60 Hz clock synchronization, independent of host framerate. |
| **ROM Loading** | Memory bounds validated against the 4 KB address space before execution. |
| **Source Layout** | Separate translation units for CPU, memory, I/O, and the main loop. |

---

## Architecture

The interpreter follows the RCA COSMAC VIP hardware layout:

```
├── Memory           4,096 bytes RAM       (ROM loads at 0x200)
├── Registers        16 × 8-bit V0–VF     + 16-bit I + Program Counter
├── Display          64 × 32 monochrome   XOR-blended pixel framebuffer
├── Stack            16 levels            for CALL / RET
└── Fontset          80 bytes             loaded at 0x050–0x0A0
```

---

## Project Structure

```
.
├── headerfiles/        # Public API & struct definitions
├── emu_cycle.c         # Fetch → Decode → Execute core
├── init_chip.c         # Hardware state initialization
├── load_rom.c          # Binary ROM parser & bounds check
├── main.c              # SDL3 window, input, and event loop
└── Makefile            # Multi-file build rules
```

---

## Controls

The 16-key hexadecimal keypad maps to the QWERTY layout as follows:

| CHIP-8 | `1` | `2` | `3` | `C` | | CHIP-8 | `4` | `5` | `6` | `D` |
|:------:|:---:|:---:|:---:|:---:|:-:|:------:|:---:|:---:|:---:|:---:|
| **Key** | `1` | `2` | `3` | `4` | | **Key** | `Q` | `W` | `E` | `R` |

| CHIP-8 | `7` | `8` | `9` | `E` | | CHIP-8 | `A` | `0` | `B` | `F` |
|:------:|:---:|:---:|:---:|:---:|:-:|:------:|:---:|:---:|:---:|:---:|
| **Key** | `A` | `S` | `D` | `F` | | **Key** | `Z` | `X` | `C` | `V` |

Press `ESC` to exit.

---

## Building

**Requirements:** `gcc` (or `clang`) and the SDL3 development libraries.

```bash
make
```

---

## Running

```bash
./chip8-emu <path_to_rom.ch8>
```

Or use the bundled test target:

```bash
make run
```

---

## Implementation

The main loop fetches a 16-bit instruction and dispatches on the high nibble using a bitmask switch. This is the standard decoding strategy for CHIP-8 and keeps the dispatch path predictable.

```c
/* Fetch two bytes and assemble the 16-bit opcode */
uint16_t opcode = (chip8->memory[chip8->pc] << 8)
                |  chip8->memory[chip8->pc + 1];
chip8->pc += 2;

/* Dispatch on the high nibble */
switch (opcode & 0xF000) {
    case 0xA000:  /* ANNN — set index register */
        chip8->I = opcode & 0x0FFF;
        break;

    case 0xD000:  /* DXYN — XOR sprite blit */
        /* render pipeline */
        break;

    /* ... */
}
```

Timers and the framebuffer are updated independently of the render tick, so emulation speed does not depend on the host's refresh rate.

---

## License

MIT — see `LICENSE` for details.