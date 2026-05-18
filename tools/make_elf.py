#!/usr/bin/env python3
"""
Create a minimal MIPS ELF from CarnEvil's flat GAME.bin binary.
Output can be loaded directly into Ghidra, IDA, or other analysis tools.
"""
import struct
import sys
import os


def create_mips_elf(bin_path, elf_path, load_addr):
    with open(bin_path, 'rb') as f:
        code = f.read()

    # Find BSS start (last non-zero byte + alignment)
    last_nz = len(code) - 1
    while last_nz > 0 and code[last_nz] == 0:
        last_nz -= 1
    last_nz += 1

    # Approximate code/data split: scan for where dense code stops
    # and data/strings begin. For now, use the full binary as one segment.
    code_size = len(code)
    va_end = load_addr + code_size

    # ELF constants
    EI_NIDENT = 16
    ET_EXEC = 2
    EM_MIPS = 8
    EV_CURRENT = 1
    ELFCLASS32 = 1
    ELFDATA2LSB = 1
    PT_LOAD = 1
    PF_R = 4
    PF_W = 2
    PF_X = 1
    SHT_PROGBITS = 1
    SHT_NOBITS = 8
    SHT_STRTAB = 3
    SHF_ALLOC = 2
    SHF_EXECINSTR = 4
    SHF_WRITE = 1
    EF_MIPS_ARCH_4 = 0x60000000  # MIPS IV

    EHDR_SIZE = 52
    PHDR_SIZE = 32
    SHDR_SIZE = 40

    # Layout:
    # - ELF header (52 bytes)
    # - Program header (1 entry, 32 bytes)
    # - Section headers (at end)
    # - Code data (aligned)
    # Sections: NULL, .text+data, .bss, .shstrtab

    num_phdrs = 1
    num_shdrs = 4  # NULL, .text, .bss, .shstrtab

    # Section name string table
    shstrtab = b'\x00.text\x00.bss\x00.shstrtab\x00'
    shstr_text = 1
    shstr_bss = 7
    shstr_shstrtab = 12

    # Offsets
    code_offset = EHDR_SIZE + num_phdrs * PHDR_SIZE
    # Align to 16 bytes
    code_offset = (code_offset + 15) & ~15
    shstrtab_offset = code_offset + code_size
    shdr_offset = shstrtab_offset + len(shstrtab)
    # Align section headers to 4 bytes
    shdr_offset = (shdr_offset + 3) & ~3

    # BSS: from end of loaded data to 0x80800000
    bss_va = va_end
    bss_size = 0x80800000 - va_end

    # Build ELF header
    ehdr = bytearray(EHDR_SIZE)
    # e_ident
    ehdr[0:4] = b'\x7fELF'
    ehdr[4] = ELFCLASS32
    ehdr[5] = ELFDATA2LSB
    ehdr[6] = EV_CURRENT
    # e_type
    struct.pack_into('<H', ehdr, 16, ET_EXEC)
    # e_machine
    struct.pack_into('<H', ehdr, 18, EM_MIPS)
    # e_version
    struct.pack_into('<I', ehdr, 20, EV_CURRENT)
    # e_entry
    struct.pack_into('<I', ehdr, 24, load_addr)
    # e_phoff
    struct.pack_into('<I', ehdr, 28, EHDR_SIZE)
    # e_shoff
    struct.pack_into('<I', ehdr, 32, shdr_offset)
    # e_flags (MIPS IV)
    struct.pack_into('<I', ehdr, 36, EF_MIPS_ARCH_4)
    # e_ehsize
    struct.pack_into('<H', ehdr, 40, EHDR_SIZE)
    # e_phentsize
    struct.pack_into('<H', ehdr, 42, PHDR_SIZE)
    # e_phnum
    struct.pack_into('<H', ehdr, 44, num_phdrs)
    # e_shentsize
    struct.pack_into('<H', ehdr, 46, SHDR_SIZE)
    # e_shnum
    struct.pack_into('<H', ehdr, 48, num_shdrs)
    # e_shstrndx
    struct.pack_into('<H', ehdr, 50, 3)  # .shstrtab is section 3

    # Build program header (single LOAD segment)
    phdr = bytearray(PHDR_SIZE)
    struct.pack_into('<I', phdr, 0, PT_LOAD)       # p_type
    struct.pack_into('<I', phdr, 4, code_offset)    # p_offset
    struct.pack_into('<I', phdr, 8, load_addr)      # p_vaddr
    struct.pack_into('<I', phdr, 12, load_addr)     # p_paddr
    struct.pack_into('<I', phdr, 16, code_size)     # p_filesz
    struct.pack_into('<I', phdr, 20, code_size + bss_size)  # p_memsz (includes BSS)
    struct.pack_into('<I', phdr, 24, PF_R | PF_W | PF_X)   # p_flags
    struct.pack_into('<I', phdr, 28, 0x1000)        # p_align

    # Build section headers
    shdrs = bytearray(num_shdrs * SHDR_SIZE)

    # Section 0: NULL
    # (already zeroed)

    # Section 1: .text (code + data)
    off = 1 * SHDR_SIZE
    struct.pack_into('<I', shdrs, off + 0, shstr_text)     # sh_name
    struct.pack_into('<I', shdrs, off + 4, SHT_PROGBITS)   # sh_type
    struct.pack_into('<I', shdrs, off + 8, SHF_ALLOC | SHF_EXECINSTR | SHF_WRITE)  # sh_flags
    struct.pack_into('<I', shdrs, off + 12, load_addr)      # sh_addr
    struct.pack_into('<I', shdrs, off + 16, code_offset)    # sh_offset
    struct.pack_into('<I', shdrs, off + 20, code_size)      # sh_size
    struct.pack_into('<I', shdrs, off + 36, 4)              # sh_addralign

    # Section 2: .bss
    off = 2 * SHDR_SIZE
    struct.pack_into('<I', shdrs, off + 0, shstr_bss)       # sh_name
    struct.pack_into('<I', shdrs, off + 4, SHT_NOBITS)      # sh_type
    struct.pack_into('<I', shdrs, off + 8, SHF_ALLOC | SHF_WRITE)  # sh_flags
    struct.pack_into('<I', shdrs, off + 12, bss_va)          # sh_addr
    struct.pack_into('<I', shdrs, off + 16, 0)               # sh_offset (no file data)
    struct.pack_into('<I', shdrs, off + 20, bss_size)        # sh_size
    struct.pack_into('<I', shdrs, off + 36, 4)               # sh_addralign

    # Section 3: .shstrtab
    off = 3 * SHDR_SIZE
    struct.pack_into('<I', shdrs, off + 0, shstr_shstrtab)  # sh_name
    struct.pack_into('<I', shdrs, off + 4, SHT_STRTAB)      # sh_type
    struct.pack_into('<I', shdrs, off + 16, shstrtab_offset) # sh_offset
    struct.pack_into('<I', shdrs, off + 20, len(shstrtab))   # sh_size
    struct.pack_into('<I', shdrs, off + 36, 1)               # sh_addralign

    # Write ELF file
    with open(elf_path, 'wb') as out:
        out.write(ehdr)
        out.write(phdr)
        # Padding to code_offset
        padding = code_offset - EHDR_SIZE - PHDR_SIZE
        out.write(b'\x00' * padding)
        out.write(code)
        out.write(shstrtab)
        # Padding to section header alignment
        cur = shstrtab_offset + len(shstrtab)
        pad = shdr_offset - cur
        if pad > 0:
            out.write(b'\x00' * pad)
        out.write(shdrs)

    total = shdr_offset + len(shdrs)
    print(f'Created ELF: {elf_path}')
    print(f'  Entry point:  {load_addr:#010x}')
    print(f'  .text:        {load_addr:#010x} - {va_end:#010x} ({code_size:,} bytes)')
    print(f'  .bss:         {bss_va:#010x} - 0x80800000 ({bss_size:,} bytes)')
    print(f'  ELF size:     {total:,} bytes')


def main():
    if len(sys.argv) < 2:
        print(f'Usage: {sys.argv[0]} <binary.bin> [output.elf] [load_address]')
        print(f'  Creates a MIPS-IV LE ELF from a flat binary for Ghidra/IDA analysis')
        print(f'  Default load address: 0x800C4000 (CarnEvil GAME.EXE)')
        sys.exit(1)

    bin_path = sys.argv[1]
    elf_path = sys.argv[2] if len(sys.argv) > 2 else bin_path.rsplit('.', 1)[0] + '.elf'
    load_addr = int(sys.argv[3], 0) if len(sys.argv) > 3 else 0x800C4000

    create_mips_elf(bin_path, elf_path, load_addr)


if __name__ == '__main__':
    main()
