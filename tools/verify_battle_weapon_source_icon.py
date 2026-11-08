#!/usr/bin/env python3
"""Pin original US Icon Matrix/GX bodies and exact source SDK object dimension getters."""
import argparse
import hashlib
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_source_icon_setup import verify as verify_setup
from verify_source_palette_texture_object import verify as verify_palette
BODIES = {2147588088: {'size': 2468, 'sha256': 'bb9430d4030d22982f813be3bc530403e5a1fd8c24c1ddd9298ff94b1e205848', 'calls': [['0x800198a0', '0x8029e2b0'], ['0x800198b0', '0x8029dd64'], ['0x800198b8', '0x8000f2fc'], ['0x800198c0', '0x8000f2e4'], ['0x800198e8', '0x8029e2b0'], ['0x800198f8', '0x8029dd64'], ['0x80019904', '0x802ba2fc'], ['0x8001990c', '0x802ba39c'], ['0x8001994c', '0x802b72b0'], ['0x8001995c', '0x80037a80'], ['0x80019984', '0x802b7544'], ['0x80019990', '0x802b7880'], ['0x800199c0', '0x800208c0'], ['0x800199cc', '0x802b6f3c'], ['0x800199e0', '0x802b6e54'], ['0x80019a00', '0x802b6f78'], ['0x80019a14', '0x802b8fcc'], ['0x80019a2c', '0x802b8ce0'], ['0x80019a44', '0x802b8ce0'], ['0x80019a5c', '0x802b8ce0'], ['0x80019a6c', '0x802b8c98'], ['0x80019a7c', '0x802b8c98'], ['0x80019a8c', '0x802b8c98'], ['0x80019aa0', '0x802b8ab8'], ['0x80019ab4', '0x802b8ab8'], ['0x80019ac8', '0x802b8e30'], ['0x80019ae4', '0x802b89e8'], ['0x80019b00', '0x802b8a50'], ['0x80019b18', '0x802b8960'], ['0x80019b30', '0x802b89a4'], ['0x80019b44', '0x802b8e30'], ['0x80019b60', '0x802b89e8'], ['0x80019b7c', '0x802b8a50'], ['0x80019b94', '0x802b8960'], ['0x80019bac', '0x802b89a4'], ['0x80019bc0', '0x802b8e30'], ['0x80019bdc', '0x802b89e8'], ['0x80019bf8', '0x802b8a50'], ['0x80019c10', '0x802b8960'], ['0x80019c28', '0x802b89a4'], ['0x80019c30', '0x802b5084'], ['0x80019c4c', '0x802b4e04'], ['0x80019c58', '0x802b8fcc'], ['0x80019c6c', '0x802b8e30'], ['0x80019c78', '0x802b88d4'], ['0x80019c88', '0x802b8c98'], ['0x80019c9c', '0x802b8ab8'], ['0x80019cb0', '0x802b8e30'], ['0x80019ccc', '0x802b89e8'], ['0x80019ce8', '0x802b8a50'], ['0x80019d00', '0x802b8960'], ['0x80019d18', '0x802b89a4'], ['0x80019d38', '0x8002b438'], ['0x80019d4c', '0x8001d968'], ['0x80019d60', '0x802b8ab8'], ['0x80019d68', '0x802b8fcc'], ['0x80019d7c', '0x802b8e30'], ['0x80019d98', '0x802b89e8'], ['0x80019db4', '0x802b8a50'], ['0x80019dcc', '0x802b8960'], ['0x80019de4', '0x802b89a4'], ['0x80019dec', '0x802b5084'], ['0x80019e08', '0x802b4e04'], ['0x80019e18', '0x800217c8'], ['0x80019e40', '0x802b8ff4'], ['0x80019e60', '0x802b932c'], ['0x80019e68', '0x802b940c'], ['0x80019e80', '0x802b8d60'], ['0x80019e90', '0x802b93d8'], ['0x80019ea8', '0x802b932c'], ['0x80019eb0', '0x802b940c'], ['0x80019ec8', '0x802b8d60'], ['0x80019ed8', '0x802b93d8'], ['0x80019ef4', '0x802b93d8'], ['0x80019f08', '0x802b5c8c'], ['0x80019f1c', '0x802b5c8c'], ['0x80019f28', '0x802b5c8c'], ['0x80019f2c', '0x802b4520'], ['0x80019f38', '0x802b3c08'], ['0x80019f44', '0x802b3c08'], ['0x80019f5c', '0x802b4558'], ['0x80019f74', '0x802b4558'], ['0x80019f94', '0x802b76c4'], ['0x80019fc0', '0x802b76d4'], ['0x8001a098', '0x802b5a6c'], ['0x8001a140', '0x802b8c98'], ['0x8001a150', '0x802b8c98'], ['0x8001a160', '0x802b8c98']]}, 2147591356: {'size': 548, 'sha256': '76ff3611a63baa74f3f6f8ff9490ffe72637b5649b125c839d0767624b3233dd', 'calls': [['0x8001a4dc', '0x8000f2e4'], ['0x8001a6bc', '0x8029dd64'], ['0x8001a6c8', '0x800197f8']]}, 2147591904: {'size': 568, 'sha256': '81d66a9b10d3353c8dd6a9f1ffda9a90cadb7674fc599299f27707a27c1c3446', 'calls': [['0x8001a708', '0x8000f2e4'], ['0x8001a8e8', '0x8029dd64'], ['0x8001a8f4', '0x800197f8']]}, 2147592472: {'size': 36, 'sha256': '140c49735d09568fc933f8eca36b6fc8788f0878ac8b5ecf60f19e3079b3041d', 'calls': [['0x8001a928', '0x8001a93c']]}, 2147592508: {'size': 692, 'sha256': '7bbfb1d2eb61d2993d0474a863c0fde0f10110daf4e1250b8dc5d3bcb086ba0e', 'calls': [['0x8001a968', '0x8000f2e4'], ['0x8001a9a0', '0x8029e230'], ['0x8001a9b4', '0x8029e2b0'], ['0x8001a9d0', '0x8029dff0'], ['0x8001a9e0', '0x8029dd64'], ['0x8001a9f0', '0x8029dd64'], ['0x8001aa00', '0x8029dd64'], ['0x8001abd0', '0x800197f8']]}, 2150332100: {'size': 16, 'sha256': '1d5a0d47f100f343d987520897dd27ea318022251c09585841c9bda8e96e90ab', 'calls': []}, 2150332116: {'size': 16, 'sha256': '7b7b8302f957eb41d66023825238ca9f45caf47502d52f4630ccacf10d9b6b7e', 'calls': []}}

def verify(iso=DEFAULT_ISO):
    verify_setup(iso)
    verify_palette(iso)
    with Path(iso).open('rb') as disc:
        if disc.read(8)!=b'G8ME01\0\0':
            raise ValueError('US revision-0 disc required')
        disc.seek(0x420)
        dol,=struct.unpack('>I',disc.read(4))
        disc.seek(dol)
        header=disc.read(256)
        def read(address,size):
            offsets=[]
            for i in range(18):
                off,base,n=[struct.unpack_from('>I',header,t+4*i)[0] for t in (0,0x48,0x90)]
                if base<=address and address+size<=base+n:
                    offsets.append(dol+off+address-base)
            if len(offsets)!=1:
                raise ValueError('Unmapped/ambiguous initialized source span')
            disc.seek(offsets[0])
            b=disc.read(size)
            if len(b)!=size:
                raise ValueError('Truncated original source span')
            return b
        def calls(base,raw):
            result=[]
            for offset in range(0,len(raw),4):
                word,=struct.unpack_from('>I',raw,offset)
                if word>>26==18 and word&1:
                    displacement=word&0x3fffffc
                    if displacement&0x2000000:
                        displacement-=0x4000000
                    result.append([hex(base+offset),hex((base+offset+displacement)&0xffffffff)])
            return result
        for address,expected in BODIES.items():
            raw=read(address,expected['size'])
            if hashlib.sha256(raw).hexdigest()!=expected['sha256'] or calls(address,raw)!=expected['calls']:
                raise ValueError(f'Original whole body/calls changed: {address:#x}')
    return dict(whole_matrix_gx_bodies=5, whole_sdk_dimension_getters=2,
        linked_actual_icon_setup_unpack_tex_gx=True,
        scope='Original source bodies pinned; native finite bridge, not PPC/GX submission equivalence')

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
