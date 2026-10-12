#!/usr/bin/env python3
"""Pin complete Majinai construction,pool acquisition and map publication requests."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'__memAlloc': (2147680496,
                100,
                '51d2010e5ae0c3265971a8dfaee60b24489d8bf8eb2ca386f77c1d0ceb98712b'),
 'effEntry': (2147871848, 104, '482732598b7e4aa96b36a33981a3e3d05e9e06a61b1ed3c16b2c1ecccde47776'),
 'effMajinaiEntry': (2149960064,
                     244,
                     'd9545f33b86640287924bc90daca6327e4d46ef02fa03aa0e52a95aa43b9cfac'),
 'effMajinaiMain': (2149959148,
                    916,
                    'e66b00e4891266ed7446d82c71a311de97634b90673e7e90d6221dedb28d680e'),
 'mapGetWork': (2147660856, 20, 'ddbc7aaeeb36ae24182217ef14c98d9a241a088126b1f39c314207b07ca13abc')}

ANCHORS = {2147660856: '800d1744',
 2147660860: '806d1740',
 2147660864: '1c0002f4',
 2147660868: '7c630214',
 2147660872: '4e800020',
 2147871848: '806d8fc0',
 2147871852: '80030000',
 2147871856: '80630004',
 2147871860: '7c0903a6',
 2147871864: '2c000000',
 2147871868: '40810018',
 2147871872: '80030000',
 2147871876: '28000000',
 2147871880: '4182000c',
 2147871884: '38630028',
 2147871888: '4200fff0',
 2147871892: '38800001',
 2147871896: '38000000',
 2147871900: '90830000',
 2147871904: '808d90b0',
 2147871908: '80a40014',
 2147871912: '7c8500d0',
 2147871916: '7c842b78',
 2147871920: '54840ffe',
 2147871924: '90830004',
 2147871928: '90030014',
 2147871932: '90030008',
 2147871936: '9003000c',
 2147871940: '90030010',
 2147871944: '98030018',
 2147871948: '4e800020',
 2149960064: '9421ffc0',
 2149960068: '7c0802a6',
 2149960072: '90010044',
 2149960076: 'dbe10030',
 2149960080: 'f3e10038',
 2149960084: 'dbc10020',
 2149960088: 'f3c10028',
 2149960092: 'dba10010',
 2149960096: 'f3a10018',
 2149960100: '93e1000c',
 2149960104: '93c10008',
 2149960108: 'ffa00890',
 2149960112: '7c7e1b78',
 2149960116: 'ffc01090',
 2149960120: 'ffe01890',
 2149960124: '4be022ad',
 2149960128: '3c808030',
 2149960132: '7c7f1b78',
 2149960136: '38842fa0',
 2149960140: '38000001',
 2149960144: '909f0014',
 2149960148: '38600003',
 2149960152: '901f0008',
 2149960156: '801f0008',
 2149960160: '54042834',
 2149960164: '4bdd370d',
 2149960168: '3c808026',
 2149960172: '907f000c',
 2149960176: '3804c5ec',
 2149960180: 'c00216fc',
 2149960184: '901f0010',
 2149960188: '38a000ff',
 2149960192: '38800000',
 2149960196: '3800ffff',
 2149960200: '80df0000',
 2149960204: '60c60002',
 2149960208: '90df0000',
 2149960212: '93c30000',
 2149960216: 'd3a30004',
 2149960220: 'd3c30008',
 2149960224: 'd3e3000c',
 2149960228: 'd0030010',
 2149960232: '90a30014',
 2149960236: '90830018',
 2149960240: '9003001c',
 2149960244: '4bdcea05',
 2149960248: '800216d0',
 2149960252: '9003001e',
 2149960256: '7fe3fb78',
 2149960260: 'e3e10038',
 2149960264: 'cbe10030',
 2149960268: 'e3c10028',
 2149960272: 'cbc10020',
 2149960276: 'e3a10018',
 2149960280: 'cba10010',
 2149960284: '83e1000c',
 2149960288: '80010044',
 2149960292: '83c10008',
 2149960296: '7c0803a6',
 2149960300: '38210040',
 2149960304: '4e800020'}

CONSTANTS = {2147505176: '3c40804260427440',
 2147505184: '3da0804161adcf20',
 2150641568: '4d616a696e616900',
 2151844624: 'ffffffff',
 2151844668: '3f800000'}

DIRECT_CALLS = {(2147759400, 2149960064), (2148615788, 2149960064), (2147934948, 2149960064)}

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
                if target==0x8025c980:
                    calls.add((site,target))
        if calls!=DIRECT_CALLS:
            raise ValueError(f'DOL branch-shaped direct call census differs: {sorted(calls)}')
        for name,(address,size,digest) in BODIES.items():
            if hashlib.sha256(read(address,size)).hexdigest()!=digest:
                raise ValueError(f'Body mismatch: {name}')
        for address,value in {**ANCHORS,**CONSTANTS}.items():
            if read(address,len(value)//2).hex()!=value:
                raise ValueError(f'Instruction/constant mismatch: {address:#x}')
    return dict(schema=1,scope='Complete Majinai constructor/effEntry/mapGetWork;allocator/update identities linked;no renderer/platform/execution claim',
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
