#!/usr/bin/env python3
"""Pin complete smart header release and automatic tag-filtered release."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'_fileGarbage': (2149099236,
                  492,
                  'e381020b9ed515b3bcfd081f72d79c0c532dfebd8ee5941ec43ff80f0fa9295c'),
 'fileGarbageMoveMem': (2149099728,
                        200,
                        '91b42628343fbbd3adb0c1e8d87815490624155a160d36334bf83518cfecf65e'),
 'smartAlloc': (2147677504,
                824,
                'cf70872b93213211fe6a7624da0cae2a3503cdd9d794fd6f13eee91ebb380713'),
 'smartAutoFree': (2147678668,
                   164,
                   '82f87f4ec86561eb675ba46a385a977fd77b99869a118be65d80ee82a6c5f8d0'),
 'smartFree': (2147678328,
               340,
               'fbec03be19c1e4417fee8b9a7baa2426da8881b5191a21f40f71e7db382ccc58'),
 'smartGarbage': (2147677048,
                  456,
                  '6d94bc372b7291c90037a32346d1746b240fbf57460b7ce7c727dc99a718caec'),
 'smartInit': (2147679200,
               484,
               '33d63786fb83aff1714848075aba7cec1bdcd944481c69b30cebfc9a6abe6de6')}

ANCHORS = {2147678328: '8803000e',
 2147678332: '28000004',
 2147678336: '40820010',
 2147678340: '38000003',
 2147678344: '9803000e',
 2147678348: '4e800020',
 2147678352: '80830018',
 2147678356: '28040000',
 2147678360: '40820034',
 2147678364: '808d82c0',
 2147678368: '80030014',
 2147678372: '3c840001',
 2147678376: '9004e008',
 2147678380: '808d82c0',
 2147678384: '3c840001',
 2147678388: '8084e008',
 2147678392: '28040000',
 2147678396: '41820018',
 2147678400: '38000000',
 2147678404: '90040018',
 2147678408: '4800000c',
 2147678412: '80030014',
 2147678416: '90040014',
 2147678420: '80830014',
 2147678424: '28040000',
 2147678428: '40820034',
 2147678432: '808d82c0',
 2147678436: '80030018',
 2147678440: '3c840001',
 2147678444: '9004e00c',
 2147678448: '808d82c0',
 2147678452: '3c840001',
 2147678456: '8084e00c',
 2147678460: '28040000',
 2147678464: '41820018',
 2147678468: '38000000',
 2147678472: '90040014',
 2147678476: '4800000c',
 2147678480: '80030018',
 2147678484: '90040018',
 2147678488: '80a30018',
 2147678492: '80830004',
 2147678496: '80030010',
 2147678500: '28050000',
 2147678504: '7cc40214',
 2147678508: '41820014',
 2147678512: '80050010',
 2147678516: '7c003214',
 2147678520: '90050010',
 2147678524: '48000018',
 2147678528: '808d82c0',
 2147678532: '3c840001',
 2147678536: '8004e004',
 2147678540: '7c003214',
 2147678544: '9004e004',
 2147678548: '808d82c0',
 2147678552: '3c840001',
 2147678556: '8004e010',
 2147678560: '28000000',
 2147678564: '40820014',
 2147678568: '9064e010',
 2147678572: '38000000',
 2147678576: '90030018',
 2147678580: '4800001c',
 2147678584: '8084e014',
 2147678588: '90640014',
 2147678592: '808d82c0',
 2147678596: '3c840001',
 2147678600: '8004e014',
 2147678604: '90030018',
 2147678608: '38000000',
 2147678612: 'b003000c',
 2147678616: '90030014',
 2147678620: '90030004',
 2147678624: '90030010',
 2147678628: '90030008',
 2147678632: '808d82c0',
 2147678636: '3c840001',
 2147678640: '9064e014',
 2147678644: '806d82c0',
 2147678648: '3c830001',
 2147678652: '8064e018',
 2147678656: '38030001',
 2147678660: '9004e018',
 2147678664: '4e800020',
 2147678668: '9421ffe0',
 2147678672: '7c0802a6',
 2147678676: '90010024',
 2147678680: '93e1001c',
 2147678684: '547f043e',
 2147678688: '93c10018',
 2147678692: '93a10014',
 2147678696: '7c7d1b78',
 2147678700: '808d82c0',
 2147678704: '3c640001',
 2147678708: '8063e008',
 2147678712: '4800001c',
 2147678716: '8803000e',
 2147678720: '83c30014',
 2147678724: '7c00f800',
 2147678728: '40820008',
 2147678732: '4bfffe6d',
 2147678736: '7fc3f378',
 2147678740: '28030000',
 2147678744: '4082ffe4',
 2147678748: '2c1d0003',
 2147678752: '40820034',
 2147678756: '806d82c0',
 2147678760: '3c630001',
 2147678764: '8063e008',
 2147678768: '4800001c',
 2147678772: '8803000e',
 2147678776: '83e30014',
 2147678780: '2c000004',
 2147678784: '40820008',
 2147678788: '4bfffe35',
 2147678792: '7fe3fb78',
 2147678796: '28030000',
 2147678800: '4082ffe4',
 2147678804: '80010024',
 2147678808: '83e1001c',
 2147678812: '83c10018',
 2147678816: '83a10014',
 2147678820: '7c0803a6',
 2147678824: '38210020',
 2147678828: '4e800020'}

CONSTANTS = {2147505184: '3da0804161adcf20', 2151764448: '803ca410'}

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
    return dict(schema=1,scope='Complete smart header release/auto-release;backend identities linked;no allocation/compaction/platform/source/runtime closure',bodies=len(BODIES),instruction_anchors=len(ANCHORS),data_spans=len(CONSTANTS))


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
