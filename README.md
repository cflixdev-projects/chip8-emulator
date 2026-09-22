# CHIP-8 Emulator in C

A high-performance, lightweight **CHIP-8 interpreter** written in modern C and powered by **SDL3** for hardware-accelerated graphics and input handling.

Built with modularity and architectural accuracy in mind, this project emulates the original 1970s VIP COSMAC computer specifications, fetching, decoding, and executing instructions at a custom execution frequency.

---

## 🚀 Features

- **Accurate CPU Emulation:** Full support for the standard 35 opcode variants (including logical arithmetic, memory jumps, stack handling, and subroutines).
- **Modern Rendering Frontend:** Utilizes the new **SDL3** dynamic texture streaming API with `SDL_SCALEMODE_NEAREST` for razor-sharp, retro pixel scaling.
- **60Hz Timer Synchronization:** Hardware-independent timing implementation tracking the `delay_timer` and `sound_timer` cycles precisely without CPU-hogging busy loops.
- **Integrated ROM Bootloader:** Handles standard binary ROM structures with safety constraints checking boundaries against the CHIP-8 4KB memory footprint.

---

## 🧠 Architectural Overview

The interpreter mimics physical hardware components encapsulated inside a structural domain framework:

- **Memory Mapping:** 4,096 bytes of system RAM. Intercepts standard program space beginning at `0x200`.
- **Registers:** 16 standard 8-bit general-purpose data registers (`V0` - `VF`), along with a single 16-bit Index Register (`I`) and Program Counter (`pc`).
- **Graphics Pipeline:** Monochromatic 64x32 display grid mapped into a flattened hardware pixel cache buffer with active dynamic frame blending to prevent flickering.
- **Fontset:** Integrated 5-byte sprite font engine loaded dynamically inside the protected system area (`0x050`-`0x0A0`).

---

## 🛠️ File Structure

The project follows a clean decoupled C-file infrastructure separating the core state logic from visual processing environments:

```text
├── headerfiles/
│   ├── emu_cycle.h    # CPU cycle definitions and opcode parsing prototypes
│   ├── init_chip.h    # System memory and layout initialization definitions
│   └── load_rom.h     # Input/Output ROM parsing prototypes
├── emu_cycle.h        # Main fetch-decode-execute loop handler
├── init_chip.c        # Initializes pointers, clears registers and stacks
├── load_rom.c         # High-level binary system parser
├── main.c             # SDL3 setup, clock sync, and main event handler loop
└── Makefile           # Multi-file compilation build instructions
```

---

## 🎮 Key Mapping Layout

The original hex keypad layout of the CHIP-8 is intuitive and mapped seamlessly over to standard QWERTY keyboards:

| CHIP-8 Keypad | QWERTY Counterpart |
| :---: | :---: |
| `1` `2` `3` `C` | `1` `2` `3` `4` |
| `4` `5` `6` `D` | `Q` `W` `E` `R` |
| `7` `8` `9` `E` | `A` `S` `D` `F` |
| `A` `0` `B` `F` | `Z` `X` `C` `V` |

*Press `ESC` at any point during active emulation states to gracefully kill window processes [3].*

---

## ⚙️ Building and Running

### Prerequisites
Make sure your compiler chain contains developmental frameworks for **GCC** and **SDL3**.

### Compilation
Compile your software utilizing the dynamic configurations within the standard automation build engine `make`:

```bash
make
```

### Execution
Boot up target rom structures directly via explicit arguments passed onto target execution binaries:

```bash
./chip8-emu <path_to_rom.ch8>
```

Alternatively, use the predefined automated testing routines target built directly inside our configurations:

```bash
make run
```

---

## 📝 Code Implementation Deep-Dive

### The Main Fetch-Decode-Execute Cycle
Core execution models fetch 16-bit wide instructions via memory bounds matching the current `pc` tracking register. Opcodes then dynamically resolve bitwise signatures using cascading logic trees:

```c
// Extracting high bits to shift individual bytes into operational execution channels
uint16_t opcode = (chip8->memory[chip8->pc] << 8) | chip8->memory[chip8->pc + 1];
chip8->pc += 2;

switch (opcode & 0xF000) {
    case 0xA000: // ANNN: Sets index register I to the address NNN
        chip8->I = (opcode & 0x0FFF);
        break;
        
    case 0xD000: // DXYN: Draw sprite logic utilizing XOR screen collision checks
        // Dynamic sprite processing loop structures...
        break;
}
```
