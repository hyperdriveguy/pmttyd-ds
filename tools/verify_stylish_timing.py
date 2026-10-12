#!/usr/bin/env python3
"""Check stylish timing, event polling and icon contracts against read-only US ISO."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {
    'iconDispGx': (0x8001a918,0x24,'140c49735d09568fc933f8eca36b6fc8788f0878ac8b5ecf60f19e3079b3041d'),
    'evtmgrCmd': (0x800316b8,0x13b8,'4b6cdc0e9b940137848236a344598d586d850790e0b25beda4b9ed81da069d0e'),
    'btlevtcmd_ACRGetResult': (0x80106d7c,0x80,'f63d4a3a813d76f95c48212651fa4098a49c0141caa11dd749b7f50d0066d1de'),
    'btlevtcmd_ACRStart': (0x80106dfc,0xb8,'7eb12e360119f4f6aa1165740bc8289c8abd17206acba76ca464c72f9cb3bb76'),
    'BattlePadMultiCheckRecordTrigger': (0x8011a008,0x20,'b3ddad8d830fad268c52fc567f17ee2f4446a5040fcfebbaa4bd77e73f2ddafb'),
    'BattlePadCheckRecordTrigger': (0x8011a028,0x2c,'1f980b4fc304740b02e761f6e2cca3bc5c759a4489281086fc97f4c16ac0564c'),
    'BattlePadMultiCheckTrigger': (0x8011a054,0x18,'ed5f5cdc2977287d3b90cb1aeb5057347751bb6f6895cf8b1cf0d03c9d2f461f'),
    'BattlePadCheckTrigger': (0x8011a06c,0x28,'aac2f85572d21b63f00306eae30db3c1656273ce4c54bc39408db998b4b8575f'),
    'BtlUnit_GetHeight': (0x80128414,0x8,'b10ca69da15980aa09f724ea61a56e7649c02d07f97c2742caef9cb2fcdf7677'),
    'BattleAcrobatGetResult': (0x80192d04,0x18,'82753d04a13a380b5f4b7b2ad51ebea8974fa70fab3e74a3b6f7118a8e7f927f'),
    '_accrobat_timing_icon_disp': (0x80192d1c,0x98,'687c21515d1147853085dbd372ddfc5103fc304efb260d9e2629c8d5976b2b1e'),
    'BattleAcrobatMain': (0x80192db4,0x208,'4ad6402d8097fa3b262a1a75dbfaa85246e701cd0995cfd90a50edb168e391c0'),
    'BattleAcrobatStart': (0x80192fbc,0x28,'5dcd3155d683c2e8ea41916b5b4f26a17ff08f51eedf165e3f5281f93c8f000c'),
}
ANCHORS = {2149133792: '901d271c', 2149133808: '901d2730', 2149133836: '901d2730', 2149133852: '5400077b', 2149133872: '40810070', 2149133884: '41810064', 2149133948: 'ec01002a', 2149133956: '4be7d9e9', 2149133980: '4be7dd29', 2149133984: '881f0307', 2149134024: '3bc00001', 2149134036: '4181004c', 2149134068: '4bf87135', 2149134088: '901d2730', 2149134116: '4bf87149', 2149134148: '41810014', 2149134160: '901d2730', 2149134176: '41810014', 2149134188: '901d2730', 2149134204: '901d2730', 2149134224: '41800014', 2149134236: '901d2730', 2149134244: '38600000', 2148560412: '4182007c', 2148560532: '4808c129', 2148560540: '4808bf19', 2148560308: '41820014', 2148560324: '4bf29f39', 2148560336: '41820014', 2148560352: '4bf29f1d', 2149133576: '90040000', 2149133580: '8003271c', 2149133588: '90050000', 2149134272: '9003271c', 2149134292: '90032730', 2149134296: '91032734', 2149133632: '4bf95f3d', 2149133640: '4bf956cd', 2149133724: '4be87b7d', 2147592488: '48000015', 2148697108: 'a86300ce', 2148638748: '7c04002e', 2148638816: '7c05002e', 2147690284: '38800001', 2147690316: '4e800421', 2147690332: '38800000', 2147691052: '981f000a', 2147691072: '981f000a'}
CONSTANTS = {2151826904: '41700000', 2151826908: '3f2b851f', 2150602424: '000000000000000000000000', 2150602440: '4330000080000000', 2150650172: '800326fc'}

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
        calls = {2150982564: (393307, 2148560380, 4294967294, 4264967297, 4264967298, 4264967296, 0), 2150982592: (196699, 2148560252, 4264967302, 4264967303), 2150984164: (393307, 2148560380, 4294967294, 4264967297, 4264967298, 4264967296, 0), 2150984192: (196699, 2148560252, 4264967302, 4264967303)}
        for address,words in calls.items():
            if struct.unpack('>'+str(len(words))+'I',read(address,4*len(words)))!=words:
                raise ValueError(f'Ordinary Jump stylish caller mismatch: {address:#x}')
    return dict(schema=1,scope='Static US stylish contracts; no measured timing claim',
                bodies=len(BODIES),instruction_anchors=len(ANCHORS),constants=len(CONSTANTS),ordinary_callers=len(calls))


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
