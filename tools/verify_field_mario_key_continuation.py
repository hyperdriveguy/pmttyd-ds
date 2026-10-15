#!/usr/bin/env python3
"""Pin both COMPLETE original Mario admission query bodies, ISO read-only."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

SPANS = [
    (0x8005c47c, 0x13c, '0c69ad08b68833746d7d0cd63d28afc8fbc39593524efbb1b34b5f75321ad387'),
    (0x8005c464, 0x18, 'b078fdbe1aa77b06e7226bb4a5ddf699e3cc47a5d9c66ea3485b9f28f4fb05c3'),
]
CALLS = [(0x8005c48c,0x80141050),(0x8005c490,0x800bf1b4),
         (0x8005c498,0x8002e210),(0x8005c4c4,0x800a4224),
         (0x8005c4d0,0x800a4224),(0x8005c4e4,0x800a16dc),
         (0x8005c4f8,0x800a68e8),(0x8005c51c,0x800c5744),
         (0x8005c530,0x800c0068),(0x8005c544,0x800b6e94),
         (0x8005c58c,0x800d0874)]

def verify(iso=DEFAULT_ISO):
    bodies={}
    with Path(iso).open('rb') as disc:
        if disc.read(6)!=b'G8ME01':raise ValueError('Original US disc required')
        disc.seek(0x420);dol,=struct.unpack('>I',disc.read(4))
        disc.seek(dol);header=disc.read(256)
        for address,size,expected in SPANS:
            offsets=[]
            for i in range(18):
                offset,base,length=[struct.unpack_from('>I',header,k+4*i)[0] for k in (0,0x48,0x90)]
                if base<=address and address+size<=base+length:offsets.append(dol+offset+address-base)
            if len(offsets)!=1:raise ValueError('Original DOL mapping unavailable/ambiguous')
            disc.seek(offsets[0]);body=disc.read(size)
            if hashlib.sha256(body).hexdigest()!=expected:raise ValueError('Whole original query body mismatch')
            bodies[address]=body
    for site,target in CALLS:
        op,=struct.unpack_from('>I',bodies[0x8005c47c],site-0x8005c47c)
        displacement=op&0x03fffffc
        if displacement&0x02000000:displacement-=0x04000000
        if op>>26!=18 or op&3!=1 or (site+displacement)&0xffffffff!=target:
            raise ValueError('Original child branch mismatch')
    return {'whole_bodies':[{'address':hex(a),'length':n,'sha256':h} for a,n,h in SPANS],
            'original_child_calls':[{'site':hex(s),'target':hex(t)} for s,t in CALLS],
            'limitation':'Static original pins; no original CPU/GPU execution claim'}
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO)
    print(json.dumps(verify(p.parse_args().iso),indent=2))
