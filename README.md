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

CarnEvil (Midway, 1998) was a light-gun rail shooter on Midway's Seattle board:
a MIPS R5000 with a 3dfx Voodoo 1. This project recompiles the game's MIPS code,
and the board's BIOS, into C with [MidwayRecomp](https://github.com/sp00nznet/MidwayRecomp),
and runs it natively against an emulation of the board's hardware.

The runtime only plays the part of the hardware. It copies the BIOS out of the
boot ROM the way the boot ROM's own warm-reset code does and starts the CPU at
the BIOS entry. From there the BIOS initialises the board, reads the disk image
over emulated IDE, loads `GAME.EXE` and runs it. No game state is set up by hand,
no game function is called out of order, and no memory is poked to get past a
check.

## Status

![Power-up screen and attract mode, first boot from a blank CMOS](docs/attract.png)

From power-on the game runs its attract mode by itself, in real time:

- The BIOS ("Phoenix Seattle R5000 BIOS for Carnevil") boots, configures PCI
  and the IDE controller, and loads `GAME.EXE` from the disk by DMA.
- The game starts its runtime ("Mother GOOSE - R5000"). On a blank CMOS it
  restores factory settings itself, and the CMOS is kept for the next boot.
- Glide initialises the Voodoo 1, textures load, and the power-up screen shows
  the version, serial number, coinage, gun and DCS2 status.
- The game boots the DCS2 sound board, downloads its sound program and banks,
  and checks its version (0223).
- Attract mode cycles through the legend, Spooky Sam's Ghost Tour, the logo,
  the AAMA warning and the 3D demo scenes, waiting for coins.

Not checked yet: gameplay (coin up with `5`, start with `1`, mouse is the gun),
audio playback in attract mode, and performance with heavy in-game overdraw.
The game's sound-board timeouts are wall-clock, as on the board, so a heavily
loaded host can make its sound init take one retry.

## How it works

| Piece | What it is |
|---|---|
| `recomp_out/` (generated) | The game and the BIOS as C, one function per MIPS function. Every load and store goes through the runtime, so device registers behave like a real bus. |
| `src/runtime/hw.c` | The board: GT64010 system controller (PCI configuration, DMA, timers, interrupt cause/mask), PCI IDE disk with bus-master DMA, Midway I/O ASIC with its security PIC, CMOS, light guns, interrupt routing and video timing. |
| `src/runtime/rtos.c` | CPU services: COP0, the BIOS's `break` service calls, address lookup for jumps through pointers, and the BIOS jump table the game copies into itself at startup. |
| `src/runtime/proc.c` | The game's cooperative processes. Midway's dispatcher switches processes by swapping the stack pointer, which C cannot do, so each process runs on a host fiber. The process list, sleep counts, creation and killing are still the game's own code. |
| `src/runtime/voodoo_sst1.c` | The 3dfx Voodoo 1: register file, FIFO and vsync'd swaps, LFB, texture units, pixel pipeline and the DAC behaviour Glide probes. |
| `src/runtime/dcs.c`, `adsp2100.c` | The DCS2 sound board: an ADSP-2115 core booting from the board ROM, the board memory map, host latches, the I/O ASIC FIFO and audio output. |
| `src/runtime/main.c`, `audio.c` | Boot, a native window, keyboard and mouse, sound output. |

Time is driven by the recompiled code itself: every few thousand memory
accesses (and on every COP0 `Count` read) the hardware gets a tick, which
advances the 57 Hz video timing and delivers pending interrupts through the
BIOS's own interrupt dispatcher. That is what lets the game's waits on its
vblank counter, disk flags and timers finish the way they do on the board.

## Building

You need your own CarnEvil v1.0.1 dumps: the boot ROM `carnevil1_9.u32`, the
DCS2 ROM `sound102.u95`, and the hard disk (MAME's `carnevi1.chd`, decompressed
to a raw image with `tools/chd_extract.py`). Game code and data are not included
in this repository, and the C generated from them is never committed.

```sh
# 1. Recompiler inputs from your boot ROM and disk (checked by SHA-1)
python tools/prepare_inputs.py carnevil1_9.u32 carnevil/carnevi1.img

# 2. Generate recomp_out/ (needs a MidwayRecomp build in ../MidwayRecomp)
tools/regen.sh

# 3. Build (MSVC 2022, CMake 3.20+)
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

Run from the repository root, with `carnevil1_9.u32`, `sound102.u95` and
`carnevil/carnevi1.img` in place:

```sh
build/Release/carnevil.exe                  # window: 5 coin, 1 start, F2 test switch, mouse = gun
build/Release/carnevil.exe --headless --frames 600 --shot out.ppm
```

The game's own debug console (the BIOS banner, Glide messages, the developers'
error messages) prints to stderr. Writes to the disk stay in memory; the CMOS is
kept in `nvram.bin`.

`python tools/smoke_test.py build/Release/carnevil.exe` boots from a blank CMOS
headless and checks the BIOS and game milestones.

Diagnostics: `CARNEVIL_PERF=1` (frame timing), `CARNEVIL_PROFILE=1` (sampling
profiler), `CARNEVIL_IDE_TRACE`, `CARNEVIL_IRQ_TRACE`, `CARNEVIL_PCI_TRACE`.

## Symbol files

`carnevil_syms.toml` and `rtos_syms.toml` tell the recompiler where functions
start. Functions reached only through pointers (driver tables, handlers passed
to the BIOS) and functions whose first instructions were scheduled above the
stack-frame setup were missing from the original lists; `tools/find_entries.py`
and `tools/find_hoisted.py` find them, and `tools/syms_add.py` adds them.
A jump to an address that is still missing stops the runtime with the address.

## License

The code in this repository is MIT (see `LICENSE`); third-party code it draws
on is listed in `NOTICE`. The MIT grant covers this project's tools, runtime,
build files and documentation. It does not and cannot cover CarnEvil itself:
the game's code, art, audio and data belong to their rights holders, are not
distributed here, and you need your own legitimate copy. Code the tools
generate from the game is a derivative of it, is made from your own copy, and
is never committed here.

---

*Built with stubbornness, quarters, and the lingering trauma of Tokkentakker by [sp00nznet](https://github.com/sp00nznet)*
