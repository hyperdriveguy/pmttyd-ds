#!/usr/bin/env python3
"""Pin both complete original US recursive MapObj flag bodies from read-only ISO."""
import argparse
import hashlib
import json
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
SPANS=[(0x8001eee8,0x314,'63d6a25d7e27a529869cfefca8a933b14ad0201998a763f4458c5b49b074047e'),(0x8001f1fc,0x440,'a141d54bf320389afa083b6dc77294c59ce4f727fb49824221432f5bc1da5d61')]
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
            if hashlib.sha256(body).hexdigest()!=digest:raise ValueError('Original MapObj flag body changed')
    return dict(spans=[dict(address=hex(a),size=n,sha256=d)for a,n,d in SPANS],source_executed=False,scope='Whole recursive source bodies; native frames verified against TESTONLY original instruction oracle, no original emulator execution claim')
if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--iso',default=DEFAULT_ISO);args=parser.parse_args();print(json.dumps(verify(args.iso),indent=2))
