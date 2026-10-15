#!/usr/bin/env python3
"""Read-only pins for whole width wrapper/line and actual lead/map/command source."""
import argparse
import hashlib
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO, inventory

BODIES = {'WidthWrapper': {'address': 2147964200, 'size': 36, 'sha256': '80a8f34948ea70c05ca31348c2e0f08bef5c0e5b899d77a2495404dc4bb7b01b', 'calls': [(2147964216, 2147964236)]}, 'WidthLine': {'address': 2147964236, 'size': 800, 'sha256': 'd7e85de3232683f40cc34ee2538baf9bfda7ec6827c0d761ed0eff1268fcb5dc', 'calls': [(2147964288, 2148014912), (2147964304, 2147962512), (2147964400, 2148014752), (2147964420, 2150028388), (2147964432, 2147545828), (2147964496, 2150024004), (2147964540, 2150024004), (2147964548, 2148014620), (2147964588, 2147590556), (2147964768, 2147962512)]}, 'LeadPredicate': {'address': 2148014912, 'size': 76, 'sha256': '6b1cc7be1b9b25190052866295ae20aafce435db8822851694bbe3efb5cf4a39', 'calls': []}, 'CodeToGlyph': {'address': 2147962512, 'size': 372, 'sha256': 'c5c62aec2542e0ef775597654d52cb05d0afb5f15614f32eaa546a8abb4c8e30', 'calls': []}, 'Command': {'address': 2148014752, 'size': 160, 'sha256': '9849799a9225a52cc9a1cd0c950302ddc4403f5deb17bb9590fecc64b63d9484', 'calls': []}}
SPANS = {'0x80420408': {'address': 2151810056, 'size': 5, 'sha256': 'a3458a02fe2c47ab7dbd0468240da989b3a8291829e530ce04e04feb3182ad0e'}, '0x802c2bbc': {'address': 2150378428, 'size': 24, 'sha256': '9ca8bfd1c7c844a4594fb610168101c31fb0845db173070233b547340789b5c4'}, '0x802c2b90': {'address': 2150378384, 'size': 16, 'sha256': '20328d4327fadaaed8ad021724d9249544865299854153192385f2b6f3204ea7'}, '0x8041601c': {'address': 2151768092, 'size': 28, 'sha256': '90c80baf2bd400796afec5f667e8fb2d78fae61b6fadd9635f6a4f0ee8a3565e'}}

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
    return dict(whole_bodies=len(BODIES),original_calls=count,constant_spans=len(SPANS),original_bfn_sha256=bfn,limitation='Static source pins only; actual actor/BFN/GP/text and typed command children remain required')

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
