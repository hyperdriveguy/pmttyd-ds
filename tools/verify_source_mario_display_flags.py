#!/usr/bin/env python3
"""Read-only original whole Mario display flags source pins."""
import hashlib
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_overworld_mario_air_shadow import dol_bytes
PINS = [(2148431272, 144, '351e7196c21a8558c902586ab0aaa97c8d0aca949e55a351a4c7bac7e6335291'), (2147868864, 8, '3a59459f28ddd5d287c200434ad158a9dfe277f4f6bcaae3eb8698505aa9773d')]

def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as source:
        if source.read(8) != b'G8ME01\0\0':
            raise ValueError('US revision zero required')
    for address, size, expected in PINS:
        if hashlib.sha256(dol_bytes(iso,address,size)).hexdigest() != expected:
            raise ValueError(f'Original display flags body changed: {address:08x}')
    print('Whole original Mario displayflags and GetPtr pinned')

if __name__ == '__main__':
    verify()
