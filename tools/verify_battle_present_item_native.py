#!/usr/bin/env python3
"""Read-only pins for whole typed native PresentItem and its FP maximum leaf."""
import argparse
import hashlib
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO
from verify_menu_interruptions import verify as verify_source

MAX_FP_SHA = '2402a8e18dedc3b2ef242fff2f573d45823d12fa0ae38b8bb20f777fc1584621'

def verify(iso=DEFAULT_ISO):
    result=verify_source(iso)
    with Path(iso).open('rb') as disc:
        disc.seek(0x420)
        dol=struct.unpack('>I',disc.read(4))[0]
        disc.seek(dol)
        header=disc.read(256)
        address,size=0x80126b14,0x44
        matches=[]
        for i in range(18):
            off,va,n=(struct.unpack_from('>I',header,b+4*i)[0] for b in (0,0x48,0x90))
            if off and va<=address and address+size<=va+n:
                matches.append(dol+off+address-va)
        if len(matches)!=1:
            raise ValueError('Unmapped/ambiguous GetMaxFp')
        disc.seek(matches[0])
        if hashlib.sha256(disc.read(size)).hexdigest()!=MAX_FP_SHA:
            raise ValueError('Complete original GetMaxFp body changed')
    result['extra_complete_max_fp_body']=True
    result['scope']='Whole PresentItem/recovery/XYZ/height/numerics source pins; typed private-output adaptation, no original execution proof'
    return result

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
