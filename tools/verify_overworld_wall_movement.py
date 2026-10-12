#!/usr/bin/env python3
"""Pin read-only US overworld wall movement evidence; no runtime claim."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'marioCheckWall': (2148054928, 684, '381f8e9a74830a8b439e1cff534438b6e5307c674d30455ab2c16c6a18bb1d30'), 'searchFrontWall': (2148049648, 2404, '3f418f1f749ecfcbdb24418a0d10366d5d20ed71e851982280e1044d37ad34b5'), 'marioChkWallAround': (2148042960, 2460, '9c8ca1abc4669f95986c2fe9e7c6d24a4d8a56be648bebcc7d9f1b465d3d0b2e'), 'chkfilterVec': (2148079076, 224, '01322c33240e5ca06f5bbbafe7f3b533678c6a4b532895f1af2c08695c5019e7'), 'kpaGetStageViewType': (2149576376, 48, '5b65b91d65b06db3bf988fe1c9d6183ce50b89e20aa6e42af59eb44ad7cc49f8'), 'hitCheckVecFilter': (2147571876, 3332, 'b15fb470ab90907c5637b54cf7421c5a049182abc5f58e6473370e24d2e0904c'), 'marioSearchFrontWall_staying': (2148054232, 696, '5996749270c064224b2f1c2dc615ff9dac44a3cc5317e2b9f65c5739d776631c'), 'marioInit': (2147857864, 1160, 'e3976200ee5fc18a245b234f0b88c4095a0de3108c620034b54df1ea545ee6ce')}
CONSTANTS = {2151811240: '447a0000', 2151811252: '41300000', 2151811260: '42340000', 2151811268: '3f800000', 2151811272: '43b40000', 2151811276: '3f000000', 2151811280: 'bf000000', 2151811284: '43340000', 2151811288: '3e000000', 2151811292: '3e800000', 2151811308: '3dcccccd', 2151811372: '41d51eb9', 2151809160: '41a00000', 2151809216: '42140000', 2150383084: '4d4f424a5f48696464656e486174656e61426c6f636b0000', 2150383108: '4d4f424a5f48696464656e4261646765426c6f636b000000', 2150383132: '4d4f424a5f48696464656e3130436f756e74426c6f636b00'}
ANCHORS = {0x8001620c: '4c411382', 2148055184: '4bffea61', 2148055556: '4bffcecd', 2148055564: '4bffe489', 2148055116: '4bfffc8d', 2148050144: 'c02294ec', 2148050172: '3ba00003', 2148050244: 'efe006b2', 2148050264: 'efdfc02a', 2148050488: '4bf8b26d', 2148050952: 'fc1ad840', 2148051040: '4180fd48', 2148051084: 'ec03007a', 2148051088: 'ec20187c', 2148051092: 'ec00217c', 2148051120: 'ec20c82a', 2148051484: '40820014', 2148051500: '4180fef0', 2148051528: 'ec390028', 2148051840: '7c7a1b78', 2148051856: '4180fec8', 2148051872: '40820024', 2148051912: 'd0140000', 2148051940: 'ec18007a', 2148043076: '48176575', 2148043088: '3b600002', 2148043148: 'ef620824', 2148043196: 'ec01002a', 2148043876: '2c000000', 2148043964: '2c000000', 2148043992: 'fc1de040', 2148044056: 'ec1fe828', 2148044084: 'ec4410fa', 2148044140: 'ef800672', 2148044176: 'ef800672', 2148044184: 'fc1c0040', 2148044228: 'fc01e040', 2148044264: '48215d99', 2148045272: '48215aad', 2148045288: '4082000c', 2148045312: '482159c9', 2148045328: '48215971', 2147574168: '41820374', 2147574212: '28000001', 2147574288: '418202e8'}

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
    return dict(scope='Static US overworld wall movement; no execution/timing claim',
                bodies=len(BODIES), anchors=len(ANCHORS), constants=len(CONSTANTS))

if __name__ == '__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    args=parser.parse_args()
    print(json.dumps(verify(args.iso), sort_keys=True))
