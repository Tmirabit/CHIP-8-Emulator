# CHIP-8 Emulator

A CHIP-8 emulator written in **C** using **SDL3** for graphics, keyboard input, and window management.

This project was built as a hands-on systems programming project to learn more about low-level programming, computer architecture, memory management, instruction decoding, graphics rendering, and emulator design.

## Features

* CHIP-8 CPU emulation
* 4 KB memory
* 16 general-purpose 8-bit registers (`V0`–`VF`)
* 16-level call stack
* Program counter and index register
* CHIP-8 delay and sound timers
* 64 × 32 monochrome display
* CHIP-8 keypad input mapped to a PC keyboard
* ROM loading
* CHIP-8 instruction decoding and execution
* Sprite drawing with XOR and collision detection
* SDL3-based graphics rendering
* CMake build system

## Technologies

* **C**
* **SDL3**
* **CMake**
* **Git / GitHub**

## Project Structure

```text
CHIP8/
├── CMakeLists.txt
├── SDL3/
├── Roms/
└── src/
    ├── main.c
    ├── chip8.c
    └── chip8.h
```

### Source Files

**`main.c`**

* Initializes SDL3
* Creates the window, renderer, and texture
* Handles keyboard input
* Runs the emulator loop
* Sends framebuffer data to SDL3 for rendering

**`chip8.c`**

* Initializes the CHIP-8 system
* Loads ROMs into memory
* Fetches, decodes, and executes CHIP-8 instructions
* Handles graphics, registers, memory, stack, and timers

**`chip8.h`**

* Defines the CHIP-8 system state
* Contains shared function declarations

## CHIP-8 Architecture

The emulator models the original CHIP-8 hardware/software environment, including:

| Component       | Description                       |
| --------------- | --------------------------------- |
| Memory          | 4096 bytes                        |
| Registers       | 16 × 8-bit registers              |
| Index Register  | 16-bit `I` register               |
| Program Counter | 16-bit `PC` register              |
| Stack           | 16-level call stack               |
| Stack Pointer   | Tracks the current stack position |
| Display         | 64 × 32 monochrome pixels         |
| Keypad          | 16-key hexadecimal keypad         |
| Delay Timer     | 8-bit timer                       |
| Sound Timer     | 8-bit timer                       |

CHIP-8 programs are loaded beginning at memory address `0x200`.

## Keyboard Controls

The CHIP-8 hexadecimal keypad is mapped to the following keyboard layout:

```text
CHIP-8        Keyboard

1  2  3  C     1  2  3  4
4  5  6  D     Q  W  E  R
7  8  9  E     A  S  D  F
A  0  B  F     Z  X  C  V
```

## Building

This project uses CMake to configure and build the emulator.

### Configure

From the project root:

```bash
cmake -S . -B build
```

### Build

```bash
cmake --build build
```

The resulting executable will be placed in the appropriate build output directory depending on the platform and build configuration.

## Running

Place a CHIP-8 ROM in the `Roms/` directory and update the ROM path in `main.c` if necessary.

For example:

```c
chip8_load_game(&chip8, "Roms/Pong [Paul Vervalin, 1990].ch8");
```

Then run the generated executable.

> **Note:** The ROM path is currently specified directly in the source code. A future improvement would be allowing the ROM path to be provided through a command-line argument.

## Emulator Cycle

The emulator follows the basic CHIP-8 execution cycle:

```text
        ┌───────────────┐
        │ Fetch Opcode  │
        └───────┬───────┘
                ↓
        ┌───────────────┐
        │ Decode Opcode │
        └───────┬───────┘
                ↓
        ┌───────────────┐
        │ Execute       │
        └───────┬───────┘
                ↓
        ┌───────────────┐
        │ Update State  │
        └───────┬───────┘
                ↓
             Repeat
```

Each instruction is two bytes, and the program counter normally advances by two after an instruction is executed.

Jump, call, return, and conditional skip instructions modify the program counter differently.

## Graphics

CHIP-8 uses a **64 × 32 monochrome framebuffer**.

Sprites are stored in memory and drawn using the `DXYN` instruction. Sprite pixels are XORed against the existing framebuffer.

Collision detection is implemented through the `VF` register:

```text
VF = 1 → A pixel was turned off during drawing
VF = 0 → No collision occurred
```

The framebuffer is then converted into an SDL3 texture for display.

## ROMs

The emulator was tested using publicly available CHIP-8 ROMs.

The ROM metadata and ROM collection used for testing were obtained from the [`mir3z/chip8-emu` repository](https://github.com/mir3z/chip8-emu/blob/master/roms/roms.json).

Please refer to the original repository and individual ROM authors for the respective ROM copyrights and distribution terms.

## Resources & Acknowledgments

This project was developed independently, with external resources used to learn about CHIP-8 architecture and emulator implementation.

A major reference used during development was:

**How to Write an Emulator (CHIP-8 Interpreter)**
https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/

This resource was used as a learning/reference guide for understanding CHIP-8 architecture, instruction implementation, and emulator structure.

The project was implemented and debugged while learning the underlying concepts rather than directly copying an existing emulator implementation.

Additional ROM testing resources:

**mir3z/chip8-emu – ROM collection**
https://github.com/mir3z/chip8-emu/blob/master/roms/roms.json

## What I Learned

This project provided practical experience with:

* C programming
* Pointers and structures
* Bitwise operations
* Memory management
* Binary data and hexadecimal representation
* CPU instruction decoding
* Registers and memory addressing
* Stack-based subroutines
* Function pointers and modular program structure
* SDL3 graphics programming
* Keyboard input handling
* Framebuffers and sprite rendering
* CMake build systems
* Debugging low-level programs
* Understanding computer architecture through emulation

## Future Improvements

Potential future improvements include:

* [ ] Command-line ROM loading
* [ ] Proper 60 Hz delay/sound timer implementation
* [ ] More accurate CPU timing
* [ ] Sound/tone generation
* [ ] Pause/resume functionality
* [ ] Configurable keyboard mapping
* [ ] ROM selection interface
* [ ] CHIP-8 compatibility testing suite
* [ ] Support for additional CHIP-8 variants and quirks
* [ ] Improved SDL3 rendering
* [ ] Automated instruction-level testing

## Status

The emulator is currently a **work in progress** and is primarily intended as a learning project.

The core emulator functionality is implemented, and CHIP-8 programs such as **Pong** can be loaded and executed.

More compatibility testing and timing improvements are planned.

---

## License

This project is provided for educational purposes.

Third-party resources, including CHIP-8 ROMs, remain subject to their respective authors' licenses and distribution terms.
