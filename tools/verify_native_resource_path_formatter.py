#!/usr/bin/env python3
"""Read-only exact source evidence for the finite native resource-path adapter."""
import argparse
import hashlib
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO
from verify_source_mario_dvd_root import verify as verify_root
from verify_source_font_setup import verify as verify_font
BODIES = [('sprintf', 2150015160, 224, '50511d2156f10f1f673ae32bf84f42cce8e35f4b3264040db35daa0666534c30'), ('__pformatter', 2150016084, 1908, 'c74bb533c4699ec5c95a120c5c9087573ef2638880f9c4049259751f202a9eb7'), ('__StringWrite', 2150015888, 108, 'f859639fca816a0f4205a654200cf3125b8d9e5ef3557f4d684eb76896924de1'), ('__va_arg', 2149992272, 200, 'a9a660215d56587712b11858b0577a45eac0fb4526f5517dcc35f8261e25752c'), ('parse_format', 2150022324, 1284, '3088f035f9ba8b0eb0482d6afcee9218dbb79e3f79e9652a05986af93c52840b'), ('strlen', 2150028980, 28, 'f8cf423851b3f901e72543ecac5969160cea3203626e88837cd2e95a99ea1214'), ('strchr', 2150028276, 48, '2ebe003307616278e39934e1fea0ed966c312549d2b4eae1f50cd5d767a6c2c3')]

def verify(iso=DEFAULT_ISO):
    verify_root(iso)
    verify_font(iso)
    with Path(iso).open('rb') as disc:
        disc.seek(0x420)
        dol, = struct.unpack('>I', disc.read(4))
        disc.seek(dol)
        header = disc.read(256)
        def read(address, size):
            offsets=[]
            for i in range(18):
                off,base,length=[struct.unpack_from('>I',header,t+4*i)[0] for t in (0,0x48,0x90)]
                if base<=address and address+size<=base+length:
                    offsets.append(dol+off+address-base)
            if len(offsets)!=1:raise ValueError('Unmapped initialized source span')
            disc.seek(offsets[0]);return disc.read(size)
        for name,address,size,sha in BODIES:
            if hashlib.sha256(read(address,size)).hexdigest()!=sha:
                raise ValueError('Original source body differs: '+name)
        if read(0x80303bb0,1)!=b'\0':raise ValueError('Original null%s sentinel differs')
    return dict(source_bodies=len(BODIES), root_and_three_actual_UI_formats=True,
                original_font_names_and_table=True,
                scope='Finite native C-runtime adapter evidence; no whole original printf/ABI execution claim')

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
