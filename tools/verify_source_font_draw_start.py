#!/usr/bin/env python3
"""Read-only pins for whole retained JUTFont draw-start source GX setup."""
import argparse
import hashlib
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'JUTFontDrawStart': {'address': 2147961852,
                      'size': 660,
                      'sha256': '25f77d0a340c245f326b7bc056bc1c1787345abe912d1aefee332c06f36e9cd3',
                      'calls': [(2147961876, 2147545828),
                                (2147961888, 2150336640),
                                (2147961896, 2150325388),
                                (2147961904, 2150339596),
                                (2147961928, 2150337888),
                                (2147961948, 2150339372),
                                (2147961964, 2150339544),
                                (2147961968, 2150319392),
                                (2147961980, 2150317064),
                                (2147961992, 2150317064),
                                (2147962004, 2150317064),
                                (2147962016, 2150317064),
                                (2147962040, 2150319448),
                                (2147962064, 2150319448),
                                (2147962088, 2150319448),
                                (2147962112, 2150319448),
                                (2147962144, 2150330232),
                                (2147962152, 2150330172),
                                (2147962160, 2150322308),
                                (2147962180, 2150337208),
                                (2147962188, 2150338508),
                                (2147962208, 2150338096),
                                (2147962236, 2150337000),
                                (2147962264, 2150337104),
                                (2147962288, 2150336864),
                                (2147962312, 2150336932),
                                (2147962324, 2150332544),
                                (2147962360, 2150228656),
                                (2147962384, 2150228656),
                                (2147962400, 2150343632),
                                (2147962428, 2150321668),
                                (2147962440, 2150343420),
                                (2147962448, 2150343580),
                                (2147962484, 2150338548)]}}
SPANS = {'0x804203e4': {'address': 2151810020,
                'size': 24,
                'sha256': '301aaa6301ebe70ed3d174d80e01e4ff4793d279102c160c7faebf8cc1c4cfcf'}}

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
        if struct.unpack_from('>II',header,0xd8)!=(0x803c5360,0x645ac):
            raise ValueError('Original DOL BSS ownership changed')
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
    return dict(whole_bodies=len(BODIES),original_calls=count,constant_spans=len(SPANS),source_bss=['803c5360','8042990c'],limitation='Static source pins, no executed-original or native display proof')

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
