#!/usr/bin/env python3
"""Pin full event initialization/reset, linked heap/fill leaves and scoped direct callers in read-only US ISO."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'memset': (2147504308,
            48,
            '4176c17fce5f28ccea737946be853ea45a125b52e9acab356ba54454aa352b5c'),
 '__fill_mem': (2147504356,
                184,
                'd1f1a1fe917b2c7bb44a8a2f9e7aaccd8c0cd1849e5fb616f027fb22bb86ae12'),
 'DCFlushRange': (2150197844,
                  48,
                  '93cccdef2db2d4edd330b23e345500e30ae5ead17cfaea13e3c8ac963a39ab33'),
 'evtmgrInit': (2147711004,
                280,
                '1d00ceae20e44915faf432c019ee680f648650085a6627042bc5e282230eaf32'),
 'evtmgrReInit': (2147710896,
                  108,
                  '5a0e1a5af9008548501e81de999f0f6d12a44f4d9642941a46127a4205e29feb'),
 '__memAlloc': (2147680496,
                100,
                '51d2010e5ae0c3265971a8dfaee60b24489d8bf8eb2ca386f77c1d0ceb98712b'),
 'OSAllocFromHeap': (2150196396,
                     252,
                     'a6afaeb690d682a3a3b22ad68be70939eae39a63fcbcc5ba2ec991be08a05848'),
 'evt_msg_init': (2148347312,
                  4,
                  'f332ea5b5437103cbb6f1508679da89eec9288ad775c96c439a17fccabe3de8e')}

ANCHORS = {2147711004: '9421fff0',
 2147711016: '90010014',
 2147711020: '38000100',
 2147711028: '38600000',
 2147711032: '93e1000c',
 2147711036: '1c8001b0',
 2147711040: '90050000',
 2147711044: '4bff88ad',
 2147711052: '80ad90b0',
 2147711060: '38800000',
 2147711064: '90680090',
 2147711068: '80080000',
 2147711072: '80c50038',
 2147711076: '80e5003c',
 2147711080: '1ca001b0',
 2147711084: '90e8009c',
 2147711088: '90c80098',
 2147711092: '4bfcd841',
 2147711100: '38800000',
 2147711108: '38a00080',
 2147711116: '4bfcd829',
 2147711124: '38800000',
 2147711132: '38a0000c',
 2147711140: '4bfcd811',
 2147711148: '38000100',
 2147711156: '38600000',
 2147711160: '1c8001b0',
 2147711164: '901f00a0',
 2147711168: '4bff8831',
 2147711172: '907f0130',
 2147711176: '38800000',
 2147711180: '80ad90b0',
 2147711184: '801f00a0',
 2147711188: '80c50038',
 2147711192: '80e5003c',
 2147711196: '1ca001b0',
 2147711200: '90ff013c',
 2147711204: '90df0138',
 2147711208: '4bfcd7cd',
 2147711216: '38800000',
 2147711220: '38a00080',
 2147711224: '4bfcd7bd',
 2147711232: '38800000',
 2147711236: '38a0000c',
 2147711240: '4bfcd7ad',
 2147711244: '38000000',
 2147711248: '900d1940',
 2147711252: '900d193c',
 2147711256: '900d1938',
 2147711260: '4809b495',
 2147711264: '80010014',
 2147711268: '83e1000c',
 2147710896: '9421fff0',
 2147710908: '90010014',
 2147710916: '80cd90b0',
 2147710920: '80060014',
 2147710924: '2c000000',
 2147710936: '80060038',
 2147710940: '38800000',
 2147710944: '8066003c',
 2147710948: '9065009c',
 2147710952: '90050098',
 2147710956: '80050000',
 2147710960: '80650090',
 2147710964: '1ca001b0',
 2147710968: '4bfcd8bd',
 2147710972: '38000000',
 2147710976: '900d1940',
 2147710980: '900d1938',
 2147710984: '4809b5a9',
 2147710988: '80010014',
 2147680496: '9421fff0',
 2147680504: '90010014',
 2147680516: '93e1000c',
 2147680520: '93c10008',
 2147680528: '7c63002e',
 2147680532: '48266399',
 2147680548: '38800000',
 2147680552: '4bfd4f8d',
 2147680564: '48266921',
 2147680568: '80010014',
 2147680576: '83e1000c',
 2147680580: '83c10008',
 2150196396: '1c03000c',
 2150196400: '806d2150',
 2150196412: '80c50004',
 2150196416: '54030034',
 2150196424: '80060008',
 2150196428: '7c030000',
 2150196436: '80c60004',
 2150196440: '28060000',
 2150196448: '28060000',
 2150196456: '38600000',
 2150196464: '80060008',
 2150196472: '28000040',
 2150196480: '80860004',
 2150196484: '80650004',
 2150196488: '28040000',
 2150196496: '80060000',
 2150196500: '90040000',
 2150196504: '80860000',
 2150196508: '28040000',
 2150196516: '80660004',
 2150196524: '80060004',
 2150196528: '90040004',
 2150196532: '90650004',
 2150196540: '90660008',
 2150196548: '90040008',
 2150196552: '80060000',
 2150196556: '90040000',
 2150196560: '80060004',
 2150196564: '90040004',
 2150196568: '80640004',
 2150196572: '28030000',
 2150196580: '90830000',
 2150196584: '80640000',
 2150196588: '28030000',
 2150196596: '90830004',
 2150196604: '90850004',
 2150196608: '80650008',
 2150196612: '38000000',
 2150196616: '90660004',
 2150196620: '28030000',
 2150196624: '90060000',
 2150196632: '90c30000',
 2150196636: '90c50008',
 2147515196: '4802fc75',
 2147941856: '4bfc7a3d',
 2147957032: '806d90b0',
 2147957036: '38000001',
 2147957040: '90030014',
 2147957044: '4bfc3e7d',
 2147956604: '4bfc4035',
 2147956608: '4bfd95b9',
 2147956612: '808d90b0',
 2147956616: '38000000',
 2147956620: '38600004',
 2147956624: '90040014',
 2147956628: '4bfc2441'}

CONSTANTS = {2147505184: '3da0804161adcf20', 2151764456: '000000013f800000'}

DIRECT_CALLS = {(0x80007b3c,0x800377b0),(0x8006fde0,0x8003781c),
                (0x8007377c,0x800377b0),(0x80073934,0x800377b0)}

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
                if target in (0x8003781c,0x800377b0):
                    calls.add((site,target))
        if calls!=DIRECT_CALLS:
            raise ValueError(f'DOL branch-shaped direct call census differs: {sorted(calls)}')
        for name,(address,size,digest) in BODIES.items():
            if hashlib.sha256(read(address,size)).hexdigest()!=digest:
                raise ValueError(f'Body mismatch: {name}')
        for address,value in {**ANCHORS,**CONSTANTS}.items():
            if read(address,len(value)//2).hex()!=value:
                raise ValueError(f'Instruction/constant mismatch: {address:#x}')
    return dict(schema=1,scope='Complete local Init/ReInit/memory allocation; four scoped DOL calls; no source/execution equivalence claim',
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
