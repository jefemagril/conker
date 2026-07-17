#!/usr/bin/env python3
"""Patch an ELF32 .rodata section size and alignment (keeps contents/relocs)."""
import struct
import sys


def main() -> None:
    path = sys.argv[1]
    new_size = int(sys.argv[2], 0)
    new_align = int(sys.argv[3], 0) if len(sys.argv) > 3 else 4
    data = bytearray(open(path, "rb").read())
    if data[:4] != b"\x7fELF":
        raise SystemExit(f"{path}: not ELF")

    # ELF32 big-endian (N64)
    e_shoff = struct.unpack_from(">I", data, 32)[0]
    e_shentsize, e_shnum, e_shstrndx = struct.unpack_from(">HHH", data, 46)
    str_off = struct.unpack_from(">I", data, e_shoff + e_shstrndx * e_shentsize + 16)[0]

    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        name_off = struct.unpack_from(">I", data, hoff)[0]
        name = data[str_off + name_off :].split(b"\0", 1)[0]
        if name == b".rodata":
            struct.pack_into(">I", data, hoff + 20, new_size)   # sh_size
            struct.pack_into(">I", data, hoff + 32, new_align)  # sh_addralign
            open(path, "wb").write(data)
            return

    raise SystemExit(f"{path}: no .rodata section")


if __name__ == "__main__":
    main()
