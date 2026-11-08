#!/usr/bin/env python3
"""Pin both complete original US group flag parents from read-only ISO."""
import argparse
import hashlib
import json
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
SPANS=[(0x8001ecb8,0x118,'3f647242377dd37ad4fa51ba0d1fe5a3e2974ba097228ae90275f4d24af75755'),(0x8001edd0,0x118,'0c197b783322085525b1f43a8d57015f5decb97c4fb0da1413a739df07478e7a')]
def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as source:
        if source.read(8)!=b'G8ME01\0\0':raise ValueError('US revision0 ISO required')
        source.seek(0x420);dol,=struct.unpack('>I',source.read(4));source.seek(dol);header=source.read(256)
        for wanted,length,digest in SPANS:
            for index in range(18):
                offset,address,size=[struct.unpack_from('>I',header,table+4*index)[0]for table in(0,0x48,0x90)]
                if address<=wanted and wanted+length<=address+size:
                    source.seek(dol+offset+wanted-address);body=source.read(length);break
            else:raise ValueError('Original body outside DOL image')
            if hashlib.sha256(body).hexdigest()!=digest:raise ValueError('Original group body changed')
    return dict(spans=[dict(address=hex(a),size=n,sha256=d)for a,n,d in SPANS],source_executed=False,scope='Whole group parents, actual deferred-name queue; recursive mapFlg and libc children external')
if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--iso',default=DEFAULT_ISO);args=parser.parse_args();print(json.dumps(verify(args.iso),indent=2))
