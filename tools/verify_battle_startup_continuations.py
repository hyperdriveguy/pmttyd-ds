#!/usr/bin/env python3
"""Pin whole retained battle startup parents and inherited PartyGet argument evidence."""
import argparse
import hashlib
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_battle_sequence_startup import verify as verify_startup
BODIES = {2148656576: {'size': 2040, 'sha256': '58885b6803df68a6e807afd45fe5d04ec0ed1da0b5fbf40a03506cec9b304291', 'calls': [['0x8011e5e0', '0x800f89a0'], ['0x8011e628', '0x800374ac'], ['0x8011e640', '0x800f88cc'], ['0x8011e680', '0x8005e0c0'], ['0x8011e6a4', '0x8012733c'], ['0x8011e6b0', '0x800374ac'], ['0x8011e6d4', '0x800374ac'], ['0x8011e704', '0x800f8a08'], ['0x8011e764', '0x800feaf8'], ['0x8011e794', '0x800374ac'], ['0x8011e7e0', '0x8011edb8'], ['0x8011e7fc', '0x800f75f8'], ['0x8011e804', '0x80128144'], ['0x8011e810', '0x8019ec60'], ['0x8011e818', '0x8019ec28'], ['0x8011e82c', '0x800374ac'], ['0x8011e874', '0x800f8728'], ['0x8011e88c', '0x800374ac'], ['0x8011e8a8', '0x800f87a8'], ['0x8011e8c0', '0x800374ac'], ['0x8011e8e4', '0x800f8a08'], ['0x8011e910', '0x80013368'], ['0x8011e91c', '0x800d98bc'], ['0x8011e938', '0x800f8a08'], ['0x8011e93c', '0x80010d94'], ['0x8011e95c', '0x800d95f8'], ['0x8011e970', '0x8011ace8'], ['0x8011e97c', '0x800f7650'], ['0x8011e984', '0x8011ace8'], ['0x8011e990', '0x80010d94'], ['0x8011e9ac', '0x800f8a08'], ['0x8011e9b8', '0x800f88cc'], ['0x8011e9d8', '0x8011aa34'], ['0x8011e9e4', '0x801a7e94'], ['0x8011e9f4', '0x800f8a08'], ['0x8011ea04', '0x800f8a08'], ['0x8011ea1c', '0x800f8a08'], ['0x8011ea24', '0x801958bc'], ['0x8011ea2c', '0x8011ace8'], ['0x8011ea3c', '0x801956f8'], ['0x8011ea48', '0x80195af8'], ['0x8011ea5c', '0x800f895c'], ['0x8011ea6c', '0x800f7624'], ['0x8011ea70', '0x800f74b4'], ['0x8011ea80', '0x801a7e94'], ['0x8011ea8c', '0x800f895c'], ['0x8011ea90', '0x801a7e30'], ['0x8011eaa0', '0x8011aa34'], ['0x8011eaac', '0x80203394'], ['0x8011eac0', '0x800f895c'], ['0x8011eac8', '0x801956f8'], ['0x8011ead4', '0x80195af8'], ['0x8011eae8', '0x800f895c'], ['0x8011eaf0', '0x8011ace8'], ['0x8011eb20', '0x800f8a08'], ['0x8011eb24', '0x80148824'], ['0x8011eb3c', '0x800f8a08'], ['0x8011eb54', '0x80036604'], ['0x8011eb84', '0x800f8a08'], ['0x8011eba8', '0x800f88cc'], ['0x8011ebc4', '0x80127814'], ['0x8011ebf4', '0x800374ac'], ['0x8011ec2c', '0x801958bc'], ['0x8011ec40', '0x800f8a08'], ['0x8011ec64', '0x800f8a08'], ['0x8011ec98', '0x80036604'], ['0x8011ecbc', '0x8011ace8'], ['0x8011ecd0', '0x80195af8'], ['0x8011ece4', '0x801956f8'], ['0x8011ed0c', '0x800f8a08'], ['0x8011ed30', '0x800f8a08'], ['0x8011ed3c', '0x80148414'], ['0x8011ed50', '0x800f8a08'], ['0x8011ed58', '0x8011ace8'], ['0x8011ed74', '0x800f8a08'], ['0x8011ed90', '0x800f8a08'], ['0x8011eda0', '0x800f8a08']]}, 2148658752: {'size': 408, 'sha256': '1e2842e3f02393f304dee29a856d0be5bea2c2913c9bd9f95de6c65e6142b746', 'calls': [['0x8011ee60', '0x800f89a0'], ['0x8011eeb0', '0x800f8a08'], ['0x8011eeb4', '0x8005e0c0'], ['0x8011eec4', '0x80141050'], ['0x8011eec8', '0x800bf1b4'], ['0x8011eee4', '0x800f8aac'], ['0x8011eef8', '0x800f8a08'], ['0x8011ef04', '0x80129994'], ['0x8011ef20', '0x800f8a08'], ['0x8011ef30', '0x800414e8'], ['0x8011ef44', '0x800414e8'], ['0x8011ef58', '0x800414e8'], ['0x8011ef68', '0x80144d44'], ['0x8011ef84', '0x800f8a08'], ['0x8011ef94', '0x800f8a08'], ['0x8011ef9c', '0x800f7650'], ['0x8011efa0', '0x800f7624'], ['0x8011efb0', '0x800f8a08'], ['0x8011efc0', '0x800f8a08']]}, 2148266420: {'size': 32, 'sha256': 'b338694df4542858c65e2ac8e13a77b66393a928b732dd679665bcfdbfac572a', 'calls': []}}

def verify(iso=DEFAULT_ISO):
    verify_startup(iso)
    with Path(iso).open('rb') as disc:
        if disc.read(8)!=b'G8ME01\0\0':
            raise ValueError('US revision-0 disc required')
        disc.seek(0x420)
        dol,=struct.unpack('>I',disc.read(4))
        disc.seek(dol)
        header=disc.read(256)
        def read(address,size):
            offsets=[]
            for i in range(18):
                off,base,n=[struct.unpack_from('>I',header,t+4*i)[0] for t in (0,0x48,0x90)]
                if base<=address and address+size<=base+n:
                    offsets.append(dol+off+address-base)
            if len(offsets)!=1:
                raise ValueError('Unmapped/ambiguous initialized source span')
            disc.seek(offsets[0])
            b=disc.read(size)
            if len(b)!=size:
                raise ValueError('Truncated original source span')
            return b
        def calls(base,raw):
            result=[]
            for offset in range(0,len(raw),4):
                word,=struct.unpack_from('>I',raw,offset)
                if word>>26==18 and word&1:
                    displacement=word&0x3fffffc
                    if displacement&0x2000000:
                        displacement-=0x4000000
                    result.append([hex(base+offset),hex((base+offset+displacement)&0xffffffff)])
            return result
        for address,expected in BODIES.items():
            raw=read(address,expected['size'])
            if hashlib.sha256(raw).hexdigest()!=expected['sha256'] or calls(address,raw)!=expected['calls']:
                raise ValueError(f'Original whole body/calls changed: {address:#x}')
    return dict(whole_retained_parents=2, whole_party_argument_getter=1,
        linked_sequence_startup_spans=50,
        scope='Source parent/control proof, not normal battle entry or external child completion')

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
