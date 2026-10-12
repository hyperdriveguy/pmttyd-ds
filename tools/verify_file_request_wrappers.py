#!/usr/bin/env python3
"""Pin complete formatted file wrappers and file-handle reference release."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'_fileAlloc': (2149098148,
                852,
                'ad4cb41f748acc8fa0ce1cac41a6b9530a662ae6ad0643554b1436529d6ffaf4'),
 'fileAlloc': (2149099000, 36, '831c2eab2a7c05d89eaeeb61cac783a0eaf08bb15af9bc20f8c38208cc53ee07'),
 'fileAllocf': (2149099036,
                200,
                '21c3389d3c270faf5d2ddd82752fe1e0117730ddba5e2dd75c5705bcab7ff67d'),
 'fileAsync': (2149096772, 932, 'fcae8d391ae9cdfcc68e58b1b50be78d8e24b1a1119fc2d80614174d6013dc55'),
 'fileAsyncf': (2149097704,
                184,
                'a9d734557b2befccff653848a56af4b016f5c553e278bdcad661e14f4a17e150'),
 'fileFree': (2149098080, 68, '684126212d2c5ab90eac038c79edb88fa85ea3ed59cb02bcbb1297db7432b017'),
 'vsprintf': (2150015384, 120, 'c226eaf311384b14385befd8757805097577ad7403895b0ad725fa23762a43f4')}

ANCHORS = {2149097704: '9421ff70',
 2149097708: '7c0802a6',
 2149097712: '90010094',
 2149097716: 'bf61007c',
 2149097720: '7c7b1b78',
 2149097724: '7c9c2378',
 2149097728: '40860024',
 2149097732: 'd8210028',
 2149097736: 'd8410030',
 2149097740: 'd8610038',
 2149097744: 'd8810040',
 2149097748: 'd8a10048',
 2149097752: 'd8c10050',
 2149097756: 'd8e10058',
 2149097760: 'd9010060',
 2149097764: '3be10098',
 2149097768: '39810008',
 2149097772: '3fc00300',
 2149097776: '3d60803e',
 2149097780: '9081000c',
 2149097784: '3ba10068',
 2149097788: '380b6e78',
 2149097792: '7ca42b78',
 2149097796: '90610008',
 2149097800: '7c030378',
 2149097804: '90a10010',
 2149097808: '7fa5eb78',
 2149097812: '90c10014',
 2149097816: '90e10018',
 2149097820: '9101001c',
 2149097824: '91210020',
 2149097828: '91410024',
 2149097832: '93c10068',
 2149097836: '93e1006c',
 2149097840: '91810070',
 2149097844: '480e0025',
 2149097848: '3c60803e',
 2149097852: '7f64db78',
 2149097856: '38636e78',
 2149097860: '7f85e378',
 2149097864: '4bfffbbd',
 2149097868: 'bb61007c',
 2149097872: '80010094',
 2149097876: '7c0803a6',
 2149097880: '38210090',
 2149097884: '4e800020',
 2149098080: '28030000',
 2149098084: '4d820020',
 2149098088: '800300a0',
 2149098092: '28000000',
 2149098096: '4d820020',
 2149098100: '88030000',
 2149098104: '28000001',
 2149098108: '4c820020',
 2149098112: 'a8830002',
 2149098116: '3804ffff',
 2149098120: 'b0030002',
 2149098124: 'a8030002',
 2149098128: '2c000000',
 2149098132: '4c820020',
 2149098136: '38000002',
 2149098140: '98030000',
 2149098144: '4e800020',
 2149099000: '9421fff0',
 2149099004: '7c0802a6',
 2149099008: '38a00000',
 2149099012: '90010014',
 2149099016: '4bfffc9d',
 2149099020: '80010014',
 2149099024: '7c0803a6',
 2149099028: '38210010',
 2149099032: '4e800020',
 2149099036: '9421ff70',
 2149099040: '7c0802a6',
 2149099044: '90010094',
 2149099048: '93e1008c',
 2149099052: '93c10088',
 2149099056: '93a10084',
 2149099060: '93810080',
 2149099064: '7c7c1b78',
 2149099068: '40860024',
 2149099072: 'd8210028',
 2149099076: 'd8410030',
 2149099080: 'd8610038',
 2149099084: 'd8810040',
 2149099088: 'd8a10048',
 2149099092: 'd8c10050',
 2149099096: 'd8e10058',
 2149099100: 'd9010060',
 2149099104: '3be10098',
 2149099108: '39810008',
 2149099112: '3fc00200',
 2149099116: '3d60803e',
 2149099120: '9081000c',
 2149099124: '3ba10068',
 2149099128: '380b6e78',
 2149099132: '90610008',
 2149099136: '7c030378',
 2149099140: '90a10010',
 2149099144: '7fa5eb78',
 2149099148: '90c10014',
 2149099152: '90e10018',
 2149099156: '9101001c',
 2149099160: '91210020',
 2149099164: '91410024',
 2149099168: '93c10068',
 2149099172: '93e1006c',
 2149099176: '91810070',
 2149099180: '480dfaed',
 2149099184: '3c60803e',
 2149099188: '7f84e378',
 2149099192: '38636e78',
 2149099196: '38a00000',
 2149099200: '4bfffbe5',
 2149099204: '80010094',
 2149099208: '83e1008c',
 2149099212: '83c10088',
 2149099216: '83a10084',
 2149099220: '83810080',
 2149099224: '7c0803a6',
 2149099228: '38210090',
 2149099232: '4e800020'}

CONSTANTS = {2150375200: '25732f652f25732f6566666563745f25732e74706c00'}

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
    return dict(schema=1,scope='Complete fileAsyncf/fileAllocf/fileAlloc/fileFree;formatter/async/load identities linked;no resource/cache/platform/runtime closure',bodies=len(BODIES),instruction_anchors=len(ANCHORS),data_spans=len(CONSTANTS))


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
