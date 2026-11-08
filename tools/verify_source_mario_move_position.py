#!/usr/bin/env python3
"""Pin whole original mov_pos2/force-start and their actual literal/ABI inputs."""
import hashlib
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_overworld_mario_air_shadow import dol_bytes
PINS = [(2148425552, 384, '46a943846e957cde37abe848ca4c7ee371ba4a9049ca151a5e5d720db3ef88d2'), (2148097680, 212, 'c043f4c04b6028d61cbd965f41d66be2d797e95dff531f61bec763d5c944a391'), (2147868864, 8, '3a59459f28ddd5d287c200434ad158a9dfe277f4f6bcaae3eb8698505aa9773d'), (2147505176, 16, '76a62b3dba4a365727ca04077b196ca71058a5e7c9fd484abfe1ad0eeb44bc32'), (2151815948, 4, 'ab086b9b9d8f6327e5a6475217234808a919c1e2719b15dcfe8e861ebfa671b7'), (2151815896, 4, 'de620abd6d3615746360c1d15ce3a56291236d37424124054a49d25e947ffc4d'), (2150514360, 8, '07af8387624cf8f36152797d2f561246f21237d1d22b5c13006eacfa6f475e43')]


def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as source:
        if source.read(8) != b'G8ME01\0\0':
            raise ValueError('US revision zero required')
    for address, size, expected in PINS:
        if hashlib.sha256(dol_bytes(iso, address, size)).hexdigest() != expected:
            raise ValueError(f'Original move body/data changed: {address:08x}')
    print('Whole original mov_pos2/force start, fresh pointer, source ABI and literals pinned')


if __name__ == '__main__':
    verify()
