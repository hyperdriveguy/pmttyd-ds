#!/usr/bin/env python3
"""Check root constructors, label scanning and run-check contracts against read-only US ISO."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'evtEntry': (2147710124, 652, 'b5e0d48f0daf7c6a6dd64a265e44788958d5922cc1b75be7445542d277171718'),
 'evtEntryType': (2147709472,
                  652,
                  '28903071b0be6475f0a7c4f8db025adddf01389815b655f0e565120b1be61e8d'),
 'evtEntryRunCheck': (2147710776,
                      120,
                      'eed033a42a12d2e19e6f5a0b309fbb8ba9ded89ba73f78ff481a12db11429b82'),
 'evtStopAll': (2147703912,
                148,
                'af4f97d2a90bab2e2687497985b60f98c05b6283b648813c95ae54a9c47746ac'),
 'evtStop': (2147705360, 980, 'cd93f55afa69c7ad1a57626848d18530200b5dc277caeccdedd48335d5acef2d')}

ANCHORS = {2147710204: '4081001c',
 2147710216: '41820010',
 2147710252: '900d1940',
 2147710256: '4bfcdb85',
 2147710268: '981e0008',
 2147710284: '937e0014',
 2147710296: '937e01a0',
 2147710300: '937e01a8',
 2147710320: '9b9e000b',
 2147710332: '90ad82c8',
 2147710336: '90de015c',
 2147710340: '909e0160',
 2147710344: '989e000e',
 2147710352: '987e000c',
 2147710360: 'c02d82cc',
 2147710364: 'd03e0164',
 2147710368: 'd01e0168',
 2147710484: '98830000',
 2147710488: '90c8002c',
 2147710564: '80050000',
 2147710568: '84650004',
 2147710576: '7c008670',
 2147710604: '4080003c',
 2147710628: '7c7e01ae',
 2147710632: '90a7002c',
 2147710660: '4bffffa0',
 2147710672: '41820034',
 2147710684: '41820028',
 2147710708: '7fa4312e',
 2147710716: '7c83312e',
 2147710720: '900d193c',
 2147710728: '48000031',
 2147710748: '900d82c8',
 2147709568: '41820010',
 2147709604: '900d1940',
 2147709608: '4bfcde0d',
 2147709684: '90be015c',
 2147709700: '9b9e000c',
 2147709712: 'd03e0164',
 2147709716: 'd01e0168',
 2147709912: '80050000',
 2147709916: '84650004',
 2147709952: '4080003c',
 2147709976: '7c7e01ae',
 2147709980: '90a7002c',
 2147710008: '4bffffa0',
 2147710020: '41820034',
 2147710032: '41820028',
 2147710056: '7fa4312e',
 2147710064: '7c83312e',
 2147710068: '900d193c',
 2147710076: '480002bd',
 2147710096: '900d82c8',
 2147710788: '800d1a20',
 2147710836: '38600001',
 2147710840: '4bffe4f1',
 2147710848: '38600002',
 2147710852: '4bffe4e5',
 2147710860: '38600010',
 2147710864: '4bffe4d9',
 2147710872: '386000ef',
 2147710876: '4bffe4cd'}

CONSTANTS = {2151807540: '00000000',
 2151764456: '00000001',
 2151764460: '3f800000',
 2147505184: '3da0804161adcf20'}

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
    return dict(schema=1,scope='Complete root/typed/run-check; linked stop identities; no execution claim',
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
