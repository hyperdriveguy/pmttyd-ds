#!/usr/bin/env python3
"""Read-only original US Turn parent, dispatch and numeric-base verification."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

TURN = (0x8011c510, 0x1ed8, '676aec347124b77cb7fbe40caeb81ff8b87fabd2ddd7ca53b96287d28b21a093')
ENTRIES = [0x8011c558,0x8011c568,0x8011c66c,0x8011c588,0x8011c60c,0x8011c684,
 0x8011d898,0x8011c69c,0x8011d978,0x8011da2c,0x8011db60,0x8011c69c,
 0x8011dd08,0x8011c69c,0x8011de64,0x8011c69c,0x8011e01c,0x8011c69c]
for entry in (0x8011c6f0,0x8011c7a8,0x8011c868,0x8011c928,0x8011c9e8,0x8011caa8,
 0x8011cb68,0x8011cc28,0x8011cd50,0x8011ce10,0x8011ced0,0x8011cf90,0x8011d050,
 0x8011d110,0x8011d1d0,0x8011d290,0x8011d350,0x8011d410,0x8011d4d0,0x8011d590,
 0x8011d650,0x8011d718,0x8011d7d8,0x8011e1c8,0x8011e204,0x8011e240,0x8011e27c,0x8011e2f4):
    ENTRIES.extend((entry,0x8011c69c))
ENTRIES.extend((0x8011e36c,0x8011e384,0x8011e3a4,0x8011e3b0))

def verify(iso):
    with Path(iso).open('rb') as disc:
        if disc.read(8)!=b'G8ME01\0\0':
            raise ValueError('Expected original US disc0 revision0')
        disc.seek(0x420); dol=struct.unpack('>I',disc.read(4))[0]
        disc.seek(dol); header=disc.read(256)
        def read(a,n):
            matches=[]
            for i in range(18):
                off,va,size=(struct.unpack_from('>I',header,k+4*i)[0] for k in (0,0x48,0x90))
                if va<=a and a+n<=va+size: matches.append(dol+off+a-va)
            if len(matches)!=1: raise ValueError(f'Unmapped original address {a:#x}')
            disc.seek(matches[0]); data=disc.read(n)
            if len(data)!=n: raise ValueError('Truncated original input')
            return data
        address,size,digest=TURN
        if hashlib.sha256(read(address,size)).hexdigest()!=digest: raise ValueError('Turn body mismatch')
        table=read(0x80363cc8,78*4)
        if list(struct.unpack('>78I',table))!=ENTRIES: raise ValueError('Turn dispatch mismatch')
        # Startup establishes r2=80427440. Earlier inferred r2 pools are stale.
        for a,data in {0x80005418:'3c40804260427440',0x80422594:'44610000',
                       0x8042256c:'3f800000',0x802efb78:'4330000080000000'}.items():
            if read(a,len(data)//2).hex()!=data: raise ValueError(f'Numeric base/constant mismatch {a:#x}')
    return dict(schema=1,source_function=hex(address),instructions=size//4,body_sha256=digest,
                dispatch_entries=len(ENTRIES),dispatch_sha256=hashlib.sha256(table).hexdigest(),
                numeric_base='0x80427440',scope='Complete parent only; actual children/native cadence remain separately owned')

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO); parser.add_argument('--output')
    args=parser.parse_args(); result=verify(args.iso)
    if args.output: Path(args.output).write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result,indent=2))
