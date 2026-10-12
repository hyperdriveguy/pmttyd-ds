#!/usr/bin/env python3
"""Pin GP10 disc-thread gate, called card/audio/font routing and startup bindings in read-only US ISO."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'marioStMain': (2147940612,
                 568,
                 '74db581fe5f8665204b63c9706e483031f73edcc99e1596ce50b2a53db672f54'),
 'viPostCallback': (2147939332,
                    952,
                    '044042552dcddf496a3dc8101c419b78fa63f1f7f0950870cec0954964b6ddff'),
 'marioStInit': (2147941180,
                 840,
                 '8018bb2efc969cac5acb75f1b3c5c027f40ba108ddde3050a5b8465264237f08'),
 'gcDvdCheckThread': (2147938784,
                      548,
                      '114599cb92a451286e87f602e6706a2552f4833816ae1e8e321571c477612815'),
 'cardIsExec': (2148215692,
                80,
                'e98c521659cd091008fd43db2c751ab1c68fad341e030507927dfdd33f47c83a'),
 'SoundOpenCover': (2148406388,
                    32,
                    '5f7ac263cc5da714a70a5f3f2bccc92dc142c85e87ed745e2527dab90f42e138'),
 'SoundCloseCover': (2148406100,
                     288,
                     '13b7a2014d3ec576ecfd679836ed3cd7a164ffab1ebb51ec5d7ebc0cfccc90e3'),
 'sndIsInstalled': (2150161412,
                    8,
                    'a28687c9bb0ce05d12887d92c1b4f8f24ba174d748556df40b532fc5ba20f1e5'),
 'romFontGetMessage': (2149914712,
                       84,
                       '50e04595b27a7d685928139a6332c1d647287b3298e47561f11f753f9c465d20'),
 'romFontInit': (2149914616,
                 96,
                 '5601fad6f0761a517a5851c5bbb3f17536f97f8e043dfd1517e992e7eadb4d8e'),
 'VIGetRetraceCount': (2150256192,
                       8,
                       '875e043b5a9023dbd66686bcd2ebd83d5d64ab4808f48ced24f8316880affc19')}

ANCHORS = {2147938784: '9421ff80',
 2147938792: '90010084',
 2147938800: '3be00000',
 2147938804: '3bc00000',
 2147938808: '3ba00000',
 2147938812: '3b800000',
 2147938816: '4823355d',
 2147938824: '2800000c',
 2147938828: '41810080',
 2147938844: '7c03002e',
 2147938860: '38000001',
 2147938864: '3b80000a',
 2147938868: '90030010',
 2147938880: '38000001',
 2147938884: '3b80000b',
 2147938888: '90030010',
 2147938900: '38000001',
 2147938904: '3b80000b',
 2147938908: '90030010',
 2147938920: '38000001',
 2147938924: '3b80000c',
 2147938928: '90030010',
 2147938940: '38000001',
 2147938944: '3b80000d',
 2147938948: '90030010',
 2147938960: '38000000',
 2147938964: '90030010',
 2147938968: '482334c5',
 2147938972: '7c1f1800',
 2147938976: '41820014',
 2147938980: '482334b9',
 2147938984: '3bc00000',
 2147938996: '48235b8d',
 2147939000: '7c1d1840',
 2147939004: '4182001c',
 2147939008: '48235b81',
 2147939020: '2c1e0064',
 2147939024: '40810008',
 2147939028: '3bc00064',
 2147939040: '2c000000',
 2147939044: '4182000c',
 2147939048: '4807218d',
 2147939056: '48072065',
 2147939068: '2c000000',
 2147939072: '4182001c',
 2147939076: '48043889',
 2147939080: '2c030000',
 2147939084: '41820010',
 2147939092: '38000000',
 2147939096: '90030010',
 2147939108: '2c000000',
 2147939112: '418200d4',
 2147939116: '2c1e000a',
 2147939120: '408100cc',
 2147939124: '4bfbf8e9',
 2147939128: '2c1c0000',
 2147939132: '418200b4',
 2147939140: '481e2515',
 2147939148: '481e1aa1',
 2147939160: '90610054',
 2147939176: '90010050',
 2147939180: 'c0228f04',
 2147939188: 'c0428f08',
 2147939192: 'ec001828',
 2147939196: 'c0628f0c',
 2147939200: 'c0828f10',
 2147939216: '4822f459',
 2147939224: '38800000',
 2147939228: '4824ade9',
 2147939248: '90610064',
 2147939264: '90010060',
 2147939280: 'c0028f14',
 2147939284: 'ec211028',
 2147939288: '90c10008',
 2147939292: 'c0428f18',
 2147939296: 'c0628f0c',
 2147939300: 'ec210032',
 2147939308: '481e1b99',
 2147939312: '4bfbf749',
 2147939316: '4bf96c05',
 2147939320: '4bfff9c5',
 2147939324: '4822d8f9',
 2148215696: '28030000',
 2148215700: '4082000c',
 2148215704: '38600000',
 2148215716: '540007bd',
 2148215724: '38600001',
 2148215736: '540007ff',
 2148215744: '38600001',
 2148406400: '28000000',
 2148406404: '4c820020',
 2148406408: '38000001',
 2148406412: 'b0030214',
 2148406100: '9421fff0',
 2148406108: '90010014',
 2148406112: '481ac8a5',
 2148406116: '28030000',
 2148406120: '418200fc',
 2148406136: '28000001',
 2148406140: '408200e8',
 2148406148: '38c00000',
 2148406156: '54a00739',
 2148406164: '54a0056b',
 2148406172: '54a007bd',
 2148406180: '54a004e7',
 2148406188: '38c00001',
 2148406196: '28000002',
 2148406200: '40820010',
 2148406204: '54a004a5',
 2148406212: '38c00001',
 2148406220: '54a00739',
 2148406228: '54a0056b',
 2148406236: '54a007bd',
 2148406244: '54a004e7',
 2148406252: '38c00001',
 2148406260: '28000002',
 2148406264: '40820010',
 2148406268: '54a004a5',
 2148406276: '38c00001',
 2148406284: '54a00739',
 2148406292: '54a0056b',
 2148406300: '54a007bd',
 2148406308: '54a004e7',
 2148406316: '38c00001',
 2148406324: '28000002',
 2148406328: '40820010',
 2148406332: '54a004a5',
 2148406340: '38c00001',
 2148406344: '2c060000',
 2148406348: '41820010',
 2148406352: '38000002',
 2148406356: 'b0030214',
 2148406364: '38000000',
 2148406368: 'b0030214',
 2149914720: '2c000000',
 2149914724: '41820028',
 2149914736: '28000000',
 2149914740: '40820018',
 2149914756: '7c63002e',
 2149914784: '7c64182e',
 2149914788: '7c63002e',
 2149914616: '9421fff0',
 2149914624: '90010014',
 2149914628: '38000000',
 2149914636: '90030000',
 2149914644: '90030004',
 2149914648: '480472f1',
 2149914652: '5460043e',
 2149914656: '28000001',
 2149914660: '40820014',
 2149914668: '38000000',
 2149914672: '90030008',
 2149914684: '38000001',
 2149914688: '90030008',
 2149914692: '4bfffc65',
 2147941724: '3cc0803e',
 2147941728: '3c60803e',
 2147941732: '3c808007',
 2147941736: '38a00000',
 2147941740: '38c69900',
 2147941744: '3863a900',
 2147941748: '3884f1e0',
 2147941752: '38e01000',
 2147941756: '38c61000',
 2147941760: '39000010',
 2147941764: '39200001',
 2147941768: '4822cfa9',
 2147941772: '38000001',
 2147941776: '3c60803e',
 2147941780: '900d1a18',
 2147941784: '3863a900',
 2147941788: '4822d41d',
 2147940624: '48000008',
 2147940628: '4822d3e1',
 2147940632: '806d90b0',
 2147940636: '80030010',
 2147940640: '2c000000',
 2147940644: '4082fff0',
 2147939532: '806d90b0',
 2147939536: '80030010',
 2147939540: '2c000000',
 2147939544: '40820124'}

CONSTANTS = {2150679328: '8006f2788006f28c8006f2988006f28c8006f28c8006f23c8006f2288006f2508006f28c8006f28c8006f28c8006f28c8006f264',
 2150378112: '4330000080000000',
 2147505176: '3c40804260427440',
 2147505184: '3da0804161adcf20',
 2151809856: 'ffffffff',
 2151809860: '41c80000',
 2151809864: '3fa22222',
 2151809868: '3f800000',
 2151809872: '461c4000',
 2151809876: '3f000000',
 2151809880: '42700000'}

MESSAGE_RESOURCES = {'US-default system message10': (2150635936,
                                 87,
                                 'c0cbf73c27ced1347f17833b0fc947342e0fd29d2ef77debb8f352eb147f6b7d'),
 'US-default system message11': (2150636024,
                                 67,
                                 'c0306d3df8e3fdebf9c846e338d8f0efe2a20bd1e8c8b8ad44e2fbf7168e1ab3'),
 'US-default system message12': (2150636092,
                                 109,
                                 '74f65a824ac2ef30c25a8a50d5ac8cd758bb6452c7bf85fd5ca0327d16a895b4'),
 'US-default system message13': (2150636204,
                                 123,
                                 '67296b685da4f3c790ec7ea1e889195b2f7a19f3564cd6d8aeb74d5e71521e8a')}

ANCHORS.update({0x8006fd10:'38800001',0x8006fd18:'9083016c'})
CONSTANTS.update({0x8041df2c:'803b9340803b9394803b93e8803b943c803b9490803b94e4',
                  0x803b93bc:'803019a0803019f880301a3c80301aac'})

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
        for name,(address,size,digest) in MESSAGE_RESOURCES.items():
            if hashlib.sha256(read(address,size)).hexdigest()!=digest:
                raise ValueError(f'System message resource mismatch: {name}')
        for address,value in {**ANCHORS,**CONSTANTS}.items():
            if read(address,len(value)//2).hex()!=value:
                raise ValueError(f'Instruction/constant mismatch: {address:#x}')
    return dict(schema=1,scope='Complete disc thread/card/audio/font-routing leaves; linked consumer and scoped startup; no hardware/thread/runtime equivalence claim',
                bodies=len(BODIES),instruction_anchors=len(ANCHORS),data_spans=len(CONSTANTS),
                message_resources=len(MESSAGE_RESOURCES))


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
