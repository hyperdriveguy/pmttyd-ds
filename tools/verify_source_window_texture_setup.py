#!/usr/bin/env python3
"""Pin original US whole window texture Setup/callback/Unpack and linked TEX/GX."""
import argparse
import hashlib
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_source_palette_texture_object import verify as verify_tex
BODIES = {2148006872: {'size': 280, 'sha256': 'ad0d279bdaf0fc7bb1d078efd0e44e1d83ab75c80def683e9c25d01f57d7c829', 'calls': [['0x8007fc08', '0x800782e4'], ['0x8007fc18', '0x80007388'], ['0x8007fc30', '0x8026a0b8'], ['0x8007fc40', '0x80144e88'], ['0x8007fc48', '0x80144dcc'], ['0x8007fc5c', '0x800300f0'], ['0x8007fc80', '0x80144de4'], ['0x8007fc88', '0x80037b6c'], ['0x8007fc9c', '0x800300f0'], ['0x8007fcb8', '0x80037a80']]}, 2148007152: {'size': 148, 'sha256': 'e7d3df81ea14901a3be297df89088936000fc0fddf269ad0cc6470ea85053c13', 'calls': [['0x8007fd0c', '0x80037b6c'], ['0x8007fd14', '0x80144dd4'], ['0x8007fd28', '0x800300f0'], ['0x8007fd44', '0x80037a80']]}, 2147711852: {'size': 288, 'sha256': '21062a25695107ab6ea889d92fbfe7beb69ba3f5d8826a9415cf6ec7dda83d55', 'calls': [['0x80037ba8', '0x80297894']]}}
LITERALS = {2150379648: '666f6e742f6d736757696e646f772e74706c0000', 2150379668: '25732f662f6d736757696e646f772e74706c0000', 2150367560: '74657850616c657474652e6300000000', 2150367576: '696e76616c69642076657273696f6e206e756d62657220666f7220746578747572652070616c65747465000000'}
LOGO_EXIT_SHA = '54b7691885ff90c84e987575559e093c599c4c125b11f62e9cc6358bb2da3500'

def verify(iso=DEFAULT_ISO):
    linked=verify_tex(iso)
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
        for address,literal in LITERALS.items():
            expected=bytes.fromhex(literal)
            if read(address,len(expected))!=expected:
                raise ValueError(f'Original literal changed: {address:#x}')
        # Pin the actual only direct Setup caller and its whole tiny body.
        if hashlib.sha256(read(0x8008746c,0x44)).hexdigest()!=LOGO_EXIT_SHA:
            raise ValueError('Original seq_logoExit body changed')
        callers=[]
        for i in range(7):
            base,size=[struct.unpack_from('>I',header,t+4*i)[0] for t in (0x48,0x90)]
            if size:
                callers += [site for site,target in calls(base,read(base,size)) if target=='0x8007fbd8']
        if callers!=['0x8008747c']:
            raise ValueError('Original direct Setup caller set changed')
    return dict(window_source_bodies=3, direct_calls=15, literal_spans=4,
                only_direct_setup_caller='seq_logoExit8008746c+10',linked_tex_bodies=linked['whole_tex_bodies'],linked_sdk_bodies=linked['actual_sdk_children'],
                scope='Original CPU/process setup graph only; actual archive/DVD/heap/callback/GPU lifetime providers still required')
if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
