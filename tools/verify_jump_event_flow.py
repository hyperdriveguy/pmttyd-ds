#!/usr/bin/env python3
"""Verify US ordinary Jump request-flow and local AC/VM evidence read-only.

Metadata only. Does not execute original instructions or assert measured timing.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct

from dol_event_inventory import parse_commands
from iso_assets import DEFAULT_ISO

BODIES = {
    'btlevtcmd_MarioJumpParam': (0x80109c68,0x19c,'bf128edee4331114b3d4d4cd147fb2d0bb2580f6ea16f5add0eb11acd6d399d5'),
    # Distance helper identity only; frsqrte/FPSCR execution remains a dependency.
    'distABf': (0x8000722c,0xfc,'39b4215c799e9ba6f7d722334bd2fe0dab2b24c06514aedabc28c2adecd85165'),
    'btlevtcmd_GetResultCountAC': (0x80107548,0x34,'d22d7c93efd083c0ab04d4d0bd352908851b7b038d88851104548913da542cf4'),
    'btlevtcmd_GetResultAC': (0x80107660,0x34,'3a2c76926cdefc97a4eef4118bbab2912529502dc8f4824e41f59ba139a299dc'),
    'btlevtcmd_GetResultPrizeLv': (0x801075c4,0x9c,'6d3c6674c779159d5fdb18e886e613e08187d3d25a2995c47c979db035a0c33a'),
    'btlevtcmd_ac_timing_flag_onoff': (0x80106f40,0x60,'6339e74b1361639796e60050ddac6dae8335686f8eab9c7f38bd7fefce35297c'),
    'btlevtcmd_ac_timing_get_success_frame': (0x80106ef0,0x50,'63849b652db4b3a96360b91ee771c0752e584d3ad6ae3de10e6b5bbba9a14d87'),
    'btlevtcmd_StopAC': (0x80107758,0x30,'4d7b1e106bdb24f69e8bc557bc99d0c6c23b1f501fc01e222d70b177330724ef'),
    'BattleActionCommandStop': (0x800fa420,0x30,'827ac29f800c2815b5866d21a5b16ced663d678704089b88efc4e8654a4e0dc4'),
    'BattleActionCommandGetPrizeLv': (0x800fa0e8,0x44,'378c08e78a8712fb4bdd987c138139d6396e532b47586b17b9a8ee3010ca8ef5'),
    'battleAcDelete_TimingA': (0x8012a7c0,0xc,'b5bed9c540056780e1b5d2a5431edf510767f94f3c7f58a08087fd321ebb3343'),
    'evt_if_large_equal': (0x8003491c,0xcc,'699e95571b0cde49f36642cf27f4d3e2e5dd04288ae3e33cfc08c47c31d7b04c'),
    'evt_if_small_equal': (0x800349e8,0xcc,'17090720e5f21e44a847cbe4e028bc5ce74a6338e8955896e3dedc1903c54c8f'),
    'evt_if_large': (0x80034ab4,0xcc,'df55ab74269caac612cf148b8d61efd4b8e878242f71ce741ee23bec3361bc42'),
    'evt_if_not_flag': (0x8003479c,0xc0,'e1168efbbf1f375f19c691355000f1a393e4c7fb16baab268fc763218ebb287b'),
    'evt_case_large_equal': (0x80033f74,0x15c,'c3035f287de2de4956aad5a31e61ff56547073e30bc2df73a9d55c8df9727f6f'),
    'evt_brother_evt': (0x8003325c,0x94,'ee44e842fd6d431fb6ec668a29914a90ef04b71bc4a666c9e66ec3b101ff69f7'),
    'evt_inline_evt': (0x8003346c,0x15c,'6654b888d43085e0db5dad6ab6d125caec48434fabc7e6897b1ff2bea4baf613'),
    # Constructor fingerprint; only ownership-zeroing region is specified here.
    'evtEntryType': (0x80037220,0x28c,'28903071b0be6475f0a7c4f8db025adddf01389815b655f0e565120b1be61e8d'),
}
ANCHORS = {
    0x80109ca4:'4801b749',0x80109cb4:'4bf275ad',0x80109ce4:'4bf2757d',
    0x80109cf0:'4bf27571',0x80109d10:'839d0010',0x80109d20:'4bf27541',
    0x80109d48:'4801ef35',0x80109d5c:'4befd4d1',0x80109d60:'2c1d0000',
    0x80109d74:'ec010024',0x80109d78:'fc00001e',0x80109d8c:'40810020',
    0x80109d98:'ec010032',0x80109da8:'7c630214',0x80109db8:'40810008',
    0x80109dc4:'28004d80',0x80109dd8:'4bf26f25',
    0x80005418:'3c408042',0x8000541c:'60427440',
    0x80107560:'80a51cb4',0x80107678:'80a51cb8',
    0x80106f60:'809f0004',0x80106f70:'7c002378',0x80106f80:'7c002078',
    0x80106f10:'48023b55',0x80107768:'4bff2cb9',0x80107770:'4810f19d',
    0x800fa42c:'81831cac',0x800fa43c:'4e800421',0x8012a7c4:'90031c9c',
    0x800fa0f0:'80040278',0x800fa0f8:'2800001e',0x800fa108:'88031cbd',
    0x800fa110:'3463ffff',0x800fa11c:'2c030007',
    0x801075f0:'2c04ffff',0x801075f8:'2c04fffb',0x80107624:'83de0008',
    0x80034950:'7c1f1800',0x80034954:'40800078',
    0x80034a1c:'7c1f1800',0x80034a20:'40810078',
    0x80034ae8:'7c1f1800',0x80034aec:'41810078',
    0x800347c0:'801f0004',0x800347c4:'7c600039',0x800347c8:'41820078',
    0x80034020:'7c032000',0x80034024:'40810088',
    0x80033288:'2c050070',0x800332a4:'48003961',
    0x80033498:'2c05006d',0x800334b8:'48003d69',
    0x800372d8:'90fe006c',0x800372e0:'90fe0074',
    0x80032948:'48003db9',0x80032988:'48003d79',
}


def verify(iso):
    with Path(iso).open('rb') as disc:
        if disc.read(8) != b'G8ME01\0\0':
            raise ValueError('Expected US G8ME01 disc0 revision0')
        disc.seek(0x420)
        dol_offset=struct.unpack('>I',disc.read(4))[0]
        disc.seek(dol_offset)
        header=disc.read(256)
        def read(address,size):
            matches=[]
            for i in range(18):
                off,va,length=(struct.unpack_from('>I',header,j+i*4)[0] for j in (0,0x48,0x90))
                if va <= address and address+size <= va+length:
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
        for address,word in ANCHORS.items():
            if read(address,4).hex()!=word:
                raise ValueError(f'Instruction mismatch: {address:#x}')
        for address,value in {0x804222d0:'3e19999a',0x804222d4:'42700000',
                              0x802ee608:'4330000080000000'}.items():
            if read(address,len(value)//2).hex()!=value:
                raise ValueError('MarioJumpParam numerical constant mismatch')
        for opcode,target in {27:0x80031a7c,28:0x80031a8c,29:0x80031a9c,
                              31:0x80031abc,41:0x80031c10,107:0x80032934,
                              109:0x80032944,110:0x80032964,112:0x80032984}.items():
            if struct.unpack('>I',read(0x80304fd0+4*opcode,4))[0]!=target:
                raise ValueError('Jump VM dispatch binding mismatch')
        data=read(0x80355cf0,0xf0c)
        digest=hashlib.sha256(data).hexdigest()
        if digest!='2858f3f63cfb95dc84eb6ed3b363c530e24200063044f37afd275d3847f9720c':
            raise ValueError('Original ordinary Jump stream mismatch')
        commands=parse_commands(data,0x80355cf0,{})
        by_address={c['address']:(c['opcode'],tuple(o['raw'] for o in c['operands'])) for c in commands}
        def record(address,opcode,*operands):
            if by_address.get(address)!=(opcode,operands):
                raise ValueError(f'Jump request/branch mismatch: {address:#x}')
        unit=0xfffffffe
        local=lambda i:0xfe363c80+i
        for address,flags in ((0x80356230,0x100100),(0x80356254,0x100)):
            record(address,91,0x8010abac,unit,local(3),local(4),local(12),flags,local(5))
        for address,flags in ((0x80356600,0x100),(0x803567f8,0x20000),(0x803568a0,0x20100)):
            record(address,91,0x8010a78c,unit,local(3),local(4),local(12),flags,local(5))
        for address in (0x80355ebc,0x80356038):
            record(address,91,0x801077ec,unit,1,1,0)
        for address,on in ((0x803565a0,1),(0x803565d0,0)):
            record(address,91,0x80106f40,on,1)
        record(0x80356204,91,0x80107788,1)
        record(0x803565e0,91,0x80107708)
        record(0x803565e8,91,0x80107660,local(6))
        record(0x803565f4,31,local(6),2)
        record(0x8035679c,28,local(6),1)
        record(0x80356878,53,local(10),1)
        record(0x80356884,4,10)
        record(0x80356310,91,0x80109c68,unit,local(0),local(1),local(2),local(6))
        record(0x80356434,91,0x801093a8,unit,local(0),local(1),local(2),local(6),3,0xffffffff)
        record(0x803564a0,91,0x801093a8,unit,local(0),local(1),local(2),local(6),0,0xffffffff)
        labels=[args[0] for op,args in by_address.values() if op==3]
        if len(commands)!=268 or labels!=[10,90,95,99]:
            raise ValueError('Jump command/label census mismatch')
        expected_blocks=[(0x8035600c,110),(0x80356050,112),
                         (0x803560f0,110),(0x803561a8,112),
                         (0x8035632c,110),(0x80356430,112),
                         (0x803567a8,107),(0x803567f4,109),
                         (0x8035696c,110),(0x80356a70,112)]
        if [(a,op) for a,(op,_) in by_address.items() if op in (107,109,110,112)]!=expected_blocks:
            raise ValueError('Jump brother/inline ownership census mismatch')
        calls=[args[0] for op,args in by_address.values() if op==91]
        if any(target in calls for target in (0x80107a64,0x80107ad4,0x801100cc,0x801101c4)):
            raise ValueError('Unexpected Jump parameter/flag/HP callback census')
        record(0x80356be0,94,0x8034f2e4)
        record(0x80356bf4,2)
        record(0x80356bf8,1)
    return dict(schema=1,scope='Static US ordinary Jump requests/local services;no original execution',
                bodies=len(BODIES),instruction_anchors=len(ANCHORS),commands=len(commands),
                event_sha256=digest,labels=labels,concurrent_blocks=len(expected_blocks)//2)


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
