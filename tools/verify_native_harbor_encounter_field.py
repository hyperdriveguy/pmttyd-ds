#!/usr/bin/env python3
"""Pin original event SetBattleInfo body; native constructor policy is separate."""
import hashlib
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_overworld_mario_air_shadow import dol_bytes

PINS = [(0x8004f090, 0x64, '1eb9b46a1e9a81f28a698df9272ceef265120817e0c25123d4f2da4ef8a703c9')]


def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as source:
        if source.read(8) != b'G8ME01\0\0':
            raise ValueError('US revision zero required')
    for address, size, expected in PINS:
        if hashlib.sha256(dol_bytes(iso, address, size)).hexdigest() != expected:
            raise ValueError(f'Original callback changed: {address:08x}')
    print('Whole original SetBattleInfo event parent pinned (8004f090/64)')


if __name__ == '__main__':
    verify()
