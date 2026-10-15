#!/usr/bin/env python3
"""Whole original submenu InitSub/Rasta/Quad/Vertex bodies; ISO read-only."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
from iso_assets import DEFAULT_ISO
SPANS=[(0x80103a14,0x134,'a815d669d03bfa71bfa8da8173cbaa77f022a9263fc0422af71b7a3ac00d3ae2'),
 (0x80103c3c,0x94,'ce0492b2e38a644ae90088dd34d8225d16c023f31eb04e7a7d76ed763fc70d37'),
 (0x801037c4,0x124,'4bb58a4df00ad37e5b4e25d4c51dc3a1ff141836a9e2d6efa9092a971b10c708'),
 (0x801038e8,0x30,'cf922e9043ec01d4212a864999dbbd146bd27879c2ddf356f31483bcec9e5dcb'),
 (0x802b76c4,0x10,'1d5a0d47f100f343d987520897dd27ea318022251c09585841c9bda8e96e90ab'),
 (0x802b76d4,0x10,'7b7b8302f957eb41d66023825238ca9f45caf47502d52f4630ccacf10d9b6b7e'),
 (0x802b76e4,8,'03f4f95fd457c776fba85c4dec52c0809298af46acd5cd90ad041962b2ca9d7a')]
def verify(iso=DEFAULT_ISO):
    bodies={}
    with Path(iso).open('rb') as f:
        if f.read(6)!=b'G8ME01':raise ValueError('US original disc required')
        f.seek(0x420);dol,=struct.unpack('>I',f.read(4));f.seek(dol);h=f.read(256)
        def read(a,n):
            positions=[]
            for i in range(18):
                off,base,length=[struct.unpack_from('>I',h,k+4*i)[0] for k in (0,0x48,0x90)]
                if base<=a and a+n<=base+length:positions.append(dol+off+a-base)
            if len(positions)!=1:raise ValueError('Unmapped original body')
            f.seek(positions[0]);return f.read(n)
        for a,n,sha in SPANS:
            b=read(a,n)
            if hashlib.sha256(b).hexdigest()!=sha:raise ValueError('Whole original body mismatch')
            bodies[a]=b
        if read(0x80422240,4)!=bytes(4) or read(0x8042224c,4)!=bytes.fromhex('3f800000'):raise ValueError('Original Quad constants differ')
    root=Path(__file__).resolve().parents[1]
    source=(root/'source/display/battle_submenu_source_gx.cpp').read_text()
    header=(root/'include/display/battle_texture_plane.hpp').read_text()
    enum={n:int(a,16) for n,a in re.findall(r'(\w+)=(0x[0-9a-f]+)',header)}
    records={int(site,16):(enum[name],[int(v) for v in values.split(',') if v]) for site,name,values in re.findall(r'\{(0x[0-9a-f]+),F::(\w+),\{([0-9,]*)\}\}',source)}
    decoded=[]
    for a in (0x80103c3c,0x80103a14):
        imm={}
        for offset in range(0,len(bodies[a]),4):
            op,=struct.unpack_from('>I',bodies[a],offset);site=a+offset
            if op>>26==14 and (op>>16)&31==0:imm[(op>>21)&31]=op&0xffff
            if op>>26==18 and op&3==1:
                d=op&0x03fffffc
                if d&0x02000000:d-=0x04000000
                target=(site+d)&0xffffffff
                if target==0x80103c3c:continue # Fully owned nested body above.
                if site not in records:raise ValueError('Missing original GX child site')
                function,args=records[site]
                if function!=target or any(imm.get(i+3)!=v for i,v in enumerate(args)):raise ValueError('Original GX operand differs')
                decoded.append(dict(site=hex(site),function=hex(target),arguments=args));imm={}
    if len(decoded)!=22 or len(records)!=22:raise ValueError('Unexpected retained source child census')
    # Quad delegates four real Vertex calls, each emits XYZ+4 byte RGBA+ST.
    def target(a,offset):
        op,=struct.unpack_from('>I',bodies[a],offset);d=op&0x03fffffc
        if d&0x02000000:d-=0x04000000
        return a+offset+d
    if [target(0x801037c4,s-0x801037c4) for s in (0x80103844,0x80103868,0x8010388c,0x801038b0)]!=[0x801038e8]*4:raise ValueError('Original vertex calls differ')
    return dict(whole_bodies=[dict(address=hex(a),length=n,sha256=h) for a,n,h in SPANS],init_GX_children=decoded,quad_FIFO_lanes=36,
        limitation='Static original pins and child operand verification; native geometry/GPU parity separate')
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);a=p.parse_args();print(json.dumps(verify(a.iso),indent=2))
