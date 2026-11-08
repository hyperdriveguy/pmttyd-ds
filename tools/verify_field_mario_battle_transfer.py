#!/usr/bin/env python3
"""Read-only US Prepare/Post/party body pins; no engine-completion claim."""
import hashlib
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_overworld_mario_air_shadow import dol_bytes

PINS = [
    (0x8005c3cc, 0x6c, 'cf6119386901ec44eac10a0408897195448a7c50925c63f46acdc68ecf9d04fa'),
    (0x8005c30c, 0xc0, '0c32635c15a028711e9c9f241da886ec7d65967141c8a46dd11bfb48d2a563d2'),
    (0x800bcf34, 0x5c, '83b854256122fbfcc9126d2722882cf7674f8f3078970a1b3886eb7d69331ea9'),
    (0x800bed44, 0x38, '480fc234b664147581dc21623e9af2fc2c2cf0ba0a35c3386880bd5e36d253a6'),
    (0x800bed0c, 0x38, '0ce771d9991495096138debff7650f315c875f8d056b2acf6b22cdae0c835318'),
    (0x80141050, 0x98, '2339c1d1b7c127d2570ff8125d1c901db3676fca636de93d725f3f923b02923b'),
    (0x800bf1b4, 0x20, 'b338694df4542858c65e2ac8e13a77b66393a928b732dd679665bcfdbfac572a'),
]


def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as source:
        if source.read(8) != b'G8ME01\0\0':
            raise ValueError('Original US revision zero required')
    for address, size, expected in PINS:
        actual = hashlib.sha256(dol_bytes(iso, address, size)).hexdigest()
        if actual != expected:
            raise ValueError(f'Original body changed at {address:08x}')
    print('Seven whole original Prepare/Post/party bodies pinned; ISO read-only')


if __name__ == '__main__':
    verify()
