#!/usr/bin/env python3
"""Read-only original US packed weapon-cost body, children and constants."""
import argparse
import hashlib
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
BODIES = {'BtlUnit_GetWeaponCost': {'address': 2148690280, 'size': 344, 'sha256': 'cd640b206f92d2d44e49fa12a881811e0852aed175f631799530c2ada575183c', 'calls': [(2148690320, 2148348672), (2148690504, 2150049312)]}, 'pouchEquipCheckBadge': {'address': 2148348672, 'size': 200, 'sha256': '2800af1700923fd4deabfe11a78b7b85163bb9be22c7fdab73bf456b29febec1', 'calls': []}, 'powWrapper': {'address': 2150049312, 'size': 32, 'sha256': '5420b121cb878185b7953f1cf2286cf08cf0db37b652270011e2704d030cd526', 'calls': [(2150049324, 2150039196)]}}
SPANS = {'0x802f00b8': {'address': 2150564024, 'size': 8, 'sha256': '07af8387624cf8f36152797d2f561246f21237d1d22b5c13006eacfa6f475e43'}, '0x802f00d0': {'address': 2150564048, 'size': 8, 'sha256': 'a06f129fc52abf6085679d7cd71dc41ec7580c7f5f73efef6d02dde22bb00994'}}
def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as disc:
        if disc.read(8)!=b'G8ME01\0\0':
            raise ValueError('US G8ME01 disc0 revision0 required')
        disc.seek(0x420)
        dol=struct.unpack('>I',disc.read(4))[0]
        disc.seek(dol)
        header=disc.read(256)
        def read(address,size):
            extents=[]
            for i in range(18):
                off,va,n=(struct.unpack_from('>I',header,b+4*i)[0] for b in (0,0x48,0x90))
                if off and va<=address and address+size<=va+n:
                    extents.append(dol+off+address-va)
            if len(extents)!=1:
                raise ValueError('Unmapped/ambiguous original source')
            disc.seek(extents[0])
            raw=disc.read(size)
            if len(raw)!=size:
                raise ValueError('Truncated original source')
            return raw
        calls_checked=0
        for name,expected in BODIES.items():
            raw=read(expected['address'],expected['size'])
            if hashlib.sha256(raw).hexdigest()!=expected['sha256']:
                raise ValueError('Original body changed: '+name)
            calls=[]
            for i in range(0,len(raw),4):
                word=struct.unpack_from('>I',raw,i)[0]
                if word&0xfc000003==0x48000001:
                    relative=word&0x03fffffc
                    if relative&0x02000000:
                        relative-=0x04000000
                    calls.append((expected['address']+i,(expected['address']+i+relative)&0xffffffff))
            if calls!=expected['calls']:
                raise ValueError('Original calls changed: '+name)
            calls_checked+=len(calls)
        for name,span in SPANS.items():
            if hashlib.sha256(read(span['address'],span['size'])).hexdigest()!=span['sha256']:
                raise ValueError('Original double changed: '+name)
    return dict(original_bodies=len(BODIES),calls=calls_checked,double_spans=len(SPANS),scope='Static original pins only; actual packed memory/pouch/pow/FPSCR ownership and native binding remain separate')
if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
