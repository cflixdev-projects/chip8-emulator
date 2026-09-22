# 🟣 CHIP-8 Emulator

A high-performance, minimalist **CHIP-8 interpreter** written in modern C, leveraging **SDL3** for hardware-accelerated graphics and low-latency input handling.

┌────────────────────────────────────────────────────────┐│  🟣 Status: Stable & Architectural Accuracy Verified  │└────────────────────────────────────────────────────────┘

---

## ⚡ Core Features

- **Accurate CPU Emulation** | Complete coverage of the standard 35 opcode variants.
- **Modern Rendering Pipeline** | Powered by SDL3 texture streaming with sharp `NEAREST` pixel scaling.
- **60Hz Clock Synchronization** | Hardware-independent precision timing without CPU-hogging loops.
- **Boundary-Safe Bootloader** | Strict memory footprint validation against the 4KB RAM layout.

---

## 🧠 Hardware Architecture

The interpreter precisely mirrors the physical VIP COSMAC structural specifications:

```text
├── 📦 Memory Mapping   --> 4,096 Bytes System RAM (Program space starts at 0x200)
├── 🎛️ Registers        --> 16x 8-bit Data (V0-VF) + 16-bit Index (I) + PC
├── 📺 Display Grid     --> Monochromatic 64x32 frame-blended hardware pixel cache
└── 🔤 Font Engine      --> Integrated 5-byte sprite fontset loaded at 0x050-0x0A0
```

---

## 🛠️ Repository Blueprint

```text
├── headerfiles/       # Decoupled structural API definitions
├── emu_cycle.c        # Core fetch-decode-execute handler
├── init_chip.c        # Hardware state & memory reset manager
├── load_rom.c         # High-level binary system parser
├── main.c             # SDL3 subsystem execution loop
└── Makefile           # Automated multi-file compiler rules
```

---

## 🎮 Keymaps & Controls

The original hexadecimal keypad is seamlessly mapped to standard QWERTY environments:

| CHIP-8 Key | QWERTY Map | | CHIP-8 Key | QWERTY Map |
| :---: | :---: | :---: | :---: | :---: |
| `1` `2` `3` `C` | `1` `2` `3` `4` | | `7` `8` `9` `E` | `A` `S` `D` `F` |
| `4` `5` `6` `D` | `Q` `W` `E` `R` | | `A` `0` `B` `F` | `Z` `X` `C` `V` |

> [!TIP]
> 🟣 *Press `ESC` at any point during runtime execution to gracefully terminate window processes.*

---

## ⚙️ Quick Start

### 1. Build Compilation
Ensure **GCC** and **SDL3** developmental paths are configured globally, then run:
```bash
make
```

### 2. Execution Run
Pass your target game binary directly into the execution pipeline:
```bash
./chip8-emu <path_to_rom.ch8>
```
*Alternatively, use the predefined testing target via:* `make run`

---

## 📝 Core Pipeline Deep-Dive

The main loop handles internal 16-bit wide instruction isolation and decodes bitwise signatures using high-speed cascading tree structures:

```c
// Fetch standard opcode chunks from memory bounds
uint16_t opcode = (chip8->memory[chip8->pc] << 8) | chip8->memory[chip8->pc + 1];
chip8->pc += 2;

// Fast bitmask decoding structure
switch (opcode & 0xF000) {
    case 0xA000: // ANNN: Set Index Register I
        chip8->I = (opcode & 0x0FFF);
        break;
    case 0xD000: // DXYN: Hardware XOR Sprite Rendering
        // High-speed blitting pipelines here...
        break;
}
```