#!/usr/bin/env python3
"""Verify static US battle jump evidence directly against the read-only ISO.

Metadata only; no original instruction execution or measured timing claim.
Kept separate from the shared general verifier while concurrent research proceeds.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct

from iso_assets import DEFAULT_ISO, inventory
from rel_event_inventory import read_bindings

BODIES = {
    'btlevtcmd_JumpSetting': (0x8010d6cc, 0xa4, 'a1d72383b900351dfbe1983be1c0311f26c6ae8c858bb71a3a452613c6942bd8'),
    'btlevtcmd_JumpPosition': (0x8010d208, 0x4c4, '95e8c2ed3148fab609989cd67a56392c571ca158a575f03e9b5eeedf517d8c48'),
    'btlevtcmd_JumpContinue': (0x8010d0f0, 0x118, '7f097e6c45e700798a4d490f272ecbd614301cddbf0d6b8cd1afc5154bf7f1e0'),
    'evtGetFloat': (0x80030840, 0x4bc, 'f714b2a7ddc29e31034b9c52f37dfcf8cb0f8b21cf73c73a81da2ccc67e8e3d8'),
    'btlMovePos': (0x80125b70, 0xac, '6506071097c4c6784d1895ee3a6607773cd45d0fefed838d45a4e33a7c0956f9'),
    'BtlUnit_SetMoveCurrentPos': (0x80128210, 0x34, '8b32b760a277074981f836b12b036b5ee9ed3ac85df2b5c3845fa4aad6394dc9'),
    'BtlUnit_SetMoveTargetPos': (0x801281a8, 0x34, 'c0ae282de347e529a9708325fa00f8a4e7ad9b963b6c719c0326b06e1b24885c'),
    'BtlUnit_SetPos': (0x80128c6c, 0x10, '473beb07b076bbf52f19537606519838c337da49a6c284fb1761df1a7e936059'),
    'BtlUnit_GetPos': (0x80128c7c, 0x1c, 'b86b6dbcd68d6df4e550ce72488eb126842189c1f29b8df5e950221aa41da78c'),
    'BattleGetFloorHeight': (0x800f7b84, 0x50, 'da651e064b9f49e0f750687f1e78ba167ddb0a72cc618e4359ec849c1a865923'),
}
# Selected instructions independently transcribed during inspection.
ANCHORS = {
    0x8010d710:'4bf23b51', 0x8010d720:'4bf23121', 0x8010d730:'4bf23111',
    0x8010d744:'93e3016c', 0x8010d748:'d3c30170', 0x8010d74c:'d3e30174',
    0x8010d260:'4bf24001', 0x8010d318:'2c1c0000', 0x8010d32c:'981b000d',
    0x8010d334:'935e016c', 0x8010d388:'d03e017c', 0x8010d3a0:'d03e0180',
    0x8010d3dc:'807e016c', 0x8010d404:'fc00001e', 0x8010d430:'ec02007c',
    0x8010d468:'d01e0170', 0x8010d498:'4082000c', 0x8010d49c:'38600002',
    0x8010d500:'ec0100ba', 0x8010d530:'48019059', 0x8010d594:'4bfcc2b5',
    0x8010d5a4:'ec22082a', 0x8010d5b4:'ec220828', 0x8010d5d8:'d03e0158',
    0x8010d60c:'901e016c', 0x8010d618:'41810084', 0x8010d62c:'4801b641',
    0x8010d694:'38600001', 0x8010d69c:'38600000',
    0x8010d138:'41820030', 0x8010d164:'4801b0ad', 0x8010d178:'ec01002a',
    0x8010d188:'ec010028', 0x8010d1c0:'4bfea9c5', 0x8010d1cc:'40800024',
    0x8010d1e8:'38600001', 0x8010d1f0:'38600000',
    0x800308cc:'38041100', 0x800308dc:'3c7f0db6', 0x800308e4:'38638580',
    0x800308fc:'c00285dc', 0x80030908:'ec210032', 0x80030ce0:'ec200828',
    0x801281b4:'d0230160', 0x8012821c:'d0230154',
    0x80128c6c:'d023003c', 0x80128c80:'d0040000',
    0x80125ba8:'ec2300b2', 0x80125bb0:'efc10024', 0x80125bdc:'ec000132',
    0x80125be0:'d03e0000', 0x80125bec:'d01f0000',
    0x800f7ba0:'c0030b2c', 0x800f7bac:'4082000c', 0x800f7bb8:'c0230b34',
}
CONSTANTS = {
    0x802bffb0:'4330000080000000', 0x802ee608:'4330000080000000',
    0x8041fa1c:'3a800000', 0x80422280:'00000000', 0x804222a0:'3f000000',
    0x80422690:'40c90fdb', 0x80422694:'43b40000',
    0x804226c4:'cd6e6b28', 0x80422170:'00000000',
}


def verify(iso):
    info = inventory(iso)
    with Path(iso).open('rb') as disc:
        if disc.read(8) != b'G8ME01\0\0':
            raise ValueError('Expected US G8ME01 disc0 revision0')
        disc.seek(0x420)
        base = struct.unpack('>I',disc.read(4))[0]
        disc.seek(base)
        header = disc.read(256)
        def read(address,size):
            matches=[]
            for i in range(18):
                off,va,length=(struct.unpack_from('>I',header,j+i*4)[0] for j in (0,0x48,0x90))
                if va <= address and address+size <= va+length:
                    matches.append(base+off+address-va)
            if len(matches)!=1:
                raise ValueError(f'Unmapped/ambiguous DOL address {address:#x}')
            disc.seek(matches[0]);data=disc.read(size)
            if len(data)!=size:
                raise ValueError('Truncated DOL')
            return data
        for name,(address,size,digest) in BODIES.items():
            if hashlib.sha256(read(address,size)).hexdigest()!=digest:
                raise ValueError(f'Body mismatch: {name}')
        for address,expected in {**ANCHORS,**CONSTANTS}.items():
            if read(address,len(expected)//2).hex()!=expected:
                raise ValueError(f'Instruction/constant mismatch: {address:#x}')
        entry=next(f for f in info['files'] if f['path']=='rel/gor.rel')
        disc.seek(entry['offset']);harbor=disc.read(entry['size'])
        if hashlib.sha256(harbor).hexdigest()!='d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a':
            raise ValueError('Harbor identity mismatch')
        _,imports,_=read_bindings(harbor)
        expected_imports={0x655ac:0x8010d6cc,0x655c4:0x8010d208,
                          0x65700:0x8010d0f0,0x65784:0x8010d6cc,
                          0x6579c:0x8010d208,0x657e4:0x8010d6cc,
                          0x657fc:0x8010d208,0x658b4:0x8010d6cc,
                          0x658cc:0x8010d208}
        for site,target in expected_imports.items():
            if imports.get(site)!=target:
                raise ValueError(f'Jump import mismatch: {site:#x}')
        expected_settings={0x655a8:(0xfffffffe,0,0xf24a8280,0xf24a7b4c),
                           0x65780:(0xfffffffe,0,0,0xf24a7b4c),
                           0x657e0:(0xfffffffe,0,0,0xf24a7b4c),
                           0x658b0:(0xfffffffe,15,0,0xf24a7e80)}
        for offset,args in expected_settings.items():
            if struct.unpack_from('>I',harbor,offset)[0]!=0x5005b or struct.unpack_from('>4I',harbor,offset+8)!=args:
                raise ValueError(f'Jump setting operands mismatch: {offset:#x}')
        for offset,count in ((0x655c0,0),(0x65798,20),(0x657f8,10),(0x658c8,10)):
            if struct.unpack_from('>I',harbor,offset)[0]!=0x7005b or struct.unpack_from('>2i',harbor,offset+24)!=(count,-1):
                raise ValueError(f'Jump count/sound operands mismatch: {offset:#x}')
    return dict(schema=1,scope='Static US jump contracts; no execution or measured tick claim',
                disc_id='G8ME01',revision=0,
                bodies=[dict(name=n,address=a,size=s,sha256=d) for n,(a,s,d) in BODIES.items()],
                instruction_anchors=len(ANCHORS),constant_checks=len(CONSTANTS),
                harbor_import_checks=len(expected_imports),
                harbor_settings_checks=len(expected_settings),harbor_jump_checks=4)


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--iso',default=DEFAULT_ISO)
    p.add_argument('--output',type=Path)
    args=p.parse_args()
    if args.output and args.output.resolve()==Path(args.iso).resolve():
        p.error('Cannot overwrite ISO')
    result=verify(args.iso)
    if args.output:
        args.output.parent.mkdir(parents=True,exist_ok=True)
        args.output.write_text(json.dumps(result,indent=2)+'\n')
    print(f"Verified {len(BODIES)} body fingerprints, {len(ANCHORS)} instructions, "
          f"{len(CONSTANTS)} constants and original harbor jump bindings")


if __name__=='__main__':
    main()
