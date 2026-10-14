#!/usr/bin/env python3
"""Read-only US damage-controller source pins; emits metadata, no source body."""
import argparse
import hashlib
import json
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO

SPANS = (
    ('BattleCheckDamage',0x800fb01c,0xa00,'a35cd56fe86bf26fb856fad2d588781f37606c890dbf4444427361eb1001c51e'),
    ('BattleCheckPikkyoro',0x800fde2c,0x1a8,'3d4cc89f305c0475385bcf6faafb232277f81d89f9b06b20f59a46d192801815'),
    ('presentation_constants',0x804221b0,20,'129ab1892716f030238c340fcef26506272e0622b17ec906c20b420a901161d5'),
)
# Exact source instructions at the captured-owner and independent-half sites.
ANCHORS = {
    0x800fb05c:0x832d1c70,
    0x800fb0a4:0x2c030005,
    0x800fb7d8:0x1c760032,0x800fb7dc:0x38000064,0x800fb7e4:0x7c0303d6,
    0x800fb7fc:0x1c760032,0x800fb800:0x38000064,0x800fb808:0x7c0303d6,
    0x800fb820:0x1c760032,0x800fb824:0x38000064,0x800fb82c:0x7c0303d6,
    0x800fb9a8:0x7f23cb78,0x800fb9b0:0x38600000,
}

def verify(iso):
    with Path(iso).open('rb') as disc:
        header=disc.read(0x440)
        if header[:8]!=b'G8ME01\0\0':raise ValueError('US disc0 revision0 required')
        dol=struct.unpack_from('>I',header,0x420)[0];disc.seek(dol);sections=disc.read(256)
        def read(address,size):
            for i in range(18):
                offset,base,length=(struct.unpack_from('>I',sections,k+4*i)[0] for k in (0,0x48,0x90))
                if base<=address and address+size<=base+length:
                    disc.seek(dol+offset+address-base);body=disc.read(size)
                    if len(body)!=size:raise ValueError('Truncated damage source')
                    return body
            raise ValueError('Unmapped damage source')
        pins=[]
        for name,address,size,digest in SPANS:
            if hashlib.sha256(read(address,size)).hexdigest()!=digest:raise ValueError('Unexpected '+name+' body')
            pins.append(dict(name=name,address=address,size=size,sha256=digest))
        for address,instruction in ANCHORS.items():
            if struct.unpack('>I',read(address,4))[0]!=instruction:raise ValueError('Unexpected damage anchor '+hex(address))
        constants=struct.unpack('>5f',read(0x804221b0,20))
        if constants!=(60,45,10,.75,1):raise ValueError('Unexpected original effect constants')
    return dict(source='US G8ME01 revision0',spans=pins,anchors_checked=len(ANCHORS),constants=constants,executed_original=False)

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--iso',default=DEFAULT_ISO)
    print(json.dumps(verify(parser.parse_args().iso),indent=2))
