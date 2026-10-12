#!/usr/bin/env python3
"""Pin complete parent battle main service order and sequence bank dispatch."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'BattleAcHelpMain': (2149673124,
                      100,
                      'ac32c584aebdfe843e27b481f92a6cb19d43d3f0766a769394f8135809ebb320'),
 'BattleActionCommandManager': (2148509104,
                                124,
                                '66404be99cc28c734fec611846176691e59af5c913523e1528cd7c114d0a0df6'),
 'BattleAudience_Main': (2149221228,
                         140,
                         '42db3ae62cdf99511f6b4bcf72933bcda669c62dd1b27a0f09c7183b0811993a'),
 'BattleBreakSlot_Main': (2149595808,
                          5596,
                          '3d1402e7ea435de74ede23e837a7f2252544e3f3d58179ba8a1c3365c4dec01b'),
 'BattleCheckAllPinchStatus': (2148659160,
                               532,
                               '81408f22ef2f41e6fcf122b802699ad30b49b51a2a7a80ada526022062c23b9d'),
 'BattleCheckUnitBroken': (2148498216,
                           92,
                           '97054b11da69e749c91e44e1ae83f33ac6619338322e35136a653defe689898e'),
 'BattleGetSeq': (2148501920,
                  104,
                  '00dd741e83176881d659d4201d6c68033d217de3a408dce9965d23774520ab60'),
 'BattleIconMain': (2149228904,
                    192,
                    'ccdd8867bed5ceb45a2d9de9aef1b3b081f2a3d6e1ca1a10196035342e65c6d9'),
 'BattleMain': (2148503080,
                220,
                'bf6aee153d46d818476c124121653739778fdcc2d73a958962b95251be990f96'),
 'BattlePadManager': (2148639804,
                      80,
                      '8003c3ad38eb531c51c884b9aadff74101fc69bb5e8035bda9e1819f7517d55b'),
 'BattleSequenceManager': (2148659692,
                           176,
                           '8b0a2896f38e2aaddc49efb4fe2edfa6e4a8c7949a7dba0ba49d6bbef14072b1'),
 'BattleSetSeq': (2148502024,
                  96,
                  'c37da15d68a33b6a9bc520d6c77c96a476750cfbb01c9512d645c893f498479d'),
 'BattleStageMain': (2148766032,
                     1584,
                     'f61083bae0f1b5fcb95e3490e3fa3216a3f34ebe9838dd5bf1440a1ee11f62e5'),
 'BattleStageObjectMain': (2148833176,
                           1080,
                           'f54092d325edbfc3f19bc7571087be134bc24f9b468040665ee79bb3870c79f4'),
 'battleMain': (2148503628,
                72,
                '748302b617d70e9e5e45de1a2f9248cd2100f499a5f5a04ff9a281acaa96e28f'),
 'btlDispMain': (2148541680,
                 2964,
                 '6281c04f5a9784643665fd82f9f0bbc37f053f52fb2cdf672076049a4c17802f'),
 'btlseqEnd': (2149667380,
               5232,
               'ea77420a93d18acc970cb2ad2402cea39b6fa7b3e72953ebc9bf5759113e0cb0'),
 'btlseqFirstAct': (2148656576,
                    2040,
                    '58885b6803df68a6e807afd45fe5d04ec0ed1da0b5fbf40a03506cec9b304291'),
 'btlseqInit': (2148658752,
                408,
                '1e2842e3f02393f304dee29a856d0be5bea2c2913c9bd9f95de6c65e6142b746'),
 'btlseqTurn': (2148648208,
                7896,
                '676aec347124b77cb7fbe40caeb81ff8b87fabd2ddd7ca53b96287d28b21a093')}

ANCHORS = {2148503080: '9421fff0',
 2148503084: '7c0802a6',
 2148503088: '90010014',
 2148503092: '93e1000c',
 2148503096: '806d1c70',
 2148503100: '7c7f1b78',
 2148503104: '4bffece9',
 2148503108: '480215f9',
 2148503112: '480263a5',
 2148503116: '7fe3fb78',
 2148503120: '48001761',
 2148503124: '48050945',
 2148503128: '480b1311',
 2148503132: '480402f5',
 2148503136: '4811da45',
 2148503140: '480af509',
 2148503144: '4810ac39',
 2148503148: '48009685',
 2148503152: '7fe3fb78',
 2148503156: '38800000',
 2148503160: '4bfffb29',
 2148503164: '2c030004',
 2148503168: '4082000c',
 2148503172: '38600000',
 2148503176: '48000068',
 2148503180: '806d90b0',
 2148503184: '80630000',
 2148503188: '546004e7',
 2148503192: '41820054',
 2148503196: '546004a5',
 2148503200: '4182004c',
 2148503204: '7fe3fb78',
 2148503208: '38800000',
 2148503212: '4bfffaf5',
 2148503216: '2c030002',
 2148503220: '40820038',
 2148503224: '801f0ef4',
 2148503228: '7fe3fb78',
 2148503232: '38800000',
 2148503236: '38a00003',
 2148503240: '60000060',
 2148503244: '901f0ef4',
 2148503248: '4bfffb39',
 2148503252: '7fe3fb78',
 2148503256: '38800007',
 2148503260: '3ca00700',
 2148503264: '4bfffb29',
 2148503268: '38600001',
 2148503272: '48000008',
 2148503276: '38600001',
 2148503280: '80010014',
 2148503284: '83e1000c',
 2148503288: '7c0803a6',
 2148503292: '38210010',
 2148503296: '4e800020',
 2148503628: '9421fff0',
 2148503632: '7c0802a6',
 2148503636: '90010014',
 2148503640: '93e1000c',
 2148503644: '83ed1c70',
 2148503648: '281f0000',
 2148503652: '4182001c',
 2148503656: '4bfffdc1',
 2148503660: '2c030000',
 2148503664: '40820010',
 2148503668: '801f0ef4',
 2148503672: '64008000',
 2148503676: '901f0ef4',
 2148503680: '80010014',
 2148503684: '83e1000c',
 2148503688: '7c0803a6',
 2148503692: '38210010',
 2148503696: '4e800020',
 2148659692: '9421fff0',
 2148659696: '7c0802a6',
 2148659700: '38800000',
 2148659704: '90010014',
 2148659708: '93e1000c',
 2148659712: '806d1c70',
 2148659716: '7c7f1b78',
 2148659720: '4bfd9799',
 2148659724: '2c030002',
 2148659728: '41820058',
 2148659732: '40800014',
 2148659736: '2c030000',
 2148659740: '4182001c',
 2148659744: '40800030',
 2148659748: '48000064',
 2148659752: '2c030004',
 2148659756: '4182005c',
 2148659760: '40800058',
 2148659764: '4800004c',
 2148659768: '7fe3fb78',
 2148659772: '4bfffc05',
 2148659776: '7fe3fb78',
 2148659780: '38800001',
 2148659784: '4bfffd91',
 2148659788: '4800003c',
 2148659792: '7fe3fb78',
 2148659796: '38800000',
 2148659800: '4bfffd81',
 2148659804: '7fe3fb78',
 2148659808: '4bfff361',
 2148659812: '48000024',
 2148659816: '7fe3fb78',
 2148659820: '38800000',
 2148659824: '4bfffd69',
 2148659828: '7fe3fb78',
 2148659832: '4bffd299',
 2148659836: '4800000c',
 2148659840: '7fe3fb78',
 2148659844: '480f5fb1',
 2148659848: '80010014',
 2148659852: '83e1000c',
 2148659856: '7c0803a6',
 2148659860: '38210010',
 2148659864: '4e800020'}

CONSTANTS = {2147505184: '3da0804161adcf20'}

CALL_SITES = {2148503112: 2148659692, 2148503656: 2148503080}

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
        sites={}
        for section in range(7):
            va,length=(struct.unpack_from('>I',header,j+4*section)[0] for j in (0x48,0x90))
            if not va: continue
            code=read(va,length)
            for offset in range(0,length,4):
                address=va+offset
                word=struct.unpack_from('>I',code,offset)[0]
                if word>>26!=18 or not word&1: continue
                delta=word&0x3fffffc
                if delta&0x2000000: delta-=0x4000000
                target=(delta if word&2 else address+delta)&0xffffffff
                if target in (0x800f8e28,0x8011f1ec): sites[address]=target
        if sites!=CALL_SITES: raise ValueError('Parent battle direct DOL caller census mismatch')
        for name,(address,size,digest) in BODIES.items():
            if hashlib.sha256(read(address,size)).hexdigest()!=digest:
                raise ValueError(f'Body mismatch: {name}')
        for address,value in {**ANCHORS,**CONSTANTS}.items():
            if read(address,len(value)//2).hex()!=value:
                raise ValueError(f'Instruction/constant mismatch: {address:#x}')
    return dict(schema=1,scope='Complete uppercase Main sequence manager and lowercase outer hook;source globals and scoped direct callers pinned;callee/field/end/query/Game/runtime remain open',dol_direct_call_sites=len(sites),bodies=len(BODIES),instruction_anchors=len(ANCHORS),data_spans=len(CONSTANTS))


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
