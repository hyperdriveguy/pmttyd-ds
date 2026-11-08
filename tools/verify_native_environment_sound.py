#!/usr/bin/env python3
"""Pin original empty-ENV chain and finite native initialization contributions."""
import hashlib
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_overworld_mario_air_shadow import dol_bytes
PINS = [
 (0x800dc4e0, 0x170, '3e0abb3a40bb3770ab7ad17952694b9d29c945ff9219102ebe5d53fdbe1e0483'),
 (0x80151e0c, 0x2c, 'b7276ed3cd3cdc76158c1976cee082f69a1d0e617ab4f6347c605190a62ed09d'),
 (0x800d76b8, 0x28, '8abf56c86225c5333691bf61ccc72c8dbcc6530b1b31e42a631eaa7c85520fda'),
 (0x800d7470, 0x248, 'c4988e7dc53bd3a0163e0c303af95a39a7ac99c85a441542339cc52e69c38069'),
 (0x800dc5ec, 0x10, '0102203ae06b64d8dc7e9b390cc0bd5e1eb7a50affef53cd524fae1dbc69e53e'),
 (0x80005418, 0x10, '76a62b3dba4a365727ca04077b196ca71058a5e7c9fd484abfe1ad0eeb44bc32'),
]


def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as source:
        if source.read(8) != b'G8ME01\0\0':
            raise ValueError('US revision zero required')
    for address, size, expected in PINS:
        if hashlib.sha256(dol_bytes(iso, address, size)).hexdigest() != expected:
            raise ValueError(f'Original environment body/data changed: {address:08x}')
    if dol_bytes(iso, 0x80416520, 4) != bytes.fromhex('0000000a'):
        raise ValueError('Actual original SDA lookup word changed')
    print('Whole ENV Off chain, original two-store contribution/ABI and actual SDA word pinned')


if __name__ == '__main__':
    verify()
