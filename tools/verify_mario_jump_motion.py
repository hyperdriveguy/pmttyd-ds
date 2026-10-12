#!/usr/bin/env python3
"""Verify US MarioJumpPosition motion/rounding/sound evidence read-only.

Checks original bytes and caller bindings, not original execution or tick timing.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct

from iso_assets import DEFAULT_ISO

BODIES = {
    'btlevtcmd_MarioJumpPosition': (0x801093a8,0x8c0,'bfe6ed6ef66602af64ebad20435d2f708346d851172f3978391411913a433c40'),
    'BtlUnit_LoadSeMode': (0x80126588,0x14c,'b1d0030827e03655df975dd419f4b6664362b255dbe2c45458802b1036ea8fa0'),
    'reviseAngle': (0x80007328,0x60,'e44664bf511be1e4b10e4f0b3e40004fbd1d412c40ce6b34fee14987db320ed5'),
    # Angle helper identity only; SFX wrapper is specified, its core remains open.
    'angleABf': (0x8000703c,0x1b4,'57be80b24f06af9f5dba17e69fa2969270f024e6932ddd39d4b943c829a6a301'),
    'psndSFXOn_3D': (0x800d9848,0x3c,'29fdab1a516235c9bfbb11635067a2f53e115e54d41985990fbd5e09b6d1e971'),
}
ANCHORS = {
    0x801093f8:'4801bff5',0x80109408:'4bf27e59',0x80109430:'4bf27e31',
    0x80109458:'4bf27e09',0x80109480:'4bf27de1',0x801094a0:'4bf27dc1',
    0x801094c0:'41820440',0x801094c8:'2c1f0001',0x801094d8:'981b000d',
    0x801094ec:'4801ecbd',0x80109514:'4801ecfd',0x8010952c:'d03d017c',
    0x80109544:'d03d0180',0x80109574:'935d016c',0x801095a8:'901d016c',
    0x801095f8:'d01d0170',0x80109618:'eca2007c',0x80109628:'38600002',
    0x8010963c:'2c1f0003',0x80109678:'d07d01a8',0x801096a0:'d07d01ac',
    0x801096c8:'d07d01b0',0x801096cc:'d03d0178',0x801096e8:'d03d0174',
    0x80109708:'ec22082a',0x80109710:'408000c4',0x80109718:'2c1f0002',
    0x8010974c:'d01d01b8',0x8010976c:'d01d0184',0x80109784:'d01d0184',
    0x801097bc:'fc2100fa',0x801097c4:'fc02007a',0x801097f8:'7c0303d6',
    0x80109808:'ec040028',0x80109844:'d01d0184',0x801098a0:'4801cce9',
    0x801098fc:'4bfcff4d',0x80109908:'2c000002',0x80109944:'ec01002a',
    0x80109894:'38bd01c4',0x80109898:'38dd018c',0x8010989c:'38600002',
    0x801098a4:'807d018c',
    0x80109974:'d01d01bc',0x8010999c:'ec4200fa',0x801099b8:'fc020040',
    0x801099c4:'2c000003',0x801099e4:'2c1f0003',0x80109a18:'fc2100fa',
    0x80109a20:'fc02007a',0x80109a80:'4befd8a9',0x80109a90:'901d016c',
    0x80109a9c:'4082019c',0x80109ac4:'ec210828',0x80109acc:'d03d0178',
    0x80109ad0:'d01d0174',0x80109adc:'4801d8a1',0x80109ae8:'38600002',
    0x80109b00:'ec000028',0x80109b34:'911d016c',0x80109b60:'fc200a10',
    0x80109b78:'981b000d',0x80109bac:'c042ae80',0x80109be0:'ec0100bc',
    0x80109c2c:'4082000c',0x80109c30:'38600001',0x80109c38:'38600000',
    0x80126590:'b0e60014',0x80126598:'90060018',0x80126634:'80050000',
    0x80126640:'90060004',0x801266a8:'b0060012',0x801266b4:'90060000',
    0x8000733c:'4826b285',0x80007348:'fc000000',0x80007364:'ec21002a',
}
CONSTANTS = {
    0x80422280:'00000000',0x804222bc:'43b40000',0x804222c0:'40c90fdb',
    0x804222c4:'40490fdb',0x804222c8:'42b40000',0x804222cc:'3fe66666',
    0x802ee608:'4330000080000000',
    0x802ee6f0:'4034000000000000',0x802ee6f8:'4018000000000000',
    0x802ee700:'4047800000000000',0x802ee708:'3ff1eb851eb851ec',
    0x802ee710:'3ff0000000000000',0x802ee718:'3fe0f5c28f5c28f6',
    0x802ee720:'3ff4000000000000',0x802ee728:'3fe999999999999a',
    0x802ee730:'4030800000000000',0x802ee738:'4040800000000000',
    0x8041f448:'00000000',0x8041f45c:'43b40000',
    0x802bf240:'4076800000000000',
}


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
            disc.seek(matches[0])
            data=disc.read(size)
            if len(data)!=size:
                raise ValueError('Truncated DOL')
            return data
        for name,(address,size,digest) in BODIES.items():
            if hashlib.sha256(read(address,size)).hexdigest()!=digest:
                raise ValueError(f'Body mismatch: {name}')
        for address,value in {**ANCHORS,**CONSTANTS}.items():
            if read(address,len(value)//2).hex()!=value:
                raise ValueError(f'Instruction/constant mismatch: {address:#x}')
        for address,mode in ((0x80356434,3),(0x803564a0,0)):
            if struct.unpack('>9I',read(address,36))!=(0x8005b,0x801093a8,
                0xfffffffe,0xfe363c80,0xfe363c81,0xfe363c82,0xfe363c86,mode,0xffffffff):
                raise ValueError('Ordinary Mario jump mode/count/sound binding mismatch')
    return dict(schema=1,scope='Static US Mario jump local body;no execution/tick claim',
                bodies=len(BODIES),instruction_anchors=len(ANCHORS),constants=len(CONSTANTS),
                ordinary_callers=2)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--iso',default=DEFAULT_ISO)
    p.add_argument('--output')
    a=p.parse_args()
    result=verify(a.iso)
    if a.output:
        output=Path(a.output)
        if output.resolve()==Path(a.iso).resolve():
            raise ValueError('Output must not overwrite ISO')
        output.parent.mkdir(parents=True,exist_ok=True)
        output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result,sort_keys=True))
