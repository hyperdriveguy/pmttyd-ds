#!/usr/bin/env python3
"""Read-only pins for whole centered string-font/helper source and finite glyph reference."""
import argparse
import hashlib
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO, inventory

BODIES = {'FontDrawString': {'address': 2147969552, 'size': 716, 'sha256': 'bbc6b1a8d82943f18d93ab656609347b49a7b148a8194a1438cac167b3f82df4', 'calls': [(2147969592, 2147545828), (2147969608, 2147964236), (2147969668, 2150227300), (2147969680, 2150343420), (2147969712, 2148014912), (2147969728, 2147962512), (2147969752, 2147962512), (2147969856, 2150337208), (2147969892, 2147960608), (2147969912, 2147960608), (2147969944, 2147960608), (2147969960, 2147960608), (2147970036, 2150337208), (2147970060, 2147960608)]}, 'LeadPredicate': {'address': 2148014912, 'size': 76, 'sha256': '6b1cc7be1b9b25190052866295ae20aafce435db8822851694bbe3efb5cf4a39', 'calls': []}, 'CodeToGlyph': {'address': 2147962512, 'size': 372, 'sha256': 'c5c62aec2542e0ef775597654d52cb05d0afb5f15614f32eaa546a8abb4c8e30', 'calls': []}, 'JutGlyphPrimitiveFiniteReference': {'address': 2147960608, 'size': 1244, 'sha256': '3451ba272eb96ac7e825eb96ec3c1bb8887f2b21a603fbdd9a24ad9c60ccd6e9', 'calls': [(2147961016, 2150324844), (2147961024, 2147507616), (2147961036, 2147507616), (2147961216, 2147963644), (2147961336, 2147963644), (2147961544, 2147963644), (2147961732, 2147963644)]}}
SPANS = {'0x80416010': {'address': 2151768080, 'size': 40, 'sha256': '13c6d2d43193c660176e94f4d11679d5abb13515954e95d6f9dc5d4a59737562'}, '0x8042041c': {'address': 2151810076, 'size': 4, 'sha256': '1b9e0fe7b740b4e583727aa1c9d27ff8c01491f911d2de0747f8c997ef6b3498'}, '0x802c2b90': {'address': 2150378384, 'size': 16, 'sha256': '20328d4327fadaaed8ad021724d9249544865299854153192385f2b6f3204ea7'}}

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
    return dict(whole_bodies=len(BODIES),original_calls=count,constant_spans=len(SPANS),original_bfn_sha256=bfn,limitation='Static source pins only; width child, actual source/camera and native glyph ownership remain separate')

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
