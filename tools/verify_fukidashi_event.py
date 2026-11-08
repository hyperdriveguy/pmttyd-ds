#!/usr/bin/env python3
"""Verify original US fukidashi event body and its initialized constants read-only."""
import argparse
import hashlib
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

SPANS = (
    ('r2+r13 startup', 0x80005418, 0x10, '76a62b3dba4a365727ca04077b196ca71058a5e7c9fd484abfe1ad0eeb44bc32'),
    ('event', 0x8006c3c4, 0x314, '1784acdf35150f3076e423445bba0b013733d2d49e15fd389f49fb35872ee287'),
    ('0x8ee8', 0x80420328, 0x4, 'c31425b44a7ed682a3356bf6820141e1584a15c43b6d6793f552e5d4621586f7'),
    ('0x8eec', 0x8042032c, 0x4, '638354a0b185ec05318029e52610c8de48d33523e2b4854e71154187583d1037'),
    ('0x8ef0', 0x80420330, 0x4, 'df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119'),
    ('0x8ef4', 0x80420334, 0x4, '4bdb347a5703ee47ed0bab05bf452f4b3c693bcae8ecc0ebdf7751db42d6503c'),
    ('0x8ef8', 0x80420338, 0x4, '964bc35d520d851571318cf74d05c759824d6ff4c949dc8cf9160a989b72da35'),
    ('0x8efc', 0x8042033c, 0x4, 'df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119'),
)

def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as disc:
        if disc.read(8) != b'G8ME01\0\0':
            raise ValueError('US G8ME01 disc0 revision0 required')
        disc.seek(0x420)
        dol, = struct.unpack('>I', disc.read(4))
        disc.seek(dol)
        header = disc.read(256)
        sections = [tuple(struct.unpack_from('>I', header, k + 4*i)[0]
                          for k in (0, 0x48, 0x90)) for i in range(18)]
        for name, address, size, expected in SPANS:
            offsets = [dol+off+address-base for off, base, length in sections
                       if base <= address and address+size <= base+length]
            if len(offsets) != 1:
                raise ValueError('Unmapped or ambiguous original span: '+name)
            disc.seek(offsets[0])
            data = disc.read(size)
            if len(data) != size or hashlib.sha256(data).hexdigest() != expected:
                raise ValueError('Original source changed: '+name)
    return dict(source='US G8ME01 disc0 revision0', spans=len(SPANS),
                original_executed=False,
                scope='Whole event body and startup/SDA2 constants; children remain separate owners')

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
