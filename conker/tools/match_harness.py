#!/usr/bin/env python3
"""Byte-match harness for Conker classic matching (IDO -O2 leaves).

Compare a rebuilt function to the ROM text slice, score exact / JUSTREG matches,
and batch-try C variants against a GLOBAL_ASM site.

Run from the Conker build root (`conker/conker/`), usually inside Docker:

  python3 tools/match_harness.py score \\
      --func func_15077404 --vram 0x15077404 --size 0xB0

  # From a one-off trial script:
  from match_harness import MatchHarness
  h = MatchHarness(src="src/game_A28B0.c", unit="game_A28B0")
  h.try_variants("func_15077404", 0x15077404, 0xB0, variants)
"""
from __future__ import annotations

import argparse
import re
import subprocess
import sys
from pathlib import Path
from typing import Dict, Iterable, List, Optional, Tuple, Union

# Conker US: .text VRAM 0x15000000 maps to ROM offset 0x2D4B0.
DEFAULT_ROM = Path("conker.us.bin")
DEFAULT_ROM_OFF = 0x2D4B0
DEFAULT_VRAM_BASE = 0x15000000

# I-type opcodes whose lo16 is often a reloc when ours is 0 (lui/addiu/loads/stores).
_RELOC_I_OPCS = frozenset(
    (15, 0x20, 0x21, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2B, 0x09, 0x31, 0x39)
)


def get_targ(
    va: int,
    size: int,
    rom: Path = DEFAULT_ROM,
    rom_off: int = DEFAULT_ROM_OFF,
    vram_base: int = DEFAULT_VRAM_BASE,
) -> bytes:
    b = rom.read_bytes()
    off = rom_off + (va - vram_base)
    return b[off : off + size]


def extract(obj: Path, name: str) -> bytes:
    dump = subprocess.check_output(["mips-linux-gnu-objdump", "-d", str(obj)], text=True)
    words: List[bytes] = []
    grab = False
    for line in dump.splitlines():
        if f"<{name}>" in line:
            grab = True
            continue
        if grab:
            if line.startswith("0") and "<" in line and name not in line:
                break
            if "\t" in line and ":" in line:
                h = line.split("\t")[1].strip().split()
                if h and len(h[0]) == 8:
                    words.append(bytes.fromhex(h[0]))
            if not line.strip():
                break
    return b"".join(words)


def _mask_reloc(a: int, b: int) -> Tuple[int, int]:
    opc = a >> 26
    am, bm = a, b
    if opc in _RELOC_I_OPCS and (a & 0xFFFF) == 0:
        am &= 0xFFFF0000
        bm &= 0xFFFF0000
    if opc == 3:  # jal
        am &= 0xFC000000
        bm &= 0xFC000000
    return am, bm


def _strip_regs(x: int) -> int:
    """Keep opcode (+ funct/shamt or imm); clear register fields for JUSTREG compare."""
    op = x >> 26
    if op == 0:  # SPECIAL
        funct = x & 0x3F
        shamt = (x >> 6) & 0x1F
        return (op << 26) | (shamt << 6) | funct
    if op in (2, 3):  # j / jal
        return x & 0xFC000000
    return (op << 26) | (x & 0xFFFF)


def score(ours: bytes, targ: bytes) -> Tuple[int, int, List[int]]:
    """Exact match with reloc lo16 / jal target masking. Returns (matched, n, diff_indices)."""
    if len(ours) != len(targ):
        n = max(len(ours), len(targ)) // 4
        return -1, n, list(range(n))
    n = len(targ) // 4
    m = 0
    diffs: List[int] = []
    for i in range(n):
        a = int.from_bytes(ours[i * 4 : i * 4 + 4], "big")
        b = int.from_bytes(targ[i * 4 : i * 4 + 4], "big")
        am, bm = _mask_reloc(a, b)
        if am == bm:
            m += 1
        else:
            diffs.append(i)
    return m, n, diffs


def score_justreg(ours: bytes, targ: bytes) -> Tuple[int, int, List[int]]:
    """Same as score but ignores register fields (schedule/opcode identity)."""
    if len(ours) != len(targ):
        n = max(len(ours), len(targ)) // 4
        return -1, n, list(range(n))
    n = len(targ) // 4
    m = 0
    diffs: List[int] = []
    for i in range(n):
        a = int.from_bytes(ours[i * 4 : i * 4 + 4], "big")
        b = int.from_bytes(targ[i * 4 : i * 4 + 4], "big")
        am, bm = _mask_reloc(a, b)
        if _strip_regs(am) == _strip_regs(bm):
            m += 1
        else:
            diffs.append(i)
    return m, n, diffs


def disasm_func(obj: Path, name: str, limit: int = 60) -> str:
    dump = subprocess.check_output(["mips-linux-gnu-objdump", "-d", str(obj)], text=True)
    lines: List[str] = []
    grab = False
    for line in dump.splitlines():
        if f"<{name}>" in line:
            grab = True
            lines.append(line)
            continue
        if grab:
            if line.startswith("0") and "<" in line and name not in line:
                break
            lines.append(line)
            if len(lines) > limit:
                break
    return "\n".join(lines)


class MatchHarness:
    """Replace a GLOBAL_ASM with variant C bodies and score against the ROM."""

    def __init__(
        self,
        src: Union[str, Path],
        unit: str,
        obj: Optional[Union[str, Path]] = None,
        rom: Path = DEFAULT_ROM,
        rom_off: int = DEFAULT_ROM_OFF,
        vram_base: int = DEFAULT_VRAM_BASE,
        asm_dir: str = "asm/nonmatchings",
    ):
        self.src = Path(src)
        self.unit = unit
        # Normalize: src/game_A28B0.c → build/src/game_A28B0.c.o
        if obj is None:
            self.obj = Path("build") / Path(src)
            if not str(self.obj).endswith(".o"):
                self.obj = Path(str(self.obj) + ".o")
        else:
            self.obj = Path(obj)
        self.rom = Path(rom)
        self.rom_off = rom_off
        self.vram_base = vram_base
        self.asm_dir = asm_dir

    def get_targ(self, va: int, size: int) -> bytes:
        return get_targ(va, size, self.rom, self.rom_off, self.vram_base)

    def _asm_pragma_pat(self, asm_stem: str) -> re.Pattern[str]:
        # Matches optional NON-MATCHING comment lines above the pragma.
        path = re.escape(f"{self.asm_dir}/{self.unit}/{asm_stem}.s")
        return re.compile(rf'(?://.*\n)*#pragma GLOBAL_ASM\("{path}"\)\n(?://.*\n)*')

    def try_variants(
        self,
        func: str,
        va: int,
        size: int,
        variants: Dict[str, str],
        asm_stem: Optional[str] = None,
        keep: bool = True,
        dump_threshold: int = 10,
    ) -> Tuple[bool, str]:
        """Try each C body in place of GLOBAL_ASM. Restores src unless keep+match.

        Returns (matched, src_text). On match with keep=True, src already has the body.
        """
        asm_stem = asm_stem or func
        orig = self.src.read_text()
        pat = self._asm_pragma_pat(asm_stem)
        targ = self.get_targ(va, size)
        best: Tuple[int, Optional[str], Optional[str]] = (-1, None, None)

        for name, body in variants.items():
            text = pat.sub(body.strip() + "\n\n", orig, count=1)
            if text == orig:
                print(func, name, "NO_REPL")
                continue
            self.src.write_text(text)
            subprocess.run(["rm", "-f", str(self.obj)], check=True)
            r = subprocess.run(["make", str(self.obj)], capture_output=True, text=True)
            if r.returncode:
                err = [l for l in (r.stderr + r.stdout).splitlines() if "Error" in l][:2]
                print(f"{func} {name}: FAIL {err}")
                self.src.write_text(orig)
                continue
            ours = extract(self.obj, func)
            m, n, d = score(ours, targ)
            jr, _, jd = score_justreg(ours, targ)
            print(
                f"{func} {name}: {m}/{n} justreg={jr}/{n} "
                f"len={len(ours):#x}/{size:#x} diffs={d[:12]}"
            )
            if m > best[0]:
                best = (m, name, body)
            if m == n and len(ours) == size:
                print("*** MATCH", func, name)
                if keep:
                    return True, text
                self.src.write_text(orig)
                return True, orig
            if len(ours) == size and m >= dump_threshold:
                print(disasm_func(self.obj, func, 35))
                for i in d[:8]:
                    a = int.from_bytes(ours[i * 4 : i * 4 + 4], "big")
                    b = int.from_bytes(targ[i * 4 : i * 4 + 4], "big")
                    print(f"  [{i}] {a:08x} vs {b:08x}")
                if jr == n and m < n:
                    print(f"  (JUSTREG-perfect: exact diffs only regs {d[:16]})")
            self.src.write_text(orig)

        print(f"  BEST {best[0]} {best[1]}")
        return False, orig

    def score_built(self, func: str, va: int, size: int) -> None:
        """Score an already-built object against the ROM (no src edits)."""
        ours = extract(self.obj, func)
        targ = self.get_targ(va, size)
        m, n, d = score(ours, targ)
        jr, _, jd = score_justreg(ours, targ)
        print(f"{func}: exact {m}/{n} justreg={jr}/{n} len={len(ours):#x}/{size:#x}")
        print(f"  diffs={d[:20]}")
        if jr == n and m < n:
            print("  Plateau: JUSTREG-hard (opcodes match; registers differ).")
        elif m == -1:
            print("  Length mismatch — fix size/frame before JUSTREG thrash.")
        print(disasm_func(self.obj, func, 40))


def _cli(argv: Optional[Iterable[str]] = None) -> int:
    p = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    sub = p.add_subparsers(dest="cmd", required=True)

    sc = sub.add_parser("score", help="Score a function in an existing .o against the ROM")
    sc.add_argument("--func", required=True)
    sc.add_argument("--vram", required=True, type=lambda s: int(s, 0))
    sc.add_argument("--size", required=True, type=lambda s: int(s, 0))
    sc.add_argument("--src", default="src/game_A28B0.c", help="C file (for default .o path)")
    sc.add_argument("--obj", default=None, help="Override object path")
    sc.add_argument("--unit", default="game_A28B0")

    args = p.parse_args(list(argv) if argv is not None else None)
    if args.cmd == "score":
        h = MatchHarness(src=args.src, unit=args.unit, obj=args.obj)
        if not h.obj.is_file():
            print(f"missing {h.obj}; build it first (make {h.obj})", file=sys.stderr)
            return 1
        h.score_built(args.func, args.vram, args.size)
        return 0
    return 1


if __name__ == "__main__":
    sys.exit(_cli())
