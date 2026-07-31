#!/usr/bin/env python3
"""Rewrite an -mips3 object's e_flags so it links as -mips2 (sibling convention)."""

import sys

path = sys.argv[1]
with open(path, "rb+") as fd:
    fd.seek(0x24)
    fd.write(bytes([0x10]))
