# 🟣 CHIP-8 Emulator

**A precision-focused CHIP-8 interpreter written in modern C, powered by SDL3 for hardware-accelerated rendering and low-latency input.**

<p align="center">
  <img src="https://img.shields.io/badge/language-C-blue?style=flat-square" alt="Language">
  <img src="https://img.shields.io/badge/graphics-SDL3-purple?style=flat-square" alt="SDL3">
  <img src="https://img.shields.io/badge/status-stable-brightgreen?style=flat-square" alt="Status">
  <img src="https://img.shields.io/badge/opcodes-35%2F35-success?style=flat-square" alt="Opcodes">
</p>

---

## 📖 Overview

This project is a clean-room implementation of the classic **CHIP-8 virtual machine**, designed around three principles: **architectural accuracy**, **minimal dependencies**, and **readable source code**. It targets the original COSMAC VIP specification and runs any standard `.ch8` ROM.

No bloat. No magic. Just a faithful interpreter.

---

## ✨ Features

| | |
|---|---|
| 🎯 **Full Opcode Coverage** | All 35 standard CHIP-8 instructions implemented and verified. |
| 🖼️ **SDL3 Rendering Pipeline** | GPU-backed texture streaming with crisp `NEAREST` pixel scaling. |
| ⏱️ **Precise 60 Hz Timing** | Hardware-independent clock synchronization — no busy-wait loops. |
| 🛡️ **Safe ROM Loading** | Memory bounds validated against the 4 KB address space before boot. |
| 🧩 **Modular Source Layout** | Clean separation between CPU, memory, I/O, and the main loop. |

---

## 🧠 Architecture

The interpreter mirrors the physical **RCA COSMAC VIP** hardware layout:

```
├── Memory           4,096 bytes RAM       (ROM loads at 0x200)
├── Registers        16 × 8-bit V0–VF     + 16-bit I + Program Counter
├── Display          64 × 32 monochrome   XOR-blended pixel framebuffer
├── Stack            16 levels            for CALL / RET
└── Fontset          80 bytes             loaded at 0x050–0x0A0
```

---

## 📂 Project Structure

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

## 🎮 Controls

The 16-key hexadecimal keypad maps directly onto the QWERTY home rows:

| CHIP-8 | `1` | `2` | `3` | `C` | | CHIP-8 | `4` | `5` | `6` | `D` |
|:------:|:---:|:---:|:---:|:---:|:-:|:------:|:---:|:---:|:---:|:---:|
| **Key** | `1` | `2` | `3` | `4` | | **Key** | `Q` | `W` | `E` | `R` |

| CHIP-8 | `7` | `8` | `9` | `E` | | CHIP-8 | `A` | `0` | `B` | `F` |
|:------:|:---:|:---:|:---:|:---:|:-:|:------:|:---:|:---:|:---:|:---:|
| **Key** | `A` | `S` | `D` | `F` | | **Key** | `Z` | `X` | `C` | `V` |

> **Note** — Press `ESC` at any time to exit cleanly.

---

## ⚙️ Building

**Requirements:** `gcc` (or `clang`) and the SDL3 development libraries installed system-wide.

```bash
make
```

---

## 🚀 Running

Pass a ROM path directly to the binary:

```bash
./chip8-emu <path_to_rom.ch8>
```

Or use the bundled test target:

```bash
make run
```

---

## 🔬 Implementation Notes

The main loop performs 16-bit instruction fetch and dispatches through a bitmask-based decode tree — the standard approach for CHIP-8, chosen for both speed and clarity:

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

Each instruction updates the framebuffer and timers independently of the render tick, keeping emulation speed consistent across different host machines.

---

## 📄 License

Released under the **MIT License** — see `LICENSE` for details.