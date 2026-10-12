#!/usr/bin/env python3
"""Pin complete native-requested first-attack pair,target and called query graph."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'BattleGetFirstAttackUnit': (2148526840,
                              208,
                              '361edbb3df70064ceef1d89b11242ea0370feb71d7babc795cdc3a3ad653e71f'),
 'BattleGetMarioPtr': (2148501416,
                       108,
                       '26840147b00e408e2cd371d8c23ddd7057de2da195881a7ab014b758fa28f869'),
 'BattleGetPartnerPtr': (2148501236,
                         52,
                         'ea797ccf16fc01e642a86eac8ad121249ac3733be087851af5be88f3224c14ce'),
 'BattleGetPartyPtr': (2148501288,
                       128,
                       '618380e524d9f6805cd215b283aa4450e7e8fb392921ef72a3bec5b00cd8dd37'),
 'BattleGetUnitPtr': (2148501708,
                      60,
                      '6a40e068a63873b3f93f3cf0bdc72c59323e60ff5b961ac740b22eddb58276fa'),
 'BtlUnit_GetBodyPartsId': (2148699288,
                            64,
                            '21ed3890883c9f9c73389ff655cd579de1edc7fd1aabc7dce0101ecc93ff811e'),
 'btlevtcmd_GetFirstAttackTarget': (2148578328,
                                    120,
                                    '91c5eb8d14cc792a697ec3c2414efb5ce1e2c9da4161236bf1a082e74df955f4'),
 'evtSetValue': (2147683580,
                 1024,
                 '9c52991a1d1db6731e485516b6ed17a63ea2c48930759cd8a1ccbe3d1c1d3c46')}

ANCHORS = {2148501236: '9421fff0',
 2148501240: '7c0802a6',
 2148501244: '90010014',
 2148501248: '80040008',
 2148501252: '2c0000de',
 2148501256: '4082000c',
 2148501260: '4800001d',
 2148501264: '48000008',
 2148501268: '48000095',
 2148501272: '80010014',
 2148501276: '7c0803a6',
 2148501280: '38210010',
 2148501284: '4e800020',
 2148501288: '9421fff0',
 2148501292: '7c0802a6',
 2148501296: '90010014',
 2148501300: 'bfc10008',
 2148501304: '7c7e1b78',
 2148501308: '3be00000',
 2148501312: '7fc3f378',
 2148501316: '7fe4fb78',
 2148501320: '48000185',
 2148501324: '28030000',
 2148501328: '41820034',
 2148501332: '8003001c',
 2148501336: '54000739',
 2148501340: '40820028',
 2148501344: '80030008',
 2148501348: '2c0000e0',
 2148501352: '4180001c',
 2148501356: '2c0000e7',
 2148501360: '40800014',
 2148501364: '8803000c',
 2148501368: '7c000775',
 2148501372: '40820008',
 2148501376: '48000014',
 2148501380: '3bff0001',
 2148501384: '2c1f0040',
 2148501388: '4180ffb4',
 2148501392: '38600000',
 2148501396: 'bbc10008',
 2148501400: '80010014',
 2148501404: '7c0803a6',
 2148501408: '38210010',
 2148501412: '4e800020',
 2148501416: '9421fff0',
 2148501420: '7c0802a6',
 2148501424: '90010014',
 2148501428: 'bfc10008',
 2148501432: '7c7e1b78',
 2148501436: '3be00000',
 2148501440: '7fc3f378',
 2148501444: '7fe4fb78',
 2148501448: '48000105',
 2148501452: '28030000',
 2148501456: '41820020',
 2148501460: '80030008',
 2148501464: '2c0000de',
 2148501468: '40820014',
 2148501472: '8803000c',
 2148501476: '7c000775',
 2148501480: '40820008',
 2148501484: '48000014',
 2148501488: '3bff0001',
 2148501492: '2c1f0040',
 2148501496: '4180ffc8',
 2148501500: '38600000',
 2148501504: 'bbc10008',
 2148501508: '80010014',
 2148501512: '7c0803a6',
 2148501516: '38210010',
 2148501520: '4e800020',
 2148501708: '2c04ffff',
 2148501712: '4082000c',
 2148501716: '38600000',
 2148501720: '4e800020',
 2148501724: '4080000c',
 2148501728: '38600000',
 2148501732: '4e800020',
 2148501736: '2c040040',
 2148501740: '40800014',
 2148501744: '5480103a',
 2148501748: '7c630214',
 2148501752: '80630020',
 2148501756: '4e800020',
 2148501760: '38600000',
 2148501764: '4e800020',
 2148526840: '9421ffe0',
 2148526844: '7c0802a6',
 2148526848: '90010024',
 2148526852: 'bf61000c',
 2148526856: '7c7b1b78',
 2148526860: '7c9c2378',
 2148526864: '7cbd2b78',
 2148526868: '3bc00000',
 2148526872: '7f63db78',
 2148526876: '7fc4f378',
 2148526880: '4bff9dad',
 2148526884: '7c7f1b79',
 2148526888: '41820010',
 2148526892: '881f000c',
 2148526896: '2c000001',
 2148526900: '41820010',
 2148526904: '3bde0001',
 2148526908: '2c1e0040',
 2148526912: '4180ffd8',
 2148526916: '807b2738',
 2148526920: '80030008',
 2148526924: '2c000009',
 2148526928: '41820050',
 2148526932: '4080004c',
 2148526936: '2c000007',
 2148526940: '40800024',
 2148526944: '2c000001',
 2148526948: '40800008',
 2148526952: '48000038',
 2148526956: '7f63db78',
 2148526960: '4bff9c39',
 2148526964: '7fe0fb78',
 2148526968: '7c7f1b78',
 2148526972: '48000030',
 2148526976: '7f63db78',
 2148526980: '4bff9c25',
 2148526984: '7c641b78',
 2148526988: '7f63db78',
 2148526992: '4bff9b65',
 2148526996: '7fe0fb78',
 2148527000: '7c7f1b78',
 2148527004: '48000010',
 2148527008: '7f63db78',
 2148527012: '4bff9c05',
 2148527016: '7c601b78',
 2148527020: '93fc0000',
 2148527024: '901d0000',
 2148527028: 'bb61000c',
 2148527032: '80010024',
 2148527036: '7c0803a6',
 2148527040: '38210020',
 2148527044: '4e800020',
 2148578328: '9421ffe0',
 2148578332: '7c0802a6',
 2148578336: '90010024',
 2148578340: '3881000c',
 2148578344: '38a10008',
 2148578348: 'bf810010',
 2148578352: '7c7c1b78',
 2148578356: '80dc0018',
 2148578360: '806d1c70',
 2148578364: '83e60000',
 2148578368: '83c60004',
 2148578372: '4bff36b5',
 2148578376: '80610008',
 2148578380: '4801d84d',
 2148578384: '80810008',
 2148578388: '7c7d1b78',
 2148578392: '7f83e378',
 2148578396: '80a40000',
 2148578400: '7fe4fb78',
 2148578404: '4bf25899',
 2148578408: '7f83e378',
 2148578412: '7fc4f378',
 2148578416: '7fa5eb78',
 2148578420: '4bf25889',
 2148578424: 'bb810010',
 2148578428: '38600002',
 2148578432: '80010024',
 2148578436: '7c0803a6',
 2148578440: '38210020',
 2148578444: '4e800020',
 2148699288: '28030000',
 2148699292: '3880ffff',
 2148699296: '41820030',
 2148699300: '80630014',
 2148699304: '48000020',
 2148699308: '800301ac',
 2148699312: '540007ff',
 2148699316: '41820010',
 2148699320: '80630004',
 2148699324: '80830000',
 2148699328: '48000010',
 2148699332: '80630000',
 2148699336: '28030000',
 2148699340: '4082ffe0',
 2148699344: '7c832378',
 2148699348: '4e800020'}

CONSTANTS = {2147505184: '3da0804161adcf20'}

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
    return dict(schema=1,scope='Complete first-attack target/pair/getter/body-part graph;evtSetValue identity linked;no source/Game/runtime closure',bodies=len(BODIES),instruction_anchors=len(ANCHORS),data_spans=len(CONSTANTS))


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
