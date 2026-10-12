#!/usr/bin/env python3
"""Pin complete battle fortune producer/pouch init and scoped direct DOL caller census."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'BattleMajinaiCheck': (2148497268,
                        336,
                        '9d9fcf58b7e24d625fdaf4b855f5f57ea52de0180b04dd94074f9c0f52efd49b'),
 'btlseqTurnFirstProcess': (2148643308,
                            240,
                            '020403148f2cdeb152b4a1e9ced298231a38922ba84791c0604d0f5fec62ca14'),
 'evtGetValue': (2147684960,
                 1112,
                 'ad7dce301c927dbc422a143f3ffb023aa32af8d4f3bb81c752fddc1b9e21bd7a'),
 'evt_pouch_majinai_set': (2148468908,
                           48,
                           'bd333bf4658e3a933a17cee9b8466841d029376962dc7f13d1237e090848535f'),
 'irand': (2147507616, 68, '0523fae52d8e314f0e46dafe5c3f04f5c4c90ced7628011fdb0cf4efa4ba0daf'),
 'pouchGetPtr': (2148359856, 8, 'dbcd2f415efe4aa4e5684ee546cf73d97cc641b5a0290f1b46b14ab606786537'),
 'pouchMajinaiInit': (2148348372,
                      48,
                      '5da1f0681fd1a6a6722e783139dd5f13e3d8bb705ccfcce7dc821c2f579e27e2'),
 'rand': (2150023972, 32, '6b19fd52fb70db058f43afa9f5b9f1922cdbd69d70b461055e6ba104576326b5')}

ANCHORS = {2148348372: '808d1be0',
 2148348376: '880405b8',
 2148348380: '7c001800',
 2148348384: '4c800020',
 2148348388: '986405b8',
 2148348392: '3880ffff',
 2148348396: '38000000',
 2148348400: '806d1be0',
 2148348404: '988305b9',
 2148348408: '806d1be0',
 2148348412: '980305ba',
 2148348416: '4e800020',
 2148468908: '9421fff0',
 2148468912: '7c0802a6',
 2148468916: '90010014',
 2148468920: '80830018',
 2148468924: '80840000',
 2148468928: '4bf409a1',
 2148468932: '4bfe2911',
 2148468936: '80010014',
 2148468940: '38600002',
 2148468944: '7c0803a6',
 2148468948: '38210010',
 2148468952: '4e800020',
 2148497268: '9421fff0',
 2148497272: '7c0802a6',
 2148497276: '90010014',
 2148497280: '93e1000c',
 2148497284: '4bfde72d',
 2148497288: '7c7f1b78',
 2148497292: '80ad1c70',
 2148497296: '880305b8',
 2148497300: '28000000',
 2148497304: '4082000c',
 2148497308: '38600000',
 2148497312: '48000110',
 2148497316: '889f05b9',
 2148497320: '7c800775',
 2148497324: '40800030',
 2148497328: '38000000',
 2148497332: '38600005',
 2148497336: '981f05ba',
 2148497340: '4bf0e5e5',
 2148497344: '38030006',
 2148497348: '38600000',
 2148497352: '981f05b9',
 2148497356: '889f05b9',
 2148497360: '3804ffff',
 2148497364: '981f05b9',
 2148497368: '480000d8',
 2148497372: '887f05ba',
 2148497376: '28030000',
 2148497380: '41820008',
 2148497384: '480000c8',
 2148497388: '3804ffff',
 2148497392: '981f05b9',
 2148497396: '881f05b9',
 2148497400: '7c000775',
 2148497404: '4081000c',
 2148497408: '38600000',
 2148497412: '480000ac',
 2148497416: '38000000',
 2148497420: '981f05b9',
 2148497424: '80652738',
 2148497428: '8063000c',
 2148497432: '80030004',
 2148497436: '540006b5',
 2148497440: '41820040',
 2148497444: '38600064',
 2148497448: '4bf0e579',
 2148497452: '2c03002d',
 2148497456: '40800010',
 2148497460: '38000001',
 2148497464: '981f05ba',
 2148497468: '48000070',
 2148497472: '2c03005a',
 2148497476: '40800010',
 2148497480: '38000002',
 2148497484: '981f05ba',
 2148497488: '4800005c',
 2148497492: '38000003',
 2148497496: '981f05ba',
 2148497500: '48000050',
 2148497504: '38600064',
 2148497508: '4bf0e53d',
 2148497512: '2c03001e',
 2148497516: '40800010',
 2148497520: '38000001',
 2148497524: '981f05ba',
 2148497528: '48000034',
 2148497532: '2c03003c',
 2148497536: '40800010',
 2148497540: '38000002',
 2148497544: '981f05ba',
 2148497548: '48000020',
 2148497552: '2c030050',
 2148497556: '40800010',
 2148497560: '38000003',
 2148497564: '981f05ba',
 2148497568: '4800000c',
 2148497572: '38000004',
 2148497576: '981f05ba',
 2148497580: '887f05ba',
 2148497584: '80010014',
 2148497588: '83e1000c',
 2148497592: '7c0803a6',
 2148497596: '38210010',
 2148497600: '4e800020'}

CONSTANTS = {2147505184: '3da0804161adcf20'}

DIRECT_CALLS = {(2148643356, 2148497268), (2148468932, 2148348372)}

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
        calls=set()
        for i in range(7):
            va,size=(struct.unpack_from('>I',header,j+i*4)[0] for j in (0x48,0x90))
            if not size:
                continue
            code=read(va,size)
            for offset in range(0,len(code)-3,4):
                word=struct.unpack_from('>I',code,offset)[0]
                if word>>26!=18 or not word&1:
                    continue
                displacement=word&0x03fffffc
                if displacement&0x02000000:
                    displacement-=0x04000000
                site=va+offset
                target=(displacement if word&2 else site+displacement)&0xffffffff
                if target in (0x800f7774,0x800d31d4):
                    calls.add((site,target))
        if calls!=DIRECT_CALLS:
            raise ValueError(f'DOL branch-shaped direct call census differs: {sorted(calls)}')
        for name,(address,size,digest) in BODIES.items():
            if hashlib.sha256(read(address,size)).hexdigest()!=digest:
                raise ValueError(f'Body mismatch: {name}')
        for address,value in {**ANCHORS,**CONSTANTS}.items():
            if read(address,len(value)//2).hex()!=value:
                raise ValueError(f'Instruction/constant mismatch: {address:#x}')
    return dict(schema=1,scope='Complete fortune producer/pouch init;RNG and first-process linked;scoped DOL callers;no source/runtime execution claim',
                bodies=len(BODIES),instruction_anchors=len(ANCHORS),data_spans=len(CONSTANTS),
                dol_direct_call_sites=len(DIRECT_CALLS))


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
