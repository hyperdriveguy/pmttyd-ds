#!/usr/bin/env python3
"""Pin the whole original getter reused by the finite native adapter."""
import hashlib
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_overworld_mario_air_shadow import dol_bytes


def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as source:
        if source.read(8) != b'G8ME01\0\0':
            raise ValueError('US revision zero required')
    body = dol_bytes(iso, 0x800e71f8, 0xc8)
    if hashlib.sha256(body).hexdigest() != '59cbd35ca40765eb91c59b4dd87248ba53fb8e2fd2bde4da492b6e87b82d0beb':
        raise ValueError('Original evt_mario_get_pos800e71f8/c8 changed')
    print('Whole original evt_mario_get_pos800e71f8/c8 pinned; native sample policy separate')


if __name__ == '__main__':
    verify()
