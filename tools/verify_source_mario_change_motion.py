#!/usr/bin/env python3
"""Pin complete original motion-change/stay-post and reached table records."""
import hashlib
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_overworld_mario_air_shadow import dol_bytes
PINS = [(2148091228, 192, 'cb0d860ff6a94e5e6d0da5dded999fef1e6b881c2169730ce9d09c7f0439da26'), (2148166416, 172, '045f375d0d3f44bb67ed8d1b0e36f43a7e5d4d90a7a3fdc15e1477764c3b4376'), (2147868864, 8, '3a59459f28ddd5d287c200434ad158a9dfe277f4f6bcaae3eb8698505aa9773d'), (2150694960, 48, '0697c61704c157ae7e794420df3c968dd1f38b02e82e673ec701bcfcf065ca00')]

def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as source:
        if source.read(8) != b'G8ME01\0\0':
            raise ValueError('US revision zero required')
    for address, size, expected in PINS:
        if hashlib.sha256(dol_bytes(iso,address,size)).hexdigest() != expected:
            raise ValueError(f'Original motion body/data changed: {address:08x}')
    print('Whole original marioChgMot/mot_stay_post, GetPtr and original table records pinned')

if __name__ == '__main__':
    verify()
