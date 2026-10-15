#!/usr/bin/env python3
"""Read-only pins for whole retained DrawSubIconSub raster parent."""
import argparse
import hashlib
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'DrawSubIconSub': {'address': 2148631676,
                    'size': 2984,
                    'sha256': '8c5d0788e75717f32fee346e0885d720f194c7ac7084d405c00bea5be14822e9',
                    'calls': [(2148631776, 2147545852),
                              (2148632008, 2148547092),
                              (2148632024, 2150227300),
                              (2148632036, 2150343420),
                              (2148632064, 2147711616),
                              (2148632076, 2150332544),
                              (2148632112, 2148546500),
                              (2148632148, 2148546500),
                              (2148632176, 2147711616),
                              (2148632188, 2150332544),
                              (2148632224, 2148546500),
                              (2148632244, 2150228528),
                              (2148632264, 2150228656),
                              (2148632280, 2150227300),
                              (2148632296, 2150227300),
                              (2148632356, 2147591356),
                              (2148632400, 2147591356),
                              (2148632404, 2147973964),
                              (2148632472, 2147973088),
                              (2148632492, 2150228528),
                              (2148632508, 2147964200),
                              (2148632528, 2147964200),
                              (2148632588, 2150228656),
                              (2148632612, 2150228656),
                              (2148632628, 2150227300),
                              (2148632644, 2150227300),
                              (2148632656, 2147970268),
                              (2148632828, 2150228656),
                              (2148632932, 2150228528),
                              (2148632948, 2150227300),
                              (2148632964, 2150227300),
                              (2148632976, 2147970268),
                              (2148633012, 2150228528),
                              (2148633032, 2150228656),
                              (2148633048, 2150227300),
                              (2148633064, 2150227300),
                              (2148633072, 2148014988),
                              (2148633084, 2147970268),
                              (2148633256, 2150228656),
                              (2148633360, 2150228528),
                              (2148633376, 2150227300),
                              (2148633392, 2150227300),
                              (2148633404, 2147970268),
                              (2148633440, 2150228528),
                              (2148633460, 2150228656),
                              (2148633476, 2150227300),
                              (2148633492, 2150227300),
                              (2148633500, 2148014988),
                              (2148633512, 2147970268),
                              (2148633576, 2150228528),
                              (2148633596, 2150228656),
                              (2148633612, 2150227300),
                              (2148633628, 2150227300),
                              (2148633636, 2148014988),
                              (2148633648, 2147970268),
                              (2148633812, 2150228656),
                              (2148633932, 2150228528),
                              (2148634004, 2150228528),
                              (2148634020, 2150227300),
                              (2148634036, 2150227300),
                              (2148634048, 2147970268),
                              (2148634100, 2150228528),
                              (2148634124, 2150228528),
                              (2148634144, 2150228656),
                              (2148634160, 2150227300),
                              (2148634176, 2150227300),
                              (2148634184, 2148014988),
                              (2148634196, 2147970268),
                              (2148634360, 2150228656),
                              (2148634480, 2150228528),
                              (2148634552, 2150228528),
                              (2148634568, 2150227300),
                              (2148634584, 2150227300),
                              (2148634596, 2147970268)]}}
SPANS = {'0x802ef780': {'address': 2150561664,
                'size': 8,
                'sha256': '07af8387624cf8f36152797d2f561246f21237d1d22b5c13006eacfa6f475e43'},
 '0x802ef808': {'address': 2150561800,
                'size': 8,
                'sha256': '2b2979f362d94c89fe17a6e01637f14332de063fe7a544bdc5facfc046f4169f'},
 '0x80422418': {'address': 2151818264,
                'size': 240,
                'sha256': '86b5800456569246f818c55205461b55ccc2f321468a08df07ebf0909d74140b'},
 '0x802ef7c0': {'address': 2150561728,
                'size': 72,
                'sha256': '67a66c7f6cfcd53735b92abff57cac17cc3645620faa0b5679c5edd8d6d00d6f'},
 '0x80416d20': {'address': 2151771424,
                'size': 3,
                'sha256': '0fae9707ba33ffbe05d71cdc1566fea81ebb674b80042ed279f5f32eb8e411dd'}}

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
