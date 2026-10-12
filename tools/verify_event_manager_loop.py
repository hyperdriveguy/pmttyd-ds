#!/usr/bin/env python3
"""Check event manager and scoped dispatch continuation contracts against read-only US ISO."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'evtmgrMain': (2147707088,
                492,
                '6d9624873872aee5e5bda1c660a5b81401eda51cdc9643c9e464ef315e0150f4'),
 'make_pri_table': (2147711284,
                    300,
                    '32e05591497e689ed976e0af70f3d67e788a827064d3c10826ad50bd8ce2e885'),
 'evtGetWork': (2147711584, 32, 'fb90cf4f7cca69ea6a6b1e08fdada64981606fbeb9df3dbf43d2d2930532cdc4'),
 'evtmgrCmd': (2147686072,
               5048,
               '4b6cdc0e9b940137848236a344598d586d850790e0b25beda4b9ed81da069d0e'),
 'evt_wait_msec': (2147702916,
                   208,
                   '9fb1aaaa4601dc4b4b2d4d03e7751f9a69ae6bcbc302c76b2fdfbdd4d11598c8'),
 '__div2i': (2149993176, 312, 'b8acc70386f7831b292e561c133ce6abb4a2881bb22e97780f09d2e0bbeee660'),
 '__div2u': (2149992940, 236, '9a130bd65ecb0edbca64d29fd90f712e96be72cd7089a05b7e64a2fe4af41d83')}

ANCHORS = {2147707112: '83ad90b0',
 2147707152: '906d1938',
 2147707164: '837d003c',
 2147707168: '839d0038',
 2147707172: '7cc4d810',
 2147707184: '7ca4e110',
 2147707208: '7c6300d1',
 2147707216: '3b200000',
 2147707232: '800400f8',
 2147707256: '7c060016',
 2147707268: '4822e155',
 2147707280: '6c058000',
 2147707320: '7c0303d6',
 2147707324: '7c00c1d6',
 2147707332: '937f009c',
 2147707336: '939f0098',
 2147707340: '48000f69',
 2147707384: '41820088',
 2147707396: '7c030000',
 2147707404: '70800092',
 2147707424: '7c00c814',
 2147707432: '90180004',
 2147707440: '90180000',
 2147707452: 'ec01002a',
 2147707456: 'd0180168',
 2147707468: '4180002c',
 2147707472: 'ec00f828',
 2147707480: 'd0180168',
 2147707488: '2c030001',
 2147707508: '4082ffd0',
 2147707532: '800d193c',
 2147707548: '900d1938',
 2147711360: '540007ff',
 2147711384: '90050000',
 2147711400: '80070000',
 2147711412: '906d193c',
 2147711504: '7c060040',
 2147711520: '93ab0000',
 2147711532: '90aa0000',
 2147686120: '4181130c',
 2147686160: '907f01a8',
 2147686184: '7c638670',
 2147686188: '987f0009',
 2147686204: '909f0014',
 2147686208: '981f000d',
 2147686664: '907f0078',
 2147686672: '981f000d',
 2147686684: '4082000c',
 2147686696: '3803ffff',
 2147686724: '38000001',
 2147702956: '83e30000',
 2147702984: '907d0078',
 2147702992: '901d007c',
 2147703004: '907d0080',
 2147703020: '4082000c',
 2147703064: '7c84f050',
 2147703076: '4822f0c9',
 2147703088: '7c1f2010',
 2147703092: '7c651914',
 2147690268: '4bffeb45',
 2147690272: '907f0098',
 2147690284: '38800001',
 2147690296: '98bf0009',
 2147690304: '981f000d',
 2147690332: '38800000',
 2147691012: '3860ffff',
 2147691028: '38600001',
 2147691052: '981f000a',
 2147691072: '981f000a',
 2147711604: '4d820020'}

CONSTANTS = {2151807536: '3f800000',
 2147505176: '3c40804260427440',
 2147505184: '3da0804161adcf20',
 2150649808: '80031704',
 2150649816: '80031748',
 2150649844: '800318e8',
 2150649848: '80031950',
 2150650172: '800326fc'}

def verify(iso):
    with Path(iso).open('rb') as disc:
        if disc.read(8)!=b'G8ME01\0\0':
            raise ValueError('Expected US G8ME01 disc0 revision0')
        disc.seek(0x420)
        dol_offset=struct.unpack('>I',disc.read(4))[0]
        disc.seek(dol_offset)
        header=disc.read(256)
        def read(address,size):
            matches=[]
            for i in range(18):
                off,va,length=(struct.unpack_from('>I',header,j+i*4)[0] for j in (0,0x48,0x90))
                if va<=address and address+size<=va+length:
                    matches.append(dol_offset+off+address-va)
            if len(matches)!=1:
                raise ValueError(f'Unmapped/ambiguous DOL address {address:#x}')
            disc.seek(matches[0]);data=disc.read(size)
            if len(data)!=size:
                raise ValueError('Truncated DOL')
            return data
        for name,(address,size,digest) in BODIES.items():
            if hashlib.sha256(read(address,size)).hexdigest()!=digest:
                raise ValueError(f'Body mismatch: {name}')
        for address,value in {**ANCHORS,**CONSTANTS}.items():
            if read(address,len(value)//2).hex()!=value:
                raise ValueError(f'Instruction/constant mismatch: {address:#x}')
    return dict(schema=1,scope='Complete local manager; scoped dispatcher/waits; no original execution',
                bodies=len(BODIES),instruction_anchors=len(ANCHORS),data_spans=len(CONSTANTS))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--iso',default=DEFAULT_ISO)
    p.add_argument('--output')
    a=p.parse_args();result=verify(a.iso)
    if a.output:
        output=Path(a.output)
        if output.resolve()==Path(a.iso).resolve():
            raise ValueError('Output must not overwrite ISO')
        output.parent.mkdir(parents=True,exist_ok=True)
        output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result,sort_keys=True))
