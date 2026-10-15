#!/usr/bin/env python3
"""Read-only original queued class callback/body pins for native composition."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO
from verify_battle_act_draw import verify as verify_ring
SPANS = [
    (0x8012210c, 0x40, 'a2b99a5291b5544d7e39394951c5d859b0b98cd6a0aa9adfb8e580963c6d01d2'),
    (0x80116cb8, 0x20, '71d3cef1972fb39853fe2a1ba50623b092d6f8f3d97a0c33e544f50e97917631'),
]

def verify(iso=DEFAULT_ISO):
    ring = verify_ring(iso)
    with Path(iso).open('rb') as disc:
        disc.seek(0x420)
        dol, = struct.unpack('>I', disc.read(4))
        disc.seek(dol)
        header = disc.read(256)
        for address, size, expected in SPANS:
            offsets = []
            for i in range(18):
                offset, base, length = [struct.unpack_from('>I', header, k + 4*i)[0]
                                        for k in (0, 0x48, 0x90)]
                if base <= address and address + size <= base + length:
                    offsets.append(dol + offset + address - base)
            if len(offsets) != 1:
                raise ValueError('Unmapped/ambiguous original source body')
            disc.seek(offsets[0])
            if hashlib.sha256(disc.read(size)).hexdigest() != expected:
                raise ValueError('Original queued callback body mismatch')
    return dict(source='Read-only original US G8ME01 revision0',
                queued_bodies=[dict(address=hex(a), size=n, sha256=h) for a,n,h in SPANS],
                reused_ring=ring,
                limitation='Source pins; native ownership is a port policy, no original execution/pixel parity claim')

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    print(json.dumps(verify(parser.parse_args().iso), indent=2))
