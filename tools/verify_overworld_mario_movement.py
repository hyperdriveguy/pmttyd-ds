#!/usr/bin/env python3
"""Pin read-only US overworld Mario local movement evidence; no runtime claim."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'marioJump': (0x80093d38, 0x2e8, '8161141f6446500f7b852be87d72f6006c47a267e0b5b7e85aa23c12c4d9ecfb'), 'marioMain': (2147854304, 2552, 'e7a4c22dbea9834233df4749dc96a6cf0687886f4792edc7c3fdf2b7331a52b8'), 'marioMoveMain': (2147849384, 1488, '2fb3744cded7847ad0c252487b8f5dfb2977f4d5aa395d0db30e565af06502ab'), 'marioInit': (2147857864, 1160, 'e3976200ee5fc18a245b234f0b88c4095a0de3108c620034b54df1ea545ee6ce'), 'marioMotion': (2148091420, 100, 'f86c292be913bbb599c8db5db59d795324f2c715ca738071ee6227b8ccefd1f1'), 'marioChgMot': (2148091228, 192, 'cb0d860ff6a94e5e6d0da5dded999fef1e6b881c2169730ce9d09c7f0439da26'), 'marioGetStick': (2148096760, 440, 'a6aba7a06277bc002c9b636e7cdc13e346b2f3a2c50d1ef6cedc01692778c741'), 'marioUpdateKeyData': (2148097200, 480, 'db6e3882e60cc9de3b1bc7aef328126769532d7db38c8fde1877f2b342ea0ab0'), 'mot_walk': (2148039060, 848, '579debb26484eea74504b0617ed05fda92e5e9655d4deccce9fb23f2683625a8'), 'mot_dash': (2148038204, 856, '7b316ca67080614da3bfaedc2eb097079c48ff4f4d3caf20a44eac5253cf9907'), 'marioGetWalkSpd': (2148040068, 156, '44b48a29b00754fa795bd2d8e3c9cea81dc97d4748e823e00aefb93c68f369ef'), 'marioGetDashSpd': (2148039908, 160, 'bf4e93869b303f771d68395708b2c7252cb4aaf89c037df3f9cf386beef94b4c'), 'mot_jump': (2148119680, 520, '394fdc33e6fcae45fae7866f5808ea1fa58699aff38428318ebe3a2329587853'), 'mot_fall': (2148118300, 572, '8e5bbb5e7348f6d18fb5434813688fba54fd905eca28f5d92f0cc21827bc533a'), 'mot_stay': (2148166736, 1952, '069e5535b3ccbcfd7ab9decef8927617d6f87731f7fdeb862ef7db9960322d04'), 'marioChgMotAuto': (2148166588, 148, '70489eaf4c37a8f4b7d802384f37ecd7cb3d685d6f417b767bde91bda5d97d0c')}
CONSTANTS = {0x80420008: '3f800000', 0x804200b8: '40100000',
             0x804200a4: '3c75c28f', 0x804200ac: '3e800000',
             0x80420a64: '428c0000', 0x8042088c: '3f000000'}
ANCHORS = {0x80093d84: '801e0044', 0x80093d88: '2c00000a', 0x80093d8c: '41800274', 0x80093dfc: 'a81e0050', 0x80093e00: '2c000004', 0x8005abdc: '4803b0d5', 0x8005abe8: '4803af11',
           0x8005abf0: '48039a2d', 0x8005affc: '48038d3d',
           0x8005b004: '4bffea75', 0x8005b0cc: '4bffe9ad',
           0x80094664: '4082ffe4', 0x8009b538: 'c01e0180',
           0x8009b53c: 'd01e0190', 0x80087910: '2c000bd1',
           0x80087c38: '2c000bd1', 0x800596d4: 'efbd0032',
           0x80059958: 'fc1f0040'}

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
    return dict(scope='Static US overworld Mario movement; no execution/timing claim',
                bodies=len(BODIES), anchors=len(ANCHORS), constants=len(CONSTANTS))

if __name__ == '__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    args=parser.parse_args()
    print(json.dumps(verify(args.iso), sort_keys=True))
