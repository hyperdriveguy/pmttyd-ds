#!/usr/bin/env python3
"""Pin read-only US Mario direction movement-rate evidence; no runtime claim."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'marioGetMoveRate': (2148100516, 100, '8046b8e17615dc89022eb71502ad82237a7c9b7b8ccb3066c29df78760c3ddce'), 'marioPreCamera': (2149225356, 168, 'dd3237d45e9a988459bd00b9433f1df6fe1765bdd320af49a5219543cfc88f4d'), 'marioMoveMain': (2147849384, 1488, '2fb3744cded7847ad0c252487b8f5dfb2977f4d5aa395d0db30e565af06502ab'), 'marioMain': (2147854304, 2552, 'e7a4c22dbea9834233df4749dc96a6cf0687886f4792edc7c3fdf2b7331a52b8'), 'marioGetStick': (2148096760, 440, 'a6aba7a06277bc002c9b636e7cdc13e346b2f3a2c50d1ef6cedc01692778c741'), 'revise360': (2148099428, 52, '817781931eeb9c0fa55f1e7bdda521295e5f8c673bee13e2df988470851d6dcf')}
CONSTANTS = {2151811740: '40490fdb', 2151811696: '43340000', 2151811700: '3f800000', 2151811744: '3e428f60', 2151828132: '00000000', 2151828176: '3f000000', 2151828180: 'bf000000', 2151828184: '447a0000'}
ANCHORS = {2148100544: 'c003019c', 2148100552: 'ec5f0028', 2148100560: 'ec2100b2', 2148100564: 'ec210024', 2148100568: '481db4d1', 2148100572: 'fc200818', 2148100584: 'fc200a10', 2148100588: 'ec22007a', 2147850572: 'c03e0188', 2147850592: 'ffe00090', 2147850596: '546001cf', 2147850600: '40820010', 2147850604: 'c03e01a0', 2147850608: '4803d035', 2147850612: 'efff0072', 2147850632: 'd3fe018c', 2147854812: '4814e9b1', 2149225384: 'c0230114', 2149225396: '4c411382', 2149225424: 'c0030114', 2149225444: 'ec02f83a', 2149225452: 'fc00001e', 2149225480: 'ec201024', 2149225484: '4beed159', 2149225496: 'd3e3019c', 2148097108: 'c01f019c', 2148097116: 'ec21002a'}

def verify(iso):
    with Path(iso).open('rb') as disc:
        if disc.read(8) != b'G8ME01\0\0':
            raise ValueError('Expected US G8ME01 disc0 revision0')
        disc.seek(0x420)
        dol=struct.unpack('>I', disc.read(4))[0]
        disc.seek(dol)
        header=disc.read(256)
        def read(address, size):
            matches=[]
            for i in range(18):
                off, va, length=(struct.unpack_from('>I', header, j+i*4)[0]
                                 for j in (0, 0x48, 0x90))
                if va <= address and address+size <= va+length:
                    matches.append(dol+off+address-va)
            if len(matches) != 1:
                raise ValueError(f'Unmapped/ambiguous address {address:#x}')
            disc.seek(matches[0])
            data=disc.read(size)
            if len(data) != size:
                raise ValueError('Truncated DOL')
            return data
        for name, (address,size,digest) in BODIES.items():
            if hashlib.sha256(read(address,size)).hexdigest() != digest:
                raise ValueError(f'Body mismatch: {name}')
        for address,value in {**CONSTANTS, **ANCHORS}.items():
            if read(address,len(value)//2).hex() != value:
                raise ValueError(f'Anchor/constant mismatch: {address:#x}')
    return dict(scope='Static US Mario direction movement rate; no execution/timing claim',
                bodies=len(BODIES), anchors=len(ANCHORS), constants=len(CONSTANTS))

if __name__ == '__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    args=parser.parse_args()
    print(json.dumps(verify(args.iso), sort_keys=True))
