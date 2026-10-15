#!/usr/bin/env python3
"""Read-only pins for whole matrix-font/helper source and finite glyph reference."""
import argparse
import hashlib
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO, inventory

BODIES = {'FontDrawStringMtx': {'address': 2147970268,
                       'size': 856,
                       'sha256': 'a032812d7a65ebc7a525e4362ce77b34f195d12ef34887b2f2f6619dffe7837d',
                       'calls': [(2147970308, 2147545828),
                                 (2147970332, 2150227300),
                                 (2147970344, 2150343420),
                                 (2147970364, 2148014912),
                                 (2147970380, 2147962512),
                                 (2147970460, 2148014752),
                                 (2147970528, 2147962512),
                                 (2147970644, 2150337208),
                                 (2147970696, 2147960608),
                                 (2147970716, 2147960608),
                                 (2147970768, 2147960608),
                                 (2147970788, 2147960608),
                                 (2147970864, 2150337208),
                                 (2147970896, 2147960608)]},
 'LeadPredicate': {'address': 2148014912,
                   'size': 76,
                   'sha256': '6b1cc7be1b9b25190052866295ae20aafce435db8822851694bbe3efb5cf4a39',
                   'calls': []},
 'Command': {'address': 2148014752,
             'size': 160,
             'sha256': '9849799a9225a52cc9a1cd0c950302ddc4403f5deb17bb9590fecc64b63d9484',
             'calls': []},
 'CodeToGlyph': {'address': 2147962512,
                 'size': 372,
                 'sha256': 'c5c62aec2542e0ef775597654d52cb05d0afb5f15614f32eaa546a8abb4c8e30',
                 'calls': []},
 'JutGlyphPrimitiveFiniteReference': {'address': 2147960608,
                                      'size': 1244,
                                      'sha256': '3451ba272eb96ac7e825eb96ec3c1bb8887f2b21a603fbdd9a24ad9c60ccd6e9',
                                      'calls': [(2147961016, 2150324844),
                                                (2147961024, 2147507616),
                                                (2147961036, 2147507616),
                                                (2147961216, 2147963644),
                                                (2147961336, 2147963644),
                                                (2147961544, 2147963644),
                                                (2147961732, 2147963644)]}}
SPANS = {'0x804203f8': {'address': 2151810040,
                'size': 4,
                'sha256': 'df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119'},
 '0x80420434': {'address': 2151810100,
                'size': 4,
                'sha256': '1649baee94cb608ff716e671e816a19ab8ad9a00d9de3e60d826a3503ada2cce'},
 '0x802c2b98': {'address': 2150378392,
                'size': 8,
                'sha256': '07af8387624cf8f36152797d2f561246f21237d1d22b5c13006eacfa6f475e43'},
 '0x80416010': {'address': 2151768080,
                'size': 40,
                'sha256': '13c6d2d43193c660176e94f4d11679d5abb13515954e95d6f9dc5d4a59737562'}}

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
    entry=next(row for row in inventory(Path(iso))['files'] if row['path']=='f/papermarioset_US.bfn')
    with Path(iso).open('rb') as disc:
        disc.seek(entry['offset'])
        original=disc.read(entry['size'])
    bfn='bb9cf410e5d916a1268ff503fc4f1ad7e8630b381cd781958eb0be32abb56b45'
    if hashlib.sha256(original).hexdigest()!=bfn:
        raise ValueError('Original US BFN changed')
    return dict(whole_bodies=len(BODIES),original_calls=count,constant_spans=len(SPANS),original_bfn_sha256=bfn,limitation='Static source pins, no executed-original or native display proof')

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
