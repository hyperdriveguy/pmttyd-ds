#!/usr/bin/env python3
"""Pin seven-party battle snapshot initialization raw clear and preservation."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'BattleInit': (2148503300,
                312,
                '616f72f3468ba55fb0ac5dc234c5b950eb85fed4c8caafc20cca55bb73aa48c7'),
 'BattlePartyInfoWorkInit': (2148498464,
                             132,
                             '890163d04d56031a9ecd5e1edc8cca4e0702b011f8b92476afe0ed0314389e26'),
 'memset': (2147504308,
            48,
            '4176c17fce5f28ccea737946be853ea45a125b52e9acab356ba54454aa352b5c')}

ANCHORS = {2148498464: '9421ffd0',
 2148498468: '7c0802a6',
 2148498472: '90010034',
 2148498476: 'dbe10020',
 2148498480: 'f3e10028',
 2148498484: 'bf810010',
 2148498488: 'c3e2ad34',
 2148498492: '7c7c1b78',
 2148498496: '3ba000e0',
 2148498500: '3be00000',
 2148498504: '381dff20',
 2148498508: '38800000',
 2148498512: '1c60002c',
 2148498516: '38a0001e',
 2148498520: '3fc30001',
 2148498524: '3bde62a0',
 2148498528: '7fdcf214',
 2148498532: 'd3fe0000',
 2148498536: '387e0004',
 2148498540: '93fe0024',
 2148498544: '4bf0d445',
 2148498548: 'b3fe0028',
 2148498552: '3bbd0001',
 2148498556: '2c1d00e7',
 2148498560: 'b3fe002a',
 2148498564: '4180ffc4',
 2148498568: 'e3e10028',
 2148498572: 'cbe10020',
 2148498576: 'bb810010',
 2148498580: '80010034',
 2148498584: '7c0803a6',
 2148498588: '38210030',
 2148498592: '4e800020',
 2148503576: '4bffec09'}

CONSTANTS = {2147505176: '3c40804260427440', 2151817588: '3f800000'}

def verify(iso):
    with Path(iso).open('rb') as disc:
        if disc.read(8)!=b'G8ME01\0\0':
            raise ValueError('Expected US G8ME01 disc0 revision0')
        disc.seek(0x420)
        dol_offset=struct.unpack('>I',disc.read(4))[0]
        disc.seek(dol_offset)
        header=disc.read(256)
        def read(address,size):
            matches=[]
            for i in range(18):
                off,va,length=(struct.unpack_from('>I',header,j+i*4)[0] for j in (0,0x48,0x90))
                if va<=address and address+size<=va+length:
                    matches.append(dol_offset+off+address-va)
            if len(matches)!=1:
                raise ValueError(f'Unmapped/ambiguous DOL address {address:#x}')
            disc.seek(matches[0]);data=disc.read(size)
            if len(data)!=size:
                raise ValueError('Truncated DOL')
            return data
        sites=set()
        for section in range(7):
            va,length=(struct.unpack_from('>I',header,j+4*section)[0] for j in (0x48,0x90))
            if not va: continue
            code=read(va,length)
            for offset in range(0,length,4):
                address=va+offset
                word=struct.unpack_from('>I',code,offset)[0]
                if word>>26!=18 or not word&1: continue
                delta=word&0x3fffffc
                if delta&0x2000000: delta-=0x4000000
                target=(delta if word&2 else address+delta)&0xffffffff
                if target==0x800f7c20: sites.add(address)
        if sites!={0x800f9018}:
            raise ValueError('Scoped aligned direct DOL caller census mismatch')
        for name,(address,size,digest) in BODIES.items():
            if hashlib.sha256(read(address,size)).hexdigest()!=digest:
                raise ValueError(f'Body mismatch: {name}')
        for address,value in {**ANCHORS,**CONSTANTS}.items():
            if read(address,len(value)//2).hex()!=value:
                raise ValueError(f'Instruction/constant mismatch: {address:#x}')
    return dict(schema=1,scope='Complete seven-party snapshot initializer;fill and scoped caller identities linked;later producers/save/runtime remain open',dol_direct_call_sites=len(sites),bodies=len(BODIES),instruction_anchors=len(ANCHORS),data_spans=len(CONSTANTS))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--iso',default=DEFAULT_ISO)
    p.add_argument('--output')
    a=p.parse_args();result=verify(a.iso)
    if a.output:
        output=Path(a.output)
        if output.resolve()==Path(a.iso).resolve():
            raise ValueError('Output must not overwrite ISO')
        output.parent.mkdir(parents=True,exist_ok=True)
        output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result,sort_keys=True))
