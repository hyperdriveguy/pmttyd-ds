#!/usr/bin/env python3
"""Check US instruction/data anchors cited by docs/behavior.

These are static evidence checks, not original gameplay observation or proof
that a port's implementation is equivalent. Never executes original code.
"""
import argparse
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO


# Independent expected instructions transcribed from the inspected US functions.
ANCHORS = {
    'defence': {
        0x800fa200: '88a30306', 0x800fa204: '88030305',
        0x800fa208: '39650003', 0x800fa210: '7d605851',
        0x800fa238: '88040013', 0x800fa254: 'a803010c',
        0x800fa260: 'a89f1d18', 0x800fa284: '88030307',
        0x800fa2ac: '38800200', 0x800fa2dc: '38800100',
        0x800fa30c: '2c1b000f', 0x800fa314: '2c1a0002',
        0x800fa394: '2c1b0002', 0x800fa404: '4801ff89',
    },
    'pad': {
        0x8011a0b0: '3800001d', 0x8011a0b8: '38a00070',
        0x8011a0d8: '80060018', 0x8011a0dc: '9006001c',
        0x8011a1b4: '2c00001e', 0x8011a1d0: '2c00ffe2',
        0x8011a314: '38800008', 0x8011a338: '38800010',
    },
    'event': {
        0x80037a10: '7c060040', 0x80037a14: '4080001c',
        0x80036f78: '60000010', 0x800371e8: '900d193c',
        0x800367b4: '90040070', 0x800367b8: '801f009c',
        0x8003684c: '900400e4', 0x80036894: '38a001b0',
    },
    'saved_work': {
        0x80085838: '80630174', 0x80085848: '88630578',
        0x8008585c: '90830174', 0x8008586c: '98830578',
        0x80085948: '38a00040', 0x80085964: '38a00400',
        0x8008598c: '38a00400', 0x800859a8: '38a00800',
    },
    'pouch': {
        0x800d3d04: 'b0640070', 0x800d3d0c: 'a8030070',
        0x800d3ca4: '1ca3000e', 0x800d3bd8: '4080000c',
        0x800d3e24: 'b0040078', 0x800d3e48: '2c0003e7',
        0x800d561c: '38600001', 0x800d562c: '3800000a',
        0x800d5634: '38000014', 0x800d564c: 'a8040192',
    },
    'rng': {
        0x8026c324: '3c6041c6', 0x8026c32c: '38034e6d',
        0x8026c334: '38033039', 0x8026c33c: '5403847e',
        0x80005db4: '4082000c', 0x80005dc0: '48266565',
        0x80005dc4: '7c03fbd6', 0x80005dcc: '7c601850',
    },
}


def verify(iso_path):
    with Path(iso_path).open('rb') as disc:
        if disc.read(8) != b'G8ME01\0\0':
            raise ValueError('Expected G8ME01 disc 0 revision 0')
        disc.seek(0x420)
        base = struct.unpack('>I', disc.read(4))[0]
        disc.seek(base)
        header = disc.read(0x100)
        def read(address, size):
            matches = []
            for i in range(18):
                off, va, length = (struct.unpack_from('>I', header, j + i * 4)[0]
                                   for j in (0, 0x48, 0x90))
                if va <= address and address + size <= va + length:
                    matches.append(base + off + address - va)
            if len(matches) != 1:
                raise ValueError(f'Unmapped/ambiguous address {address:#x}')
            disc.seek(matches[0])
            value = disc.read(size)
            if len(value) != size:
                raise ValueError('Truncated binary')
            return value
        for group, anchors in ANCHORS.items():
            for address, expected in anchors.items():
                actual = read(address, 4).hex()
                if actual != expected:
                    raise ValueError(f'{group}: {address:#x}: {actual} != {expected}')
            print(f'{group}: {len(anchors)} instruction anchors verified')
        for address, expected in ((0x802ee018, [10, 9, 8, 8, 7, 6, 5]),
                                  (0x802ee020, [5, 4, 3, 3, 2, 1, 1])):
            if list(read(address, 7)) != expected:
                raise ValueError(f'Defence table mismatch at {address:#x}')
        print('Both seven-entry defence window tables verified')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    verify(parser.parse_args().iso)
