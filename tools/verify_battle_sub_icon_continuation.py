#!/usr/bin/env python3
"""Read-only pins for whole retained DrawSubIcon row geometry/gates."""
import argparse
import hashlib
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'DrawSubIcon': {'address': 2148634660,
                 'size': 388,
                 'sha256': '89b54ee51718eb237fc69d85e89b7ce0dcfbfa5a544a19c47b3caae3ad35f966',
                 'calls': [(2148634732, 2148501708),
                           (2148634924, 2150228528),
                           (2148634948, 2150227300),
                           (2148634992, 2148631676)]}}
SPANS = {'0x802ef780': {'address': 2150561664,
                'size': 8,
                'sha256': '07af8387624cf8f36152797d2f561246f21237d1d22b5c13006eacfa6f475e43'},
 '0x8042242c': {'address': 2151818284,
                'size': 4,
                'sha256': 'df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119'},
 '0x80422480': {'address': 2151818368,
                'size': 4,
                'sha256': 'f0c7ba19fc245f87a24a19dbabb209fbd07505bf56bcf6cf9428f4210c91ec75'},
 '0x804224f8': {'address': 2151818488,
                'size': 4,
                'sha256': '9dc1b62d60acff118f222dc22cb76a1c7e9f2fcc1ae7d16c56861eb0301bd59a'},
 '0x80422504': {'address': 2151818500,
                'size': 4,
                'sha256': '25daa80c2db5345b78a217e02b173cf202d3d9fe141e44c7a04828512fe28db4'}}

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
