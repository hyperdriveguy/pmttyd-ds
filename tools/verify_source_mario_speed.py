#!/usr/bin/env python3
"""Read-only original whole speed getter/Bgmode source pins."""
import hashlib
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_overworld_mario_air_shadow import dol_bytes
PINS = [(2148039908, 160, 'bf4e93869b303f771d68395708b2c7252cb4aaf89c037df3f9cf386beef94b4c'), (2148040068, 156, '44b48a29b00754fa795bd2d8e3c9cea81dc97d4748e823e00aefb93c68f369ef'), (2147860836, 16, '793b4a2433739c998d83da3ba3e3eae39963f55b7e3ae0878c96222b7fcf0d42'), (2147868864, 8, '3a59459f28ddd5d287c200434ad158a9dfe277f4f6bcaae3eb8698505aa9773d'), (2151811212, 4, '1b9e0fe7b740b4e583727aa1c9d27ff8c01491f911d2de0747f8c997ef6b3498')]

def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as source:
        if source.read(8) != b'G8ME01\0\0':
            raise ValueError('US revision zero required')
    for address, size, expected in PINS:
        if hashlib.sha256(dol_bytes(iso,address,size)).hexdigest() != expected:
            raise ValueError(f'Original speed body/data changed: {address:08x}')
    print('Whole original Dash/Walk/Bgmode/GetPtr and actual half factor pinned')

if __name__ == '__main__':
    verify()
