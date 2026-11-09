#!/usr/bin/env python3
"""
DS203 License Generator

Reverse-engineered from DFU bootloader and SYS V1.60 source.

Two DFU algorithm variants exist, both using hash = hash * 31 + byte:

  NEW (DFU >= V2.81): init=0xDEC0DE02, serial loop i=20..30, const2 loop i=57..70
  OLD (DFU < V2.81):  init=0xDEC0DE02, serial loop i=2..12,  const contrib=0x4013E5E1

The manufacturer typically provides two license codes per device,
one for each algorithm variant.

Usage:
    python3 gen_license.py <serial>
    python3 gen_license.py <word0> <word1> <word2>
"""

import sys

CONST2 = 0xD5203002

def compute_new(serial_sum):
    """DFU >= V2.81 (V3.45C, V3.46C): loops 20..30 and 57..70"""
    serial_sum &= 0xFFFFFFFF
    h = 0xDEC0DE02
    for i in range(20, 31):
        b = (serial_sum >> ((i % 4) * 8)) & 0xFF
        h = ((h * 31) + b) & 0xFFFFFFFF
    for i in range(57, 71):
        b = (CONST2 >> ((i % 4) * 8)) & 0xFF
        h = ((h * 31) + b) & 0xFFFFFFFF
    return h

def compute_old(serial_sum):
    """Older DFU (< V2.81): loop 2..12 for serial, fixed const contribution"""
    serial_sum &= 0xFFFFFFFF
    sp = 0
    for i in range(2, 13):
        b = (serial_sum >> ((i % 4) * 8)) & 0xFF
        sp = ((sp * 31) + b) & 0xFFFFFFFF
    M = 2**32
    return (0xDEC0DE02 * pow(31, 25, M) + sp * pow(31, 14, M) + 0x4013E5E1) % M

def main():
    args = [a for a in sys.argv[1:] if not a.startswith('-')]

    if len(args) == 3:
        w0, w1, w2 = (int(a, 0) for a in args)
        serial_sum = (w0 + w1 + w2) & 0xFFFFFFFF
    elif len(args) == 1:
        serial_sum = int(args[0], 0)
    else:
        print(f"Usage: {sys.argv[0]} <serial>")
        print(f"       {sys.argv[0]} <word0> <word1> <word2>")
        sys.exit(1)

    print(f"Serial: 0x{serial_sum:08X}")
    print(f"  NEW: {compute_new(serial_sum):08X}  (DFU >= V2.81)")
    print(f"  OLD: {compute_old(serial_sum):08X}  (DFU < V2.81)")

if __name__ == "__main__":
    main()
