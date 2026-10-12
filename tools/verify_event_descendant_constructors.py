#!/usr/bin/env python3
"""Check descendant construction, inline/brother and restart contracts against read-only US ISO."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'evtChildEntry': (2147708652,
                   820,
                   '56b15399f281a3d5c484a0150028c0783b45fa22c06865f11daaf28266791b7e'),
 'evtBrotherEntry': (2147707908,
                     744,
                     '83d78011b51fca396a2975396e3bce010b5642804199af7265350f36cff9f3ef'),
 'evtRestart': (2147707580,
                328,
                '3454b4ed9de0e7d6413f75579c40767a8a8cbb003f98f704dfa9e356bddd7601'),
 'evt_inline_evt': (2147693676,
                    348,
                    '6654b888d43085e0db5dad6ab6d125caec48434fabc7e6897b1ff2bea4baf613'),
 'evt_brother_evt': (2147693148,
                     148,
                     'ee44e842fd6d431fb6ec668a29914a90ef04b71bc4a666c9e66ec3b101ff69f7'),
 'evtEntryType': (2147709472,
                  652,
                  '28903071b0be6475f0a7c4f8db025adddf01389815b655f0e565120b1be61e8d'),
 'evtEntryRunCheck': (2147710776,
                      120,
                      'eed033a42a12d2e19e6f5a0b309fbb8ba9ded89ba73f78ff481a12db11429b82')}

ANCHORS = {2147708744: '41820010',
 2147708780: '900d1940',
 2147708784: '93db0070',
 2147708796: '981b0008',
 2147708800: '4bfce135',
 2147708848: '937e006c',
 2147708868: '98de000b',
 2147708884: '90fe015c',
 2147708892: '90de0160',
 2147708908: '98de000c',
 2147708920: '90de0154',
 2147708928: '90de0158',
 2147708936: 'd03e0164',
 2147708940: 'd01e0168',
 2147708952: '90de0170',
 2147708960: '907e0000',
 2147708968: '90de017c',
 2147708976: '90de0178',
 2147708984: '90de0180',
 2147708996: '90fe0184',
 2147709000: '90de0188',
 2147709008: '90de018c',
 2147709016: '90de0190',
 2147709024: '90de009c',
 2147709168: '90de00e4',
 2147709312: '4080003c',
 2147709336: '7c7e01ae',
 2147709368: '4bffffa0',
 2147709380: '41820028',
 2147709404: '7fa4312e',
 2147709416: '900d193c',
 2147709424: '48000549',
 2147709444: '900d82c8',
 2147708036: '900d1940',
 2147708040: '4bfce42d',
 2147708088: '907e006c',
 2147708092: '937e0074',
 2147708096: '907e0070',
 2147708104: '98be000b',
 2147708120: '90fe015c',
 2147708144: '98be000c',
 2147708172: 'd03e0164',
 2147708176: 'd01e0168',
 2147708200: '80bb009c',
 2147708348: '90be00e4',
 2147708492: '4080003c',
 2147708516: '7c7e01ae',
 2147708548: '4bffffa0',
 2147708560: '41820028',
 2147708584: '7fa4312e',
 2147708596: '900d193c',
 2147708616: '900d82c8',
 2147708624: '48000869',
 2147707628: '909f0014',
 2147707632: '909f01a8',
 2147707636: '98bf000a',
 2147707640: 'd01f0164',
 2147707644: 'd03f0168',
 2147707648: '987f000e',
 2147707652: '987f000f',
 2147707660: 'd01f0164',
 2147707664: 'd03f0168',
 2147707668: '90bf0004',
 2147707672: '90bf0000',
 2147707696: '98640000',
 2147707700: '90c7002c',
 2147707816: '4080003c',
 2147707840: '7c7f01ae',
 2147707872: '4bffffa0',
 2147707880: '48000b51',
 2147693736: '909f0014',
 2147693752: '48003d69',
 2147693764: '90030160',
 2147693776: '90050154',
 2147693800: '9005017c',
 2147693848: '90050190',
 2147694000: '900500e4',
 2147693208: '907f0014',
 2147693220: '48003961',
 2147693236: '9803000c',
 2147693268: '90030150'}

CONSTANTS = {2151807536: '3f800000',
 2151807540: '00000000',
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
    return dict(schema=1,scope='Complete descendant/restart/107/110; root/run-check identities; no execution claim',
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
