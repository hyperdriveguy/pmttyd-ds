#!/usr/bin/env python3
"""Pin whole original GX texture-object constructors and their live source data."""
import argparse
import hashlib
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO

BODIES = (
    ('GXInitTexObj', 0x802B72B0, 0x24C, 'd3e3abf80473bd429cd7780ac301bd0514915035a7ddb80d274843777a63a827'),
    ('GXInitTexObjLOD', 0x802B7544, 0x164, 'd701772879c440c2af49bec024a9576a911a57c6140c7717de40e7425c2bb2f8'),
)
SPANS = (
    (0x803C4E84, '802b7430802b7444802b7444802b7458802b7458802b7458802b746c802b7494802b7430802b7444802b7458802b7494802b7494802b7494802b7480'),
    (0x8041E558, '0004010502060000'),
    (0x80429440, '41800000000000004330000000000000c080000040800000407f5c29420000000000000041200000'),
    (0x80005418, '3c40804260427440'),
    (0x800050B4, '9421fff07c0802a69001001493e1000c7c7f1b784800001d800100147fe3fb7883e1000c7c0803a6382100104e800020'),
)

def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as disc:
        if disc.read(8) != b'G8ME01\0\0':
            raise ValueError('US revision-0 disc required')
        disc.seek(0x420)
        dol = struct.unpack('>I', disc.read(4))[0]
        disc.seek(dol)
        header = disc.read(256)
        def read(address, size):
            offsets = []
            for index in range(18):
                offset, base, length = (struct.unpack_from('>I', header, table+4*index)[0]
                                        for table in (0, 0x48, 0x90))
                if base <= address and address+size <= base+length:
                    offsets.append(dol+offset+address-base)
            if len(offsets) != 1:
                raise ValueError(f'Unmapped/ambiguous initialized source span {address:#x}')
            disc.seek(offsets[0])
            data = disc.read(size)
            if len(data) != size:
                raise ValueError('Truncated source span')
            return data
        for name, address, size, digest in BODIES:
            if hashlib.sha256(read(address, size)).hexdigest() != digest:
                raise ValueError('Body mismatch: '+name)
        for address, literal in SPANS:
            expected = bytes.fromhex(literal)
            if read(address, len(expected)) != expected:
                raise ValueError(f'Literal mismatch: {address:#x}')
        # SDK uses the actual r13 base initialized next to r2, not a fresh table.
        if read(0x80005420, 8) != bytes.fromhex('3da0804161adcf20'):
            raise ValueError('Original r13 initialization changed')
        # Original Init's only BL is the complete memset child; LOD has none.
        for _, address, size, _ in BODIES:
            calls = []
            for offset in range(0, size, 4):
                word = struct.unpack('>I', read(address+offset, 4))[0]
                if word >> 26 == 18 and word & 1:
                    displacement = word & 0x3fffffc
                    if displacement & 0x2000000:
                        displacement -= 0x4000000
                    calls.append((address+offset+displacement) & 0xffffffff)
            if calls != ([0x800050B4] if address == 0x802B72B0 else []):
                raise ValueError('Original linked dependency graph changed')
    return dict(bodies=2, data_and_initializer_spans=len(SPANS)+1,
                scope='Whole Init/LOD, real jump/min-filter tables, initialized SDK scalars and r2/r13, linked memset; CPU object construction only')

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
