#!/usr/bin/env python3
"""Pin original US whole icon Init/callbacks, actual DVD rounding evidence and linked Unpack/TEX/GX."""
import argparse
import hashlib
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_source_window_texture_setup import verify as verify_unpack
BODIES = {2147595928: {'size': 376, 'sha256': '3daebb70b893168d2ac90b649492e5cba552f9b7d461cef8d818d7aea6f6134c', 'calls': [['0x8001b6bc', '0x800300f0'], ['0x8001b6d4', '0x800050b4'], ['0x8001b6ec', '0x800300f0'], ['0x8001b700', '0x800050b4'], ['0x8001b718', '0x80007388'], ['0x8001b730', '0x8026a0b8'], ['0x8001b740', '0x80144e88'], ['0x8001b74c', '0x80144dcc'], ['0x8001b760', '0x800300f0'], ['0x8001b784', '0x80144de4'], ['0x8001b788', '0x80007388'], ['0x8001b7a0', '0x8026a0b8'], ['0x8001b7b0', '0x80144e88'], ['0x8001b7bc', '0x80144dcc'], ['0x8001b7d0', '0x800300f0'], ['0x8001b7f4', '0x80144de4']]}, 2147596304: {'size': 44, 'sha256': '5dcd1ca00413af9de212e2f5f44118889478cadf885bae72f5d41df0f173e4ff', 'calls': [['0x8001b820', '0x80144dd4']]}, 2147596348: {'size': 64, 'sha256': '1045401d08c5cda862c016001ef54023c24387773511762a9f2efd1b30285f10', 'calls': [['0x8001b854', '0x80037b6c'], ['0x8001b85c', '0x80144dd4']]}, 2148814884: {'size': 612, 'sha256': '0c36f07bb2d0e9bba569a99e997e5fa02ae0e7e55d9f07e17fca2169984b30e8', 'calls': [['0x80145044', '0x800050b4'], ['0x801450e8', '0x800064fc'], ['0x8014515c', '0x802a03c0'], ['0x80145234', '0x802a0178'], ['0x80145244', '0x800050b4']]}, 2150237120: {'size': 280, 'sha256': '1ae8b4001e51e6aa0a3e244d4e79bcada065e72e8ce2cdee6a2ef32b00883084', 'calls': [['0x802a0408', '0x80297894'], ['0x802a0438', '0x80297894'], ['0x802a045c', '0x802a21f8'], ['0x802a0470', '0x80299ad4'], ['0x802a04b0', '0x8029d5b0'], ['0x802a04bc', '0x80299afc']]}}
FORMAT_SHA="b20463192bd4764d579f6b85f8f8119ebf8892396b7ccd6a9b28e05fec6ec59e"

def verify(iso=DEFAULT_ISO):
    linked=verify_unpack(iso)
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
        if hashlib.sha256(read(0x802bf858,24)).hexdigest()!=FORMAT_SHA:
            raise ValueError('Original icon path formats changed')
    return dict(whole_icon_bodies=3,dvd_evidence_bodies=2,linked_unpack=True,
        scope='Whole IconInit and callbacks; DVDMgrMain/DVDReadPrio are pinned evidence, not claimed implemented parents')
if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
