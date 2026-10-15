#!/usr/bin/env python3
"""Read-only pins for whole retained queued weapon display/Common/Sub1 graph."""
import argparse
import hashlib
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'QueuedWeapon': {'address': 2148670696,
                  'size': 36,
                  'sha256': '97138d6bead01355951de44e594dc198f0e1946ee2465213118a855391c628ea',
                  'calls': [(2148670712, 2148622412)]},
 'WeaponDisp': {'address': 2148622412,
                'size': 32,
                'sha256': '87c489cd3e3253a44b8c3a59b3ff05d4a101cf9d9f2852eb43a7bccf025cffb4',
                'calls': [(2148622424, 2148621740)]},
 'DrawWeaponWin': {'address': 2148621740,
                   'size': 608,
                   'sha256': '1c7b9d07be84ebe56c4819b3e9b9a08d9e04f742f2f14ba54f4b4ff99a89919c',
                   'calls': [(2148621780, 2148501708),
                             (2148621828, 2148626048),
                             (2148621956, 2148690280),
                             (2148622068, 2148637372),
                             (2148622112, 2148634660),
                             (2148622152, 2148629100),
                             (2148622324, 2148629532)]},
 'DrawSubMenuCommon': {'address': 2148626048,
                       'size': 1948,
                       'sha256': '6cb20828d8b3706e573f55e2a2a1ef8200683601239ea38b7115c109a5431a0b',
                       'calls': [(2148626116, 2148501708),
                                 (2148626512, 2148627996),
                                 (2148626580, 2148627996),
                                 (2148626720, 2148627996),
                                 (2148626836, 2148637936),
                                 (2148626876, 2147511436),
                                 (2148626920, 2147511436),
                                 (2148626952, 2150228528),
                                 (2148626992, 2147511436),
                                 (2148627140, 2150228528),
                                 (2148627248, 2148637936),
                                 (2148627264, 2148637820),
                                 (2148627392, 2150228656),
                                 (2148627412, 2150228528),
                                 (2148627428, 2150227300),
                                 (2148627652, 2147511436),
                                 (2148627684, 2150227952),
                                 (2148627780, 2150227204),
                                 (2148627876, 2150227204),
                                 (2148627900, 2147511436)]},
 'DrawSubMenuCommonSub1': {'address': 2148627996,
                           'size': 580,
                           'sha256': '945afcb36d9cfd3db9f56f041c6b5d7095941d62d70ef4ec844ca260a7049ddf',
                           'calls': []}}
SPANS = {'0x803636e8': {'address': 2151036648,
                'size': 164,
                'sha256': '5f73217cccca0bf0b5004f5b199f38d534a1bb4a0013c34ac9432a3040a774fb'},
 '0x80363644': {'address': 2151036484,
                'size': 164,
                'sha256': 'a56ac057035e690aa4a4cfcbf534f585d61201ae14d4a32200b60fc1424a3f16'},
 '0x803635d8': {'address': 2151036376,
                'size': 100,
                'sha256': 'a6c32aec9492f82dd16d3c172debb12c968893ee227499bb87b8e3d84c665e45'},
 '0x802ef750': {'address': 2150561616,
                'size': 12,
                'sha256': '3195a0c447fdbcaa4e2686211e8e79025236c8cc5ff773b42200114aab58bbee'},
 '0x802ef780': {'address': 2150561664,
                'size': 8,
                'sha256': '07af8387624cf8f36152797d2f561246f21237d1d22b5c13006eacfa6f475e43'},
 '0x8042242c': {'address': 2151818284,
                'size': 64,
                'sha256': '1cb4a16e5e8a43412980b9ffbc4aee39e5c122e0b6c67a1506c2dc872dc5c084'}}

def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as disc:
        if disc.read(8)!=b'G8ME01\0\0':
            raise ValueError('US G8ME01 disc0 revision0 required')
        disc.seek(0x420)
        dol=struct.unpack('>I',disc.read(4))[0]
        disc.seek(dol)
        header=disc.read(256)
        def read(address,size):
            spans=[]
            for i in range(18):
                off,va,n=(struct.unpack_from('>I',header,b+4*i)[0] for b in (0,0x48,0x90))
                if off and va<=address and address+size<=va+n:
                    spans.append(dol+off+address-va)
            if len(spans)!=1:
                raise ValueError('Unmapped/ambiguous source')
            disc.seek(spans[0])
            raw=disc.read(size)
            if len(raw)!=size:
                raise ValueError('Truncated source')
            return raw
        count=0
        for name,expected in BODIES.items():
            raw=read(expected['address'],expected['size'])
            if hashlib.sha256(raw).hexdigest()!=expected['sha256']:
                raise ValueError('Whole original body changed: '+name)
            calls=[]
            for i in range(0,len(raw),4):
                word=struct.unpack_from('>I',raw,i)[0]
                if word & 0xfc000003 == 0x48000001:
                    relative=word & 0x03fffffc
                    if relative & 0x02000000:
                        relative-=0x04000000
                    calls.append((expected['address']+i,(expected['address']+i+relative)&0xffffffff))
            if calls!=expected['calls']:
                raise ValueError('Original whole child census changed: '+name)
            count+=len(calls)
        for name, span in SPANS.items():
            if hashlib.sha256(read(span['address'],span['size'])).hexdigest()!=span['sha256']:
                raise ValueError('Original table/constants changed: '+name)
    return dict(whole_bodies=len(BODIES),original_calls=count,constant_spans=len(SPANS),limitation='Static source pins, no executed-original or native display proof')

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
