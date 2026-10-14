#!/usr/bin/env python3
"""Read-only US original opening glide/callback/curve/NPC-consumer evidence."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_opening_house_requests import verify as verify_requests
FUNCTIONS=[('evt_npc_glide_position',0x800512bc,0x5ac,'dbb3b625cd1770f9909536015b532d889611860c38e23917e1c699f95cf50f78'),
 ('_intplGetFloat',0x800509ec,0x1e0,'0f2a733e2cf09ee68829956488049c40c48ecac9f7dd5f7cf4757347777b30f3'),
 ('npcMain',0x80049a7c,0x19a4,'decc7aa26c3e7990624bbb9204ed0ce286ecabc49405d3140390a301e59a6b7f'),
 ('npcMoveG',0x80048d9c,0x330,'9e112539a029748a486e2f692b41cf3fd6884beb3dc9c0ea3a36673c092602b6')]
def verify(iso,symbols):
    count=verify_requests(iso,symbols)
    with Path(iso).open('rb') as source:
        if source.read(8)!=b'G8ME01\0\0':raise ValueError('Original US disc0/revision0 required')
        source.seek(0x420);dol=struct.unpack('>I',source.read(4))[0];source.seek(dol);h=source.read(256)
        def read(a,n):
            for j in range(18):
                o,v,z=(struct.unpack_from('>I',h,b+4*j)[0] for b in [0,0x48,0x90])
                if v<=a and a+n<=v+z:
                    source.seek(dol+o+a-v);data=source.read(n)
                    if len(data)!=n:raise ValueError('Truncated source DOL')
                    return data
            raise ValueError('Unmapped source evidence')
        for name,address,size,digest in FUNCTIONS:
            if hashlib.sha256(read(address,size)).hexdigest()!=digest:raise ValueError('Original source changed: '+name)
        words={0x80306a24+11*4:0x80050b58,0x8041fd84:0,0x8041fd88:0x3f800000,0x8041fd90:0x3f000000,
               0x8041fd94:0x447a0000,0x8041fda8:0x40490fdb,
               0x800515dc:0xec221828,0x800515e4:0xec840828,
               0x80051644:0xec1b06ba,0x800516dc:0xec0316ba,
               0x80051708:0xec1e00b8,0x80051818:0x38600001}
        for address,expected in words.items():
            if struct.unpack('>I',read(address,4))[0]!=expected:raise ValueError('Glide scalar/order/result evidence changed')
    return count+len(words)
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--symbols',default='artifacts/reference/us_symbols.csv');a=p.parse_args()
    print('Original opening glide: four function hashes and',verify(a.iso,a.symbols),'request/constant/arithmetic/result anchors verified')
