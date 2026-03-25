# CarnEvil Static Recompilation

```
     ██████╗ █████╗ ██████╗ ███╗   ██╗███████╗██╗   ██╗██╗██╗
    ██╔════╝██╔══██╗██╔══██╗████╗  ██║██╔════╝██║   ██║██║██║
    ██║     ███████║██████╔╝██╔██╗ ██║█████╗  ██║   ██║██║██║
    ██║     ██╔══██║██╔══██╗██║╚██╗██║██╔══╝  ╚██╗ ██╔╝██║██║
    ╚██████╗██║  ██║██║  ██║██║ ╚████║███████╗ ╚████╔╝ ██║███████╗
     ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚══════╝  ╚═══╝  ╚═╝╚══════╝

        "Step right up! Don't be shy! Everybody's gonna DIE!"
```

**Tearing apart Midway's 1998 horror-themed light gun arcade classic and putting it back together for modern hardware.**

CarnEvil was a coin-op rail shooter where you blasted your way through a haunted amusement park full of undead clowns, evil toys, and a giant eyeball boss named Eyeclops. It ran on Midway's Seattle arcade board -- a MIPS R5000 CPU bolted to a 3DFX Voodoo 1 GPU. The goal: statically recompile the game's MIPS binary into native C/C++ so it runs on a modern PC without emulation.

## Hardware Target

| Component | Spec |
|-----------|------|
| **CPU** | MIPS R5000LE @ 150 MHz (MIPS-IV ISA) |
| **RAM** | 8 MB DRAM @ 0x80000000 (kseg0) |
| **GPU** | 3DFX Voodoo 1 -- FBI (2MB framebuffer) + TMU (4MB texture) |
| **Sound** | DCS2 -- ADSP-2115 @ 16 MHz, 2MB DRAM, DCS 3.0 compressed audio |
| **Storage** | IDE hard drive, custom "TRAP" / Phoenix filesystem |
| **I/O** | Midway IOASIC (shuffled register mode), analog lightguns, PIC security |
| **Display** | 640x480 @ 57 Hz |

Source: [System16 Hardware Page](https://www.system16.com/hardware.php?id=618)

## Current Status: Phase 1 -- Reverse Engineering

### What's Done

- **CHD Extraction** -- Custom Python CHD v5 extractor (`tools/chd_extract.py`) that handles LZMA, zlib, Huffman, FLAC, and self-referencing codecs. Both disk images extracted and SHA1-verified.
- **TRAP Filesystem Parsed** -- Reverse-engineered the Midway "TRAP" partition format: 17 partitions, CLFN32 directory entries with 1,515 files catalogued. Custom extractor at `tools/seattle_fs.py`.
- **Asset Inventory Complete** -- 1,506 files extracted including:
  - 1,134 `.WMS` textures (Voodoo RGB565 pixel data)
  - 73 `.ZM` + `.ZM2` 3D models (IEEE float vertex/face data)
  - 42 `.ZA` + `.ZA2` animations
  - 62 `.BNK` DCS 3.0 audio banks
  - 26 `.PTH` + `.PT2` AI pathfinding data
  - 5 `.RAW` + `.RW2` camera rail data
  - `GAME.EXE`, `DIAG.EXE`, config files
- **MIPS Code Located** -- Found the game binary embedded in the metadata partition with debug strings intact (`main_init_sound()`, function names, error messages). Init sequence identified: stack at 0x80800000, BSS clear from 0x801A3708, ~1.7MB code+data.
- **Hardware Memory Map Documented** -- Full address map from MAME analysis:
  - Galileo GT64010 system controller
  - Voodoo 1 PCI at 16MB BAR (registers + LFB + texture port)
  - IOASIC at 0x16000000 (CarnEvil shuffle mode)
  - Lightgun registers at 0x16800000 (dual analog guns)
  - CMOS/NVRAM at 0x16100000
  - Interrupt routing (Galileo, IOASIC, IDE, VBlank)

### What's Next

- [ ] **Properly extract GAME.EXE** -- The Phoenix filesystem block-chain mapping (FAT at 0x8800, f1c partition selector, f1a block offset) needs to be fully decoded to extract the contiguous 1.7MB MIPS binary
- [ ] **MIPS-IV Disassembly** -- Full recursive-descent disassembly of the game binary, leveraging N64Recomp's MIPS lifter as a starting point
- [ ] **Function Discovery** -- Use JAL/JR targets, prologue patterns (`addiu sp, sp, -N`), and debug strings to identify all ~1,700+ functions
- [ ] **Static Recompilation** -- Lift MIPS-IV to C, adapting N64Recomp's approach for the Seattle platform
- [ ] **Hardware Runtime Shims** -- Reimplement Voodoo 1 (-> Vulkan/OpenGL), DCS2 audio, IOASIC, lightgun input, PIC security as C libraries. Heavy reference from MAME's `seattle.cpp`, `midwayic.cpp`, and `voodoo_pci.cpp`.
- [ ] **Asset Pipeline** -- Decode WMS textures, ZM models, ZA animations, BNK audio into modern formats

## Architecture

```
carnevil/
├── tools/              Extraction & analysis tools
│   ├── chd_extract.py  CHD v5 -> raw disk image
│   └── seattle_fs.py   TRAP filesystem parser & file extractor
├── docs/               Hardware docs & memory maps (TODO)
├── src/                Recompiled game code (TODO)
│   ├── recomp/         Lifted C from MIPS binary
│   └── runtime/        Hardware shim libraries
│       ├── voodoo/     3DFX Voodoo 1 -> modern GPU
│       ├── dcs/        DCS2 audio system
│       ├── ioasic/     Midway I/O ASIC
│       └── seattle/    Board-level glue (interrupts, timers, IDE)
└── assets/             Decoded game assets (TODO)
```

## Why Static Recompilation?

MAME emulates CarnEvil already, but emulation has overhead and limitations. Static recompilation converts the original MIPS machine code directly into equivalent C code that compiles natively. The result runs at full speed without a CPU interpreter, and the translated code can be understood, modified, and enhanced. Think of it as a mechanical translation -- every MIPS instruction becomes a C statement, every hardware register access becomes a function call into our runtime shims.

The Midway Seattle platform is actually a great target for this:
- **Simple CPU** -- MIPS-IV is clean and regular, no microcode weirdness
- **Small binary** -- ~1.7MB of code+data fits in your head
- **Known hardware** -- MAME has fully documented every chip on the board
- **Debug strings** -- The binary has function names and error messages baked in
- **3DFX Voodoo** -- Well-documented GPU with a straightforward register interface

## Related Projects

This is part of the [sp00nznet](https://github.com/sp00nznet) recompilation ecosystem:
- **[pcrecomp](https://github.com/sp00nznet/pcrecomp)** -- Unified x86 PC recompilation toolbox (Soldier of Fortune, X-Wing Alliance, Civilization, etc.)
- **[N64Recomp](https://github.com/N64Recomp/N64Recomp)** -- MIPS static recompilation for N64 (same CPU family!)

## Requirements

- Python 3.10+ with `soundfile`, `numpy` (for CHD FLAC codec)
- Game disk images (CHD format, not included)

## License

MIT

---

*Built with stubbornness, quarters, and the lingering trauma of Tökkentäkker by [sp00nznet](https://github.com/sp00nznet)*
