#!/usr/bin/env python3
"""Pin whole original key wrapper, fresh pointer and wrapping leaves reused natively."""
import hashlib
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_overworld_mario_air_shadow import dol_bytes
PINS = [(2148430964, 100, '7b8e06d2aff85b14c07ed4981aaccc7e29601fb7801e03ab77fc36f288a1c06f'), (2147868864, 8, '3a59459f28ddd5d287c200434ad158a9dfe277f4f6bcaae3eb8698505aa9773d'), (2147860980, 40, '2355261017e21abec8723a2e8e16dc95e049b5cc11ffae10395adfe72089b442'), (2147860928, 52, '2f7a25677970c4fa4a4ee6740bde4c79382361b5f3f7987e56e2ced532c3f301'), (2148234888, 4, 'f332ea5b5437103cbb6f1508679da89eec9288ad775c96c439a17fccabe3de8e'), (2148234892, 4, 'f332ea5b5437103cbb6f1508679da89eec9288ad775c96c439a17fccabe3de8e')]


def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as source:
        if source.read(8) != b'G8ME01\0\0':
            raise ValueError('US revision zero required')
    for address, size, expected in PINS:
        if hashlib.sha256(dol_bytes(iso, address, size)).hexdigest() != expected:
            raise ValueError(f'Original field key body changed: {address:08x}')
    print('Whole original key wrapper, pointer, Mario leaves and return-only party bodies pinned')


if __name__ == '__main__':
    verify()
