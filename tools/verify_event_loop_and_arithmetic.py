#!/usr/bin/env python3
"""Check GOTO loop, assignment arithmetic and data cursor contracts against read-only US ISO."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'evtmgrCmd': (2147686072,
               5048,
               '4b6cdc0e9b940137848236a344598d586d850790e0b25beda4b9ed81da069d0e'),
 'evt_while': (2147703124, 212, '0813952cd86c66b111b7f66579d0420da19095d79e9ba37b37f68ccbfe0e8b18'),
 'evtGetValue': (2147684960,
                 1112,
                 'ad7dce301c927dbc422a143f3ffb023aa32af8d4f3bb81c752fddc1b9e21bd7a'),
 'evtSetValue': (2147683580,
                 1024,
                 '9c52991a1d1db6731e485516b6ed17a63ea2c48930759cd8a1ccbe3d1c1d3c46'),
 'evtGetFloat': (2147682368,
                 1212,
                 'f714b2a7ddc29e31034b9c52f37dfcf8cb0f8b21cf73c73a81da2ccc67e8e3d8'),
 'evtSetFloat': (2147681468,
                 900,
                 '07b1e305d3c2e3c1d3432397aa243d56c39c07fcafdb124586f1204b04f82921')}

ANCHORS = {2147686252: '4bfffaf5',
 2147686260: '80bf0018',
 2147686268: '7c030000',
 2147686296: '7c1f00ae',
 2147686320: '7cbf002e',
 2147686340: '907f0014',
 2147686376: '981f000e',
 2147686384: '7c000774',
 2147686396: '90a300e8',
 2147686400: '90830108',
 2147686472: '41820010',
 2147686492: '4080ffc4',
 2147686508: '909f0014',
 2147686524: '981f000e',
 2147686564: '41820010',
 2147686584: '4080000c',
 2147686620: '907f0014',
 2147703172: '83a50108',
 2147703180: '40820014',
 2147703192: '901c0014',
 2147703212: '41800010',
 2147703220: '93a50108',
 2147703232: '4bffb8a1',
 2147703252: '4bffb329',
 2147703264: '41820018',
 2147703280: '901c0014',
 2147703300: '981c000e',
 2147687836: '4bfff4c5',
 2147687852: '4bffef51',
 2147687880: '4bffef35',
 2147687908: '4bffea5d',
 2147687920: '4bffe6cd',
 2147687948: '4bfff455',
 2147687964: '4bfff445',
 2147687968: '7cbe1a14',
 2147688008: '4bfff419',
 2147688024: '4bfff409',
 2147688028: '7cbe1850',
 2147688068: '4bfff3dd',
 2147688084: '4bfff3cd',
 2147688088: '7cbe19d6',
 2147688128: '4bfff3a1',
 2147688144: '4bfff391',
 2147688148: '7ca3f3d6',
 2147688188: '4bfff365',
 2147688232: 'ec211028',
 2147688236: 'ec00082a',
 2147688240: 'fc00001e',
 2147688252: '4bfff325',
 2147688296: 'ec211028',
 2147688300: 'ec00082a',
 2147688304: 'fc00001e',
 2147688316: '7c05ebd6',
 2147688320: '7c00e9d6',
 2147688324: '7ca02850',
 2147688328: '4bffed75',
 2147688356: '4bffe89d',
 2147688372: '4bffe88d',
 2147688376: 'ec3f082a',
 2147688436: 'ec21f828',
 2147688496: 'ec3f0072',
 2147688556: 'ec21f824',
 2147688596: '907f0150',
 2147688628: '901f0150',
 2147688632: '80a50000',
 2147688636: '4bffec41',
 2147688668: '901f0150',
 2147688672: '80a50000',
 2147688676: '4bffec19',
 2147688680: '80bf0150',
 2147688696: '901f0150',
 2147688700: '80a50000',
 2147688704: '4bffebfd'}

CONSTANTS = {2151807528: '3f000000',
 2150367152: '4330000080000000',
 2151807512: '44800000',
 2151807516: '3a800000',
 2150649824: '80031760',
 2150649828: '800317d0',
 2150649832: '80031808',
 2150649836: '80031818',
 2150649840: '80031884',
 2150650008: '80031d8c',
 2150650012: '80031db8',
 2150650016: '80031dd4',
 2150650020: '80031dfc',
 2150650024: '80031e38',
 2150650028: '80031e74',
 2150650032: '80031eb0',
 2150650036: '80031eec',
 2150650040: '80031f94',
 2150650044: '80031fd0',
 2150650048: '8003200c',
 2150650052: '80032048',
 2150650056: '80032084',
 2150650060: '800320a0',
 2150650064: '800320c8'}

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
    return dict(schema=1,scope='Scoped complete4..8/50..64 and NEXT body; linked operand APIs; no execution claim',
                bodies=len(BODIES),instruction_anchors=len(ANCHORS),data_spans=len(CONSTANTS))


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
