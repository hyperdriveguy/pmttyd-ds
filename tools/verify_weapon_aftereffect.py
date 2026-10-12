#!/usr/bin/env python3
"""Check weapon aftereffect and stage request contracts against read-only US ISO."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'btlevtcmd_WeaponAftereffect': (2148553960,
                                 524,
                                 'a4e23fc36b5cd6826449ca322c4d9af872f9f8f3df5d2eda5d4ff4e08858e5fa'),
 'BattleStage_DestroyA1': (2148833008,
                           164,
                           'be00cbaf8354d3190989d5901cf193487239f4eea4af6d88183309d39fecfaab'),
 'BattleStage_DestroyA2': (2148832900,
                           108,
                           '7ce829dc6341b56644d40dfe93a373030ac5dab91243d2f7e0747644d0b20436'),
 'BattleStage_DestroyB': (2148832736,
                          164,
                          '14feb4eff587eac9d4a07e3435a8a72b370f557be2b2414fe087bef4662d087e'),
 'BattleStage_NozzleDirChangeCheck': (2148830192,
                                      100,
                                      '8ab99959670044d255bde910739ff17138edc2fff9176a56fe6684164b32f04c'),
 '_Nozzle_Change_Check': (2148829936,
                          256,
                          '5010b23adc7218e9e6f53cec3923ae4c38b64171dbb868e4d1087cec3a3df684'),
 'BattleStage_NozzleWorkCheck': (2148828180,
                                 184,
                                 '5f83144e7932115302d3ef47be0514469917bd1af4d7fc65ef848234353e12f2'),
 '_nozzle_type_init': (2148828020,
                       160,
                       '5321d7d8fd6cdd9b66971e1916afc31fec6eadb035ffbe291c840c625f529dfe'),
 'BattleStage_IronFrameFallCheck': (2148829120,
                                    100,
                                    '1f3ac6d3d91c19e8495e01207979a0d542a38091f5419858759c552228ed2cb6'),
 'BattleStage_ObjectFallCheck': (2148824644,
                                 96,
                                 'ea42c25520ba1630ea3c9883e4659f2ae722838e769d194d142bd317de2fec46'),
 'irand': (2147507616, 68, '0523fae52d8e314f0e46dafe5c3f04f5c4c90ced7628011fdb0cf4efa4ba0daf'),
 'rand': (2150023972, 32, '6b19fd52fb70db058f43afa9f5b9f1922cdbd69d70b461055e6ba104576326b5'),
 'pouchGetPtr': (2148359856, 8, 'dbcd2f415efe4aa4e5684ee546cf73d97cc641b5a0290f1b46b14ab606786537')}
ANCHORS = {2148554036: '4bf0086d',
 2148554048: '548006f7',
 2148554060: '548006b5',
 2148554076: '7c601851',
 2148554100: '7c601851',
 2148554120: '7c601851',
 2148554148: '3b800000',
 2148554160: '3ba00000',
 2148554168: '41820048',
 2148554184: '4bf007d9',
 2148554196: '41800014',
 2148554212: '4182001c',
 2148554236: '3b800001',
 2148554248: '480440e9',
 2148554260: '48044071',
 2148554272: '48043fc1',
 2148554280: '4bf00779',
 2148554312: '480435a9',
 2148554320: '4bf00751',
 2148554356: '48042da1',
 2148554364: '4bf00725',
 2148554396: '48043125',
 2148554412: '41820030',
 2148554428: '48041f89',
 2148554440: '4bf006d9',
 2148554456: '48041f6d',
 2148833052: '4beecf49',
 2148833064: '90038100',
 2148833100: '4beecf19',
 2148833136: '41820010',
 2148833148: '900380fc',
 2148832960: '90038104',
 2148832984: '4beeddd5',
 2148832780: '4beed059',
 2148832792: '90038108',
 2148832876: '900380fc',
 2148830252: '4bfffec5',
 2148830008: '4bebd269',
 2148830032: '41800018',
 2148830068: '41820068',
 2148830108: '901d01cc',
 2148830132: '9b6401c7',
 2148830156: '901d01cc',
 2148830168: '90030160',
 2148828220: '4081007c',
 2148828224: '4bffff35',
 2148828244: '41820064',
 2148828276: '901f82b4',
 2148828340: '901f82b4',
 2148828108: '4bebd9d5',
 2148828136: '41800014',
 2148828156: '98bf82a6',
 2148829172: '981f80f8',
 2148829196: '901f82c4',
 2148824676: '4182000c',
 2148824712: '901f82c8'}
CONSTANTS = {2150971740: '000000640004020101',
 2150973084: '000303640306020303',
 2151775560: '00020032fe363c89000000000001005e80376ab00000000200000001',
 2151775588: '00020032fe363c89000000010001005e80376ab00000000200000001',
 2151775616: '00020032fe363c89000000020001005e80376ab00000000200000001',
 2151775704: '00020032fe363c8a000000000001005e80376ea00000000200000001',
 2151775732: '00020032fe363c8a000000010001005e80376ea00000000200000001'}

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
        calls = {2150983272: (131163, 2148553960, 4264967308), 2150983916: (131163, 2148553960, 4264967308)}
        for address,words in calls.items():
            if struct.unpack('>'+str(len(words))+'I',read(address,4*len(words)))!=words:
                raise ValueError(f'Ordinary Jump aftereffect caller mismatch: {address:#x}')
    return dict(schema=1,scope='Static US stage aftereffect requests; no execution claim',
                bodies=len(BODIES),instruction_anchors=len(ANCHORS),data_spans=len(CONSTANTS),ordinary_callers=len(calls))


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
