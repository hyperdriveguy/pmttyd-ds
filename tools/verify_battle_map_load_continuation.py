#!/usr/bin/env python3
"""Pin the complete original US bmapLoad body from the read-only ISO."""
import argparse
import hashlib
import json
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
ADDRESS=0x8002892c
SIZE=0x17c
SHA256='db1c37ba1723eb44e26223fdc78d595381d71abe136fa9c7987691c0024e55d7'
def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as source:
        if source.read(8)!=b'G8ME01\0\0':raise ValueError('US revision0 ISO required')
        source.seek(0x420);dol,=struct.unpack('>I',source.read(4));source.seek(dol);header=source.read(256)
        for index in range(18):
            offset,address,size=[struct.unpack_from('>I',header,table+4*index)[0] for table in (0,0x48,0x90)]
            if address<=ADDRESS and ADDRESS+SIZE<=address+size:
                source.seek(dol+offset+ADDRESS-address);body=source.read(SIZE);break
        else:raise ValueError('Original body outside DOL image')
    if hashlib.sha256(body).hexdigest()!=SHA256:raise ValueError('Original bmapLoad body changed')
    return dict(address=hex(ADDRESS),size=SIZE,sha256=SHA256,source_executed=False,
                scope='Whole original bank producer; native source heap/files/cache/light and map display owners remain external')
if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--iso',default=DEFAULT_ISO);args=parser.parse_args();print(json.dumps(verify(args.iso),indent=2))
