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
| **Display** | 512x384 @ 57 Hz |

Source: [System16 Hardware Page](https://www.system16.com/hardware.php?id=618)

## Current Status: Rendering Pipeline Active, Scene Graph Populated

The game fully boots, enters attract mode, and the 3D rendering pipeline is connected end-to-end. Zone files (BABY.ZM, baby.za) are loaded from the TRAP filesystem into game memory. The scene graph has active nodes and the Voodoo emulation processes 3.8M register writes per run. LFB pixel writes and FastFill are handled with double-buffered output. Next milestone: breaking DCS2 polling loops so the zone parser can create geometry-attached scene objects.

### What's Working

- **2,498 recompiled MIPS functions** (2,187 game + 304 RTOS + 7 stubs) running as native x64
- **Full RTOS emulation** -- 27 vector table trampolines, device callbacks (VEC[60]/[64]), fiber-based cooperative task scheduler with proper task message dispatch (channel 6)
- **Complete game boot sequence**: RTOS banner -> env config -> diagnostics -> CMOS -> DCS2 handshake -> free play -> attract mode
- **State machine dispatcher** correctly reads registered mode entries at 0x001A25F0
- **Attract mode lifecycle**: init (func_800C50AC) -> camera update (func_800CADD4) -> scene functions
- **Zone file I/O**: device_open returns small positive indices, vec[18] (static_0_800C411C) copies BABY.ZM (90KB), baby.za (246KB) into game heap buffers
- **Scene graph**: 5 nodes created via func_800D7600, linked list at 0x0017B71C traversed by renderer
- **Voodoo double-buffered emulation**: LFB writes (1.7M/run) to back buffer, FastFill, SwapBuffers copy, 3.8M register writes total
- **LFB/Register separation**: physical 0x00800000+ = framebuffer pixels, 0x08100000+ = Voodoo registers (was previously misidentified, causing 65K fake triangles)
- **Widget board registers**: return proper 512x384 video config with render-enable bits
- **Display rendering gates**: mode cycling (0x40->0x20), channel active flags, Voodoo PCI base forced
- **Zone config parser**: processes ~50 commands per frame from the zone setup stream
- **Device I/O**: DCS2 (0x69XX), IOASIC (0x74XX), PIC commands with proper handshake responses
- **Heap management** with per-frame snapshot/restore
- **PCI configuration space bridge**, CMOS/NVRAM, DMA buffer allocation

### What's Next

- [ ] **Break DCS2 polling loops** -- Fibers and main thread enter infinite DCS/IOASIC polling (0x6909/0x7405 alternating). Need to return correct "done" status or implement cooperative yielding
- [ ] **Zone geometry parsing** -- Zone FILE parser (func_801143CC -> func_800D036C -> func_800D7600) creates geometry-attached scene nodes. Currently blocked by DCS polling in the async task pipeline
- [ ] **Voodoo Triangle Rasterization** -- Implement edge-walking using vertex setup registers (SVX/SVY/dXdY) for 3D pixel output
- [ ] **Texture Support** -- Load WMS textures and map through TMU registers
- [ ] **Modern GPU Backend** -- Replace software Voodoo with Vulkan/OpenGL
- [ ] **DCS2 Audio** -- ADSP-2115 DSP or direct DCS 3.0 audio bank decoding
- [ ] **Input System** -- Mouse/gamepad/Sinden lightgun, networked 2P co-op

## Architecture

```
carnevil/
├── tools/                     Extraction & analysis tools
│   ├── chd_extract.py         CHD v5 -> raw disk image
│   ├── seattle_fs.py          TRAP/Phoenix filesystem extractor
│   ├── make_elf.py            Flat MIPS binary -> ELF (for Ghidra/IDA)
│   └── analyze_*.py           Binary analysis scripts
├── src/runtime/               Hardware runtime shims
│   ├── seattle_runtime.c      Main runtime: memory map, MMIO, frame loop
│   ├── seattle_overrides.c    Function overrides for RTOS/game functions
│   ├── voodoo.c/h             3DFX Voodoo 1 register emulation + FastFill
│   ├── rtos_scheduler.c/h     Cooperative fiber scheduler (Windows fibers)
│   ├── rtos_trampolines.c     27 vector table -> RTOS function redirects
│   ├── rtos_stubs.c           RTOS function stubs + callback system
│   ├── rtos_registration.c    304 RTOS function registrations
│   ├── input.c/h              Input system (mouse/gamepad/lightgun)
│   ├── dcs_sound.c/h          DCS2 audio stubs
│   └── recomp.h               Recompiled code support macros
├── recomp_out/                N64Recomp output (recompiled C code)
│   ├── funcs/                 2,187 game functions
│   └── rtos_funcs/            304 RTOS functions
├── extracted/                 Extracted game data
│   ├── GAME.bin/elf           Game binary (914 KB, loads at 0x800C4000)
│   ├── RTOS.bin/elf           RTOS binary (89 KB, loads at 0x80000000)
│   ├── cmos_nvram.bin         CMOS/NVRAM from MAME
│   └── files/                 1,361 game assets
├── carnevil_syms.toml         Game symbol file (2,044 functions)
├── rtos_syms.toml             RTOS symbol file (317 functions)
├── carnevil_recomp.toml       Game recompilation config
├── rtos_recomp.toml           RTOS recompilation config
└── CMakeLists.txt             Build configuration
```

## Building

```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

Requires: MSVC 2022+, CMake 3.20+, game ROM files (not included)

## Why Static Recompilation?

MAME emulates CarnEvil already, but emulation has overhead and limitations. Static recompilation converts the original MIPS machine code directly into equivalent C code that compiles natively. The result runs at full speed without a CPU interpreter, and the translated code can be understood, modified, and enhanced.

The Midway Seattle platform is a great target:
- **Simple CPU** -- MIPS-IV is clean and regular
- **Small binary** -- ~1.7MB of code+data
- **Known hardware** -- MAME has fully documented every chip
- **Debug strings** -- Function names and error messages baked in
- **3DFX Voodoo** -- Well-documented register interface
- **Shared RTOS** -- "Mother GOOSE" RTOS used by ALL Seattle games (Wayne Gretzky, Mace, Blitz, California Speed, Hyperdrive, etc.)

## Related Projects

- **[pcrecomp](https://github.com/sp00nznet/pcrecomp)** -- Our unified x86 PC recompilation toolbox
- **[N64Recomp](https://github.com/N64Recomp/N64Recomp)** -- MIPS static recompilation for N64 by Mr-Wiseguy

## Requirements

- MSVC 2022+ (Windows) or GCC/Clang (Linux, untested)
- CMake 3.20+
- Python 3.10+ with `soundfile`, `numpy` (for extraction tools)
- Game ROM files (not included)

## License

MIT

---

*Built with stubbornness, quarters, and the lingering trauma of Tokkentakker by [sp00nznet](https://github.com/sp00nznet)*
