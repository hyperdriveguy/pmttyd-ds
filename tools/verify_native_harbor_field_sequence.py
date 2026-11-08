#!/usr/bin/env python3
"""Pin complete original source parents reused by the native sequence owner."""
import hashlib
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_overworld_mario_air_shadow import dol_bytes

PINS = [
    (0x8004ef54, 0xb8, '279bfc7dd90633634ae77475a0e6bb49688853836c74210ad5bb267a6db23b62'),
    (0x8004eed8, 0x7c, '3ce3d14c0105a232a61153ebb6161804a24c23747ad2e274f31ec92062db6dcb'),
    (0x8004f894, 0x58, 'bd57fbcab0beafde808c84ebfce609053943708af08d4e04876e31da2e0dfed7'),
]


def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as source:
        if source.read(8) != b'G8ME01\0\0':
            raise ValueError('US revision zero required')
    for address, size, expected in PINS:
        if hashlib.sha256(dol_bytes(iso, address, size)).hexdigest() != expected:
            raise ValueError(f'Original field callback changed: {address:08x}')
    print('Whole US Start/Wait/GetRegularId pinned; native frontend policy is separate')


if __name__ == '__main__':
    verify()
