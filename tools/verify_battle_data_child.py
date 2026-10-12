#!/usr/bin/env python3
"""Verify complete unit-data child/ID translation and scoped original Mario key7 source."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'BattleGetMarioPtr': (2148501416,
                       108,
                       '26840147b00e408e2cd371d8c23ddd7057de2da195881a7ab014b758fa28f869'),
 'BattleGetPartyPtr': (2148501288,
                       128,
                       '618380e524d9f6805cd215b283aa4450e7e8fb392921ef72a3bec5b00cd8dd37'),
 'BattleGetSystemPtr': (2148501524,
                        96,
                        'a50cd9eb39e42632367e6b85a3e93f4ec37b28ea0580cca93b1cf8d593caa5f2'),
 'BattleGetUnitPtr': (2148501708,
                      60,
                      '6a40e068a63873b3f93f3cf0bdc72c59323e60ff5b961ac740b22eddb58276fa'),
 'BattleTransID': (2148684780,
                   180,
                   '0ee29df5fb9888083ca20bcdab1043e488a773a48ae78ab053286b73351e21c3'),
 'BtlUnit_Entry': (2148700128,
                   1716,
                   '28c393e0dda25fa7af7e6c82f477f4067e7e5e7f6b50d09766996228814707f9'),
 'BtlUnit_GetData': (2148692796,
                     64,
                     '1d86cd707b384f02d0716f3be4ceb19ca3fa49d5b16586a96005b4cd70056b8e'),
 'btlevtcmd_RunDataEventChild': (2148573200,
                                 160,
                                 'b2af79fafad773cfbd9319674aba10dfbecb715552f1be41795693c678ad7ecb'),
 'evtChildEntry': (2147708652,
                   820,
                   '56b15399f281a3d5c484a0150028c0783b45fa22c06865f11daaf28266791b7e'),
 'evtGetValue': (2147684960,
                 1112,
                 'ad7dce301c927dbc422a143f3ffb023aa32af8d4f3bb81c752fddc1b9e21bd7a')}

ANCHORS = {2148501524: '9421fff0',
 2148501528: '7c0802a6',
 2148501532: '90010014',
 2148501536: 'bfc10008',
 2148501540: '7c7e1b78',
 2148501544: '3be00000',
 2148501548: '7fc3f378',
 2148501552: '7fe4fb78',
 2148501556: '48000099',
 2148501560: '28030000',
 2148501564: '41820014',
 2148501568: '80030008',
 2148501572: '2c0000dc',
 2148501576: '40820008',
 2148501580: '48000014',
 2148501584: '3bff0001',
 2148501588: '2c1f0040',
 2148501592: '4180ffd4',
 2148501596: '38600000',
 2148501600: 'bbc10008',
 2148501604: '80010014',
 2148501608: '7c0803a6',
 2148501612: '38210010',
 2148501616: '4e800020',
 2148573200: '9421ffe0',
 2148573204: '7c0802a6',
 2148573208: '90010024',
 2148573212: 'bfa10014',
 2148573216: '7c7d1b78',
 2148573220: '83c30018',
 2148573224: '809e0000',
 2148573228: '4bf27235',
 2148573232: '7c601b78',
 2148573236: '7fa3eb78',
 2148573240: '7c040378',
 2148573244: '4801b3b1',
 2148573248: '7c7f1b78',
 2148573252: '806d1c70',
 2148573256: '7fe4fb78',
 2148573260: '4bfee881',
 2148573264: '809e0004',
 2148573268: '7c7e1b78',
 2148573272: '7fa3eb78',
 2148573276: '4bf27205',
 2148573280: '7c601b78',
 2148573284: '7fc3f378',
 2148573288: '7c040378',
 2148573292: '4801d2d1',
 2148573296: '7c641b79',
 2148573300: '4082000c',
 2148573304: '38600002',
 2148573308: '48000020',
 2148573312: '7fa3eb78',
 2148573316: '38a00000',
 2148573320: '4bf2ce65',
 2148573324: '38000000',
 2148573328: '981d000a',
 2148573332: '93e30160',
 2148573336: '386000ff',
 2148573340: 'bba10014',
 2148573344: '80010024',
 2148573348: '7c0803a6',
 2148573352: '38210020',
 2148573356: '4e800020',
 2148684780: '9421fff0',
 2148684784: '7c0802a6',
 2148684788: '2c040000',
 2148684792: '90010014',
 2148684796: '80ad1c70',
 2148684800: '4080008c',
 2148684804: '2c04fffd',
 2148684808: '41820034',
 2148684812: '4080001c',
 2148684816: '2c04fffb',
 2148684820: '41820060',
 2148684824: '40800034',
 2148684828: '2c04fffa',
 2148684832: '40800064',
 2148684836: '48000068',
 2148684840: '2c04ffff',
 2148684844: '41820040',
 2148684848: '4080005c',
 2148684852: '80630160',
 2148684856: '48000058',
 2148684860: '7ca32b78',
 2148684864: '4bfd3369',
 2148684868: '80630000',
 2148684872: '48000048',
 2148684876: '7ca32b78',
 2148684880: '4bfd32d9',
 2148684884: '28030000',
 2148684888: '4182000c',
 2148684892: '80630000',
 2148684896: '48000030',
 2148684900: '3860ffff',
 2148684904: '48000028',
 2148684908: '3860ffff',
 2148684912: '48000020',
 2148684916: '7ca32b78',
 2148684920: '4bfd339d',
 2148684924: '80630000',
 2148684928: '48000010',
 2148684932: '80650424',
 2148684936: '48000008',
 2148684940: '7c832378',
 2148684944: '80010014',
 2148684948: '7c0803a6',
 2148684952: '38210010',
 2148684956: '4e800020'}

CONSTANTS = {2147505184: '3da0804161adcf20',
 2150981144: '0003005b8010a010fffffffe00000007',
 2151098824: '0000000580373304000000068037337c0000003080416b3800000033803730fc0000003180372ff00000004080417c500000000780416bd00000004180372ce40000002f8034f4cc0000000000000000',
 2151099324: '803729c8',
 2151771088: '000000020000000100020032'}

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
    return dict(schema=1,scope='Complete data-child/translation/system query;linked entry/namespace/constructor/query bodies;Mario key7 source and tiny child;no original execution claim',bodies=len(BODIES),instruction_anchors=len(ANCHORS),data_spans=len(CONSTANTS))


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
