#!/usr/bin/env python3
"""Pin whole TEX palette-to-GX source and actual original menu TPL headers."""
import argparse
import hashlib
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO, inventory
from verify_source_gx_texture_object import verify as verify_gx

BODY = (0x80037a80, 0xdc, '7c513078ebe80f85256108702b3b74d65b9036b6a1115b88c8a3750d19819fe6')
FILES = (
    ('battle/common/battle_common.tpl', 942647408, 217920,
     '8eb685317056f42b3ef73138eaad85d16fa4943d830b44a149f09971a5959bb1',
     ((0, '55b5dcf84a129751b7bf1c7525aac2c0ff33aa55d38efe9e83ea65d4d948266a'),
      (1, '2026a7eed80f4c85240d25697bb5ef26cb40641da42464104a86a92618497e6f'),
      (2, '12ce4590fabc8f29570acba283d2c27956714e49a61980938c8e7916c88fdf36'),
      (3, 'c882878e1ffd6a6baeda68fa1fff20ac2c3dca7f8821e0e950252c7596b3678d'))),
    ('f/msgWindow.tpl', 947396944, 321984,
     '2ef0cdc818d2402de5bfb95394f67d78017e74bc1b41c38b79780cb0b870dc67',
     ((0, '2e539b7635913420320ca2fcbc626ad29845f9150e38d4e6e53c639af7e1f769'),)),
)

def verify(iso=DEFAULT_ISO):
    gx = verify_gx(iso)
    entries = {v['path']: v for v in inventory(iso)['files']}
    with Path(iso).open('rb') as disc:
        disc.seek(0x420)
        dol, = struct.unpack('>I', disc.read(4))
        disc.seek(dol)
        header = disc.read(256)
        def read(address, size):
            offsets = []
            for i in range(18):
                off, base, length = [struct.unpack_from('>I', header, table+4*i)[0]
                                     for table in (0, 0x48, 0x90)]
                if base <= address and address+size <= base+length:
                    offsets.append(dol+off+address-base)
            if len(offsets) != 1:
                raise ValueError('Unmapped source span')
            disc.seek(offsets[0])
            return disc.read(size)
        address, size, digest = BODY
        body = read(address, size)
        if hashlib.sha256(body).hexdigest() != digest:
            raise ValueError('Whole TEX body differs')
        if read(0x802c0140, 8) != bytes.fromhex('4330000000000000'):
            raise ValueError('Original unsigned LOD conversion constant differs')
        calls = []
        for offset in range(0, size, 4):
            op, = struct.unpack_from('>I', body, offset)
            if op >> 26 == 18 and op & 1:
                displacement = op & 0x3fffffc
                if displacement & 0x2000000:
                    displacement -= 0x4000000
                calls.append((address+offset, (address+offset+displacement)&0xffffffff))
        if calls != [(0x80037ae0, 0x802b72b0), (0x80037b3c, 0x802b7544)]:
            raise ValueError('Original whole child graph differs')
        count = 0
        for name, offset, length, sha, images in FILES:
            if entries[name] != dict(path=name, offset=offset, size=length):
                raise ValueError('Original TPL location differs')
            disc.seek(offset)
            raw = disc.read(length)
            if hashlib.sha256(raw).hexdigest() != sha:
                raise ValueError('Original TPL bytes differ')
            table, = struct.unpack_from('>I', raw, 8)
            for index, expected in images:
                image, = struct.unpack_from('>I', raw, table+8*index)
                if hashlib.sha256(raw[image:image+36]).hexdigest() != expected:
                    raise ValueError('Original used image header differs')
                count += 1
    return dict(whole_tex_bodies=1, actual_sdk_children=gx['bodies'],
                source_unsigned_conversion_constant=1, original_tpls=2,
                original_image_headers=count,
                scope='CPU source object construction only; no window/GPU/Setup readiness')

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
