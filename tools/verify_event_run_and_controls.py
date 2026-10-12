#!/usr/bin/env python3
"""Check run helpers, ID variants and event controls contracts against read-only US ISO."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'evt_run_evt': (2147694312,
                 260,
                 '82b94cc320f0dfbbb020850984d5f774e6be9329a66202cf0db6e9a43459306e'),
 'evt_run_evt_id': (2147694024,
                    288,
                    '75e9a358844f564d64cbd0c182dea59015def56e25830cd56422fdd4d4f67aa6'),
 'evt_inline_evt_id': (2147693296,
                       380,
                       '6129d5989bbf34d9290f7b1c05d6c727123abd99b003811839bde658066f99d8'),
 'evt_brother_evt_id': (2147692972,
                        176,
                        '3440115a42fb703c48fae90861dea826c30199e0ba8b15910335086b5316a6ee'),
 'evtSetPri': (2147706364, 8, '84d8bbf213632b97e0c03db67c4f827df152a0a7c5c6e98b53de6af2fa1ff791'),
 'evtSetSpeed': (2147706348,
                 16,
                 '5144f5c22dafbc452594da5a77473a23375f80786ce3461bd4872e1fcfe56ef6'),
 'evtSetType': (2147706340, 8, 'dc9e91dc8b705cad9320a7b2c6eec3af472f47094c91edf5e54ed8bd2019ded6'),
 'evtStopOther': (2147703628,
                  136,
                  'c88864e4d4f0bcd8e8419c9c89de3295bdfc3a68e572da1ba3ecf6fc1fb71622'),
 'evtStartOther': (2147703492,
                   136,
                   '0b7c88804d78376b079375dbd72edfc07dfd4642f3ff15d54d5704849095467f'),
 'evtStopID': (2147704220, 160, 'c29afcd75d3368cda59cd7ba86fdfdc569e4f2470a91d7d1834f8fe2b96d343d'),
 'evtStartID': (2147704060,
                160,
                '908053c40e74d34c601791f4783d1e033b87fb52bdb5c94f5155327c3388cb08'),
 'evtCheckID': (2147706372, 96, '3a98a24e3d46852ee1f4a80dd51ff76dbacc0541fb68c824ac6032bc20ded5c5'),
 'evtmgrCmd': (2147686072,
               5048,
               '4b6cdc0e9b940137848236a344598d586d850790e0b25beda4b9ed81da069d0e'),
 'evtEntryType': (2147709472,
                  652,
                  '28903071b0be6475f0a7c4f8db025adddf01389815b655f0e565120b1be61e8d'),
 'evtChildEntry': (2147708652,
                   820,
                   '56b15399f281a3d5c484a0150028c0783b45fa22c06865f11daaf28266791b7e'),
 'evtBrotherEntry': (2147707908,
                     744,
                     '83d78011b51fca396a2975396e3bce010b5642804199af7265350f36cff9f3ef')}

ANCHORS = {2147694340: '4bffdb5d',
 2147694348: '38a00000',
 2147694356: '48003b0d',
 2147694368: '90030160',
 2147694380: '90040170',
 2147694532: '900400e4',
 2147694540: '90040154',
 2147694548: '90040158',
 2147694056: '4bffdc79',
 2147694060: '83de0004',
 2147694076: '48003c25',
 2147694256: '900500e4',
 2147694272: '90050158',
 2147694276: '80a5015c',
 2147694280: '4bffd635',
 2147693004: '83e30000',
 2147693028: '2c050070',
 2147693044: '907e0014',
 2147693056: '48003a05',
 2147693076: '90050160',
 2147693084: '9805000c',
 2147693108: '90050170',
 2147693112: '80a5015c',
 2147693116: '4bffdac1',
 2147693328: '83c40000',
 2147693352: '2c05006d',
 2147693368: '909f0014',
 2147693384: '48003ed9',
 2147693404: '90060160',
 2147693428: '90060170',
 2147693636: '900600e4',
 2147693640: '80a6015c',
 2147693644: '4bffd8b1',
 2147690412: '48004741',
 2147690420: '3bc000ff',
 2147690424: '981f000a',
 2147689564: '48004209',
 2147690552: '907f01a0',
 2147690560: '4800427d',
 2147690456: '48003e25',
 2147690480: '4bffe051',
 2147690488: '48003df5',
 2147690524: '48003dc9',
 2147690588: '4800340d',
 2147690616: '4800335d',
 2147690652: '480032b1',
 2147690688: '48003205',
 2147690716: '480034c1',
 2147690744: '48003405',
 2147690772: '83bd0004',
 2147690776: '48003ced',
 2147690792: '4bffe3d5',
 2147706364: '9883000b',
 2147706352: 'ec010032',
 2147706356: 'd0030164',
 2147706340: '9883000c',
 2147703708: '41820010',
 2147703720: '48000669',
 2147703732: '801f0000',
 2147703572: '41820010',
 2147703584: '4800031d',
 2147704312: '40820010',
 2147704320: '388000ef',
 2147704324: '4800040d',
 2147704336: '801f0000',
 2147704152: '40820010',
 2147704160: '388000ef',
 2147704164: '480000d9',
 2147706400: '80040000',
 2147706416: '4081002c',
 2147706440: '4082000c',
 2147706448: '4e800020'}

CONSTANTS = {2151807540: '00000000',
 2151764456: '00000001',
 2151764460: '3f800000',
 2147505184: '3da0804161adcf20',
 2150650176: '80032770',
 2150650180: '80032780',
 2150650184: '80032790',
 2150650188: '8003244c',
 2150650192: '80032828',
 2150650196: '800327c0',
 2150650200: '800327e4',
 2150650204: '80032804',
 2150650208: '8003284c',
 2150650212: '80032868',
 2150650216: '80032884',
 2150650220: '800328a8',
 2150650224: '800328cc',
 2150650228: '800328e8',
 2150650232: '80032904',
 2150650236: '80032934',
 2150650240: '80032954',
 2150650244: '80032944',
 2150650248: '80032964',
 2150650252: '80032974',
 2150650256: '80032984'}

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
    return dict(schema=1,scope='Complete run/ID helper and control wrappers; linked constructor identities; no execution claim',
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
