#!/usr/bin/env python3
"""Pin complete battle init sequence registry reset and event readiness protocol."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'BattleCheckEndUnitInitEvent': (2148702612,
                                 124,
                                 'd97604ac736741579c207a0ee4bcd2d594ea5ba6bef36e41ca40843d9264957a'),
 'BattleGetSeq': (2148501920,
                  104,
                  '00dd741e83176881d659d4201d6c68033d217de3a408dce9965d23774520ab60'),
 'BattleGetUnitPtr': (2148501708,
                      60,
                      '6a40e068a63873b3f93f3cf0bdc72c59323e60ff5b961ac740b22eddb58276fa'),
 'BattleInit': (2148503300,
                312,
                '616f72f3468ba55fb0ac5dc234c5b950eb85fed4c8caafc20cca55bb73aa48c7'),
 'BattleSetSeq': (2148502024,
                  96,
                  'c37da15d68a33b6a9bc520d6c77c96a476750cfbb01c9512d645c893f498479d'),
 'BattleSetUnitPtr': (2148501684,
                      24,
                      '35788ac73436e3e46799252fe1c91a538e68b17ce4cf8f6554a1d63ff20ab409'),
 'BattleStatusWindowEventOn': (2148496976,
                               44,
                               '8f6d361123ef06e1225446824ef7e5fbd50094e449b4f1b10a4c2312ee3ba398'),
 'BattleStatusWindowSystemOn': (2148496932,
                                44,
                                'b615d36e0766e350b8e65ed67a1dda294cf22b2c6dfa22ea1e641227cb992fa7'),
 'BtlUnit_Init': (2148701844,
                  76,
                  '5c59fa8e9f9ae745600e470d15db21957747ba68e43e3bba6c817ba8e2770bf4'),
 'Btl_UnitSetup': (2148502188,
                   760,
                   '7629c39a04c07b612a1f7bf3d23e61a46bf187e422a4279a31f6d8eea9dc8844'),
 'animEffectAsync': (2147751144,
                     312,
                     '23f5e7d7e8bca1ddcf786dedb1c62df1cb010cfb4fc4a0408d0666cbfac61a03'),
 'btlseqInit': (2148658752,
                408,
                '1e2842e3f02393f304dee29a856d0be5bea2c2913c9bd9f95de6c65e6142b746'),
 'effNiceAsync': (2148814148,
                  128,
                  'a0f69a518a49c1d71013fed4ad1628f74f06244ab2ad059b50ba2ca8e748a302'),
 'evtCheckID': (2147706372,
                96,
                '3a98a24e3d46852ee1f4a80dd51ff76dbacc0541fb68c824ac6032bc20ded5c5'),
 'marioGetPartyId': (2148798544,
                     152,
                     '2339c1d1b7c127d2570ff8125d1c901db3676fca636de93d725f3f923b02923b'),
 'marioGetPtr': (2147868864,
                 8,
                 '3a59459f28ddd5d287c200434ad158a9dfe277f4f6bcaae3eb8698505aa9773d'),
 'partyGetPtr': (2148266420,
                 32,
                 'b338694df4542858c65e2ac8e13a77b66393a928b732dd679665bcfdbfac572a')}

ANCHORS = {2148501684: '2c040040',
 2148501688: '4c800020',
 2148501692: '5480103a',
 2148501696: '7c630214',
 2148501700: '90a30020',
 2148501704: '4e800020',
 2148501920: '28040007',
 2148501924: '4181005c',
 2148501928: '3ca08035',
 2148501932: '5480103a',
 2148501936: '3885c6b0',
 2148501940: '7c04002e',
 2148501944: '7c0903a6',
 2148501948: '4e800420',
 2148501952: '80630004',
 2148501956: '4e800020',
 2148501960: '80630f0c',
 2148501964: '4e800020',
 2148501968: '80630f10',
 2148501972: '4e800020',
 2148501976: '80630f14',
 2148501980: '4e800020',
 2148501984: '80630f18',
 2148501988: '4e800020',
 2148501992: '80630f1c',
 2148501996: '4e800020',
 2148502000: '80630f20',
 2148502004: '4e800020',
 2148502008: '80630f24',
 2148502012: '4e800020',
 2148502016: '38600000',
 2148502020: '4e800020',
 2148502024: '28040007',
 2148502028: '4d810020',
 2148502032: '3cc08035',
 2148502036: '5480103a',
 2148502040: '3886c6d0',
 2148502044: '7c04002e',
 2148502048: '7c0903a6',
 2148502052: '4e800420',
 2148502056: '90a30004',
 2148502060: '4e800020',
 2148502064: '90a30f0c',
 2148502068: '4e800020',
 2148502072: '90a30f10',
 2148502076: '4e800020',
 2148502080: '90a30f14',
 2148502084: '4e800020',
 2148502088: '90a30f18',
 2148502092: '4e800020',
 2148502096: '90a30f1c',
 2148502100: '4e800020',
 2148502104: '90a30f20',
 2148502108: '4e800020',
 2148502112: '90a30f24',
 2148502116: '4e800020',
 2148503568: '48030685',
 2148658752: '9421fff0',
 2148658756: '7c0802a6',
 2148658760: '3c80802f',
 2148658764: '90010014',
 2148658768: 'bfc10008',
 2148658772: '3be4f838',
 2148658776: '7c7e1b78',
 2148658780: '38800001',
 2148658784: '4bfd9b41',
 2148658788: '3c800100',
 2148658792: '38040003',
 2148658796: '7c030000',
 2148658800: '418200b8',
 2148658804: '40800020',
 2148658808: '38a40001',
 2148658812: '7c032800',
 2148658816: '41820034',
 2148658820: '4080007c',
 2148658824: '7c032000',
 2148658828: '4080001c',
 2148658832: '48000134',
 2148658836: '38a40005',
 2148658840: '7c032800',
 2148658844: '41820100',
 2148658848: '40800124',
 2148658852: '480000e8',
 2148658856: '7fc3f378',
 2148658860: '38800001',
 2148658864: '4bfd9b59',
 2148658868: '4bf3f20d',
 2148658872: '80030004',
 2148658876: '60000001',
 2148658880: '90030004',
 2148658884: '4802218d',
 2148658888: '4bfa02ed',
 2148658892: '28030000',
 2148658896: '41820010',
 2148658900: '80030004',
 2148658904: '60000001',
 2148658908: '90030004',
 2148658912: '7fc3f378',
 2148658916: '4bfd9bc9',
 2148658920: '3c800100',
 2148658924: '7fc3f378',
 2148658928: '38a40002',
 2148658932: '38800001',
 2148658936: '4bfd9b11',
 2148658940: '480000c8',
 2148658944: '7fc3f378',
 2148658948: '4800aa91',
 2148658952: '2c030000',
 2148658956: '418200b8',
 2148658960: '3c800100',
 2148658964: '7fc3f378',
 2148658968: '38a40003',
 2148658972: '38800001',
 2148658976: '4bfd9ae9',
 2148658980: '480000a0',
 2148658984: '387f03ac',
 2148658988: '38800001',
 2148658992: '4bf225b9',
 2148658996: '2c030000',
 2148659000: '4182008c',
 2148659004: '387f03b4',
 2148659008: '38800001',
 2148659012: '4bf225a5',
 2148659016: '2c030000',
 2148659020: '41820078',
 2148659024: '387f03c0',
 2148659028: '38800001',
 2148659032: '4bf22591',
 2148659036: '2c030000',
 2148659040: '41820064',
 2148659044: '38600001',
 2148659048: '48025ddd',
 2148659052: '2c030000',
 2148659056: '41820054',
 2148659060: '3c800100',
 2148659064: '7fc3f378',
 2148659068: '38a40004',
 2148659072: '38800001',
 2148659076: '4bfd9a85',
 2148659080: '4800003c',
 2148659084: '7fc3f378',
 2148659088: '38800001',
 2148659092: '4bfd9a75',
 2148659096: '4800002c',
 2148659100: '4bfd86b5',
 2148659104: '4bfd8685',
 2148659108: '7fc3f378',
 2148659112: '38800000',
 2148659116: '38a00001',
 2148659120: '4bfd9a59',
 2148659124: '7fc3f378',
 2148659128: '38800002',
 2148659132: '3ca00200',
 2148659136: '4bfd9a49',
 2148659140: 'bbc10008',
 2148659144: '80010014',
 2148659148: '7c0803a6',
 2148659152: '38210010',
 2148659156: '4e800020',
 2148701844: '9421fff0',
 2148701848: '7c0802a6',
 2148701852: '90010014',
 2148701856: 'bfc10008',
 2148701860: '3be00000',
 2148701864: '83cd1c70',
 2148701868: '7fc3f378',
 2148701872: '7fe4fb78',
 2148701876: '38a00000',
 2148701880: '4bfcf1fd',
 2148701884: '3bff0001',
 2148701888: '2c1f0040',
 2148701892: '4180ffe8',
 2148701896: 'bbc10008',
 2148701900: '38600001',
 2148701904: '80010014',
 2148701908: '7c0803a6',
 2148701912: '38210010',
 2148701916: '4e800020',
 2148702612: '9421ffe0',
 2148702616: '7c0802a6',
 2148702620: '90010024',
 2148702624: 'bf61000c',
 2148702628: '7c7b1b78',
 2148702632: '3ba00001',
 2148702636: '3b800000',
 2148702640: '3be00000',
 2148702644: '7f63db78',
 2148702648: '7f84e378',
 2148702652: '4bfcef11',
 2148702656: '7c7e1b79',
 2148702660: '41820028',
 2148702664: '807e0288',
 2148702668: '2c030000',
 2148702672: '4182001c',
 2148702676: '4bf0cc31',
 2148702680: '2c030000',
 2148702684: '4182000c',
 2148702688: '3ba00000',
 2148702692: '48000008',
 2148702696: '93fe0288',
 2148702700: '3b9c0001',
 2148702704: '2c1c0040',
 2148702708: '4180ffc0',
 2148702712: '7fa3eb78',
 2148702716: 'bb61000c',
 2148702720: '80010024',
 2148702724: '7c0803a6',
 2148702728: '38210020',
 2148702732: '4e800020'}

CONSTANTS = {2147505184: '3da0804161adcf20',
 2150562788: '705f625f737400',
 2150562796: '705f6a79616261726100',
 2150562808: '705f6b616974656e5f6800',
 2150942384: '800f89c0800f89c8800f89d0800f89d8800f89e0800f89e8800f89f0800f89f8800f8a28800f8a30800f8a38800f8a40800f8a48800f8a50800f8a58800f8a60'}

CALL_SITES = {2148503568: 2148701844, 2148658916: 2148502188}

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
                if target in (0x800f8aac,0x80129694): sites[address]=target
        if sites!=CALL_SITES: raise ValueError('Init/setup direct DOL caller census mismatch')
        for name,(address,size,digest) in BODIES.items():
            if hashlib.sha256(read(address,size)).hexdigest()!=digest:
                raise ValueError(f'Body mismatch: {name}')
        for address,value in {**ANCHORS,**CONSTANTS}.items():
            if read(address,len(value)//2).hex()!=value:
                raise ValueError(f'Instruction/constant mismatch: {address:#x}')
    return dict(schema=1,scope='Six complete battle init dispatch registry and readiness bodies;source sequence tables resource keys and two direct calls pinned;services/source/end/query/Game/runtime remain open',dol_direct_call_sites=len(sites),bodies=len(BODIES),instruction_anchors=len(ANCHORS),data_spans=len(CONSTANTS))


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
