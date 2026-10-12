#!/usr/bin/env python3
"""Pin complete smart compaction and file-backed payload move requests."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'DCFlushRange': (2150197844,
                  48,
                  '93cccdef2db2d4edd330b23e345500e30ae5ead17cfaea13e3c8ac963a39ab33'),
 'OSGetCurrentThread': (2150221740,
                        12,
                        '31a585b109e5b308785a71615fcd76b34006a2cf34c9584fb803a131037a4e04'),
 'fileGarbageDataAdrClear': (2149101604,
                             1692,
                             'd1ac6fa343ee6316812f7fddff6e05b3ddc1cb80121737c7fc926d1c1c9ecb1e'),
 'fileGarbageDataAdrSet': (2149099928,
                           1676,
                           '6b0ebc30871104e5cbb4142d6d771ddc1a5bef525ebce63ab877f0a36f5767cf'),
 'fileGarbageMoveMem': (2149099728,
                        200,
                        '91b42628343fbbd3adb0c1e8d87815490624155a160d36334bf83518cfecf65e'),
 'memmove': (2150014220,
             204,
             '4ced89bd71556a086bc25674d5252c212cd728c3fce1fede9180c3449c69b966'),
 'smartAlloc': (2147677504,
                824,
                'cf70872b93213211fe6a7624da0cae2a3503cdd9d794fd6f13eee91ebb380713'),
 'smartFree': (2147678328,
               340,
               'fbec03be19c1e4417fee8b9a7baa2426da8881b5191a21f40f71e7db382ccc58'),
 'smartGarbage': (2147677048,
                  456,
                  '6d94bc372b7291c90037a32346d1746b240fbf57460b7ce7c727dc99a718caec'),
 'sysWaitDrawSync': (2147507428,
                     172,
                     'd2f83d33d85471b7ae2ff076ad74e273e4a5c2c220c21e6cb911f3c6bc096a66')}

ANCHORS = {2147677048: '9421ffe0',
 2147677052: '7c0802a6',
 2147677056: '90010024',
 2147677060: '93e1001c',
 2147677064: '93c10018',
 2147677068: '93a10014',
 2147677072: '93810010',
 2147677076: '4bfd6951',
 2147677080: '806d82c0',
 2147677084: '3ca30001',
 2147677088: '8005e008',
 2147677092: '28000000',
 2147677096: '41820010',
 2147677100: '38000000',
 2147677104: '9005e004',
 2147677108: '48000020',
 2147677112: '388d1910',
 2147677116: '386d18fc',
 2147677120: '80840010',
 2147677124: '80030010',
 2147677128: '7c640050',
 2147677132: '3803ffe0',
 2147677136: '9005e004',
 2147677140: '808d82c0',
 2147677144: '3b800000',
 2147677148: '3c640001',
 2147677152: '83c40000',
 2147677156: '83a3e008',
 2147677160: '480000c4',
 2147677164: '83fd0000',
 2147677168: '7c1ff040',
 2147677172: '4182009c',
 2147677176: '807d0008',
 2147677180: '28030000',
 2147677184: '4182007c',
 2147677188: '88030000',
 2147677192: '28000003',
 2147677196: '40820060',
 2147677200: '800300ac',
 2147677204: '28000000',
 2147677208: '41820054',
 2147677212: '4826d391',
 2147677216: '3c80803e',
 2147677220: '38045bb8',
 2147677224: '7c030040',
 2147677228: '41820040',
 2147677232: '281c0000',
 2147677236: '41820010',
 2147677240: '7c1ef850',
 2147677244: '901c0010',
 2147677248: '48000014',
 2147677252: '806d82c0',
 2147677256: '7c1ef850',
 2147677260: '3c630001',
 2147677264: '9003e004',
 2147677268: '809d0000',
 2147677272: '38000000',
 2147677276: '807d0004',
 2147677280: '901d0010',
 2147677284: '7fc41a14',
 2147677288: '4800003c',
 2147677292: '809d0008',
 2147677296: '7fc3f378',
 2147677300: '4815b45d',
 2147677304: '48000018',
 2147677308: '80bd0004',
 2147677312: '7fc3f378',
 2147677316: '7fe4fb78',
 2147677320: '4823a885',
 2147677324: '93dd0000',
 2147677328: '38000000',
 2147677332: '901d0010',
 2147677336: '807d0000',
 2147677340: '801d0004',
 2147677344: '7fc30214',
 2147677348: '7fbceb78',
 2147677352: '83bd0014',
 2147677356: '281d0000',
 2147677360: '4082ff3c',
 2147677364: '80ad82c0',
 2147677368: '3c650001',
 2147677372: '80c3e00c',
 2147677376: '28060000',
 2147677380: '41820038',
 2147677384: '388d1910',
 2147677388: '386d18fc',
 2147677392: '80840010',
 2147677396: '80030010',
 2147677400: '80650000',
 2147677404: '7c040050',
 2147677408: '80860000',
 2147677412: '7c601a14',
 2147677416: '80a60004',
 2147677420: '3803ffe0',
 2147677424: '7c040050',
 2147677428: '7c050050',
 2147677432: '90060010',
 2147677436: '388d1910',
 2147677440: '386d18fc',
 2147677444: '80ad82c0',
 2147677448: '80840010',
 2147677452: '80030010',
 2147677456: '80650000',
 2147677460: '7c840050',
 2147677464: '3884ffe0',
 2147677468: '48267539',
 2147677472: '80010024',
 2147677476: '83e1001c',
 2147677480: '83c10018',
 2147677484: '83a10014',
 2147677488: '83810010',
 2147677492: '7c0803a6',
 2147677496: '38210020',
 2147677500: '4e800020',
 2149099728: '9421fff0',
 2149099732: '7c0802a6',
 2149099736: '90010014',
 2149099740: '93e1000c',
 2149099744: '7c9f2378',
 2149099748: '93c10008',
 2149099752: '7c7e1b78',
 2149099756: '88040000',
 2149099760: '28000003',
 2149099764: '4082001c',
 2149099768: '801f00ac',
 2149099772: '28000000',
 2149099776: '40820018',
 2149099780: '7fe3fb78',
 2149099784: '4800071d',
 2149099788: '4800000c',
 2149099792: '7fe3fb78',
 2149099796: '48000711',
 2149099800: '80bf00a0',
 2149099804: '7fc3f378',
 2149099808: '80850000',
 2149099812: '80a50004',
 2149099816: '480df3e5',
 2149099820: '807f00a0',
 2149099824: '93c30000',
 2149099828: '881f0000',
 2149099832: '28000003',
 2149099836: '40820030',
 2149099840: '807f00ac',
 2149099844: '28030000',
 2149099848: '4182000c',
 2149099852: '93c3007c',
 2149099856: '48000030',
 2149099860: '807f00a0',
 2149099864: '889f0001',
 2149099868: '80630000',
 2149099872: '7c840774',
 2149099876: '48000035',
 2149099880: '48000018',
 2149099884: '807f00a0',
 2149099888: '889f0001',
 2149099892: '80630000',
 2149099896: '7c840774',
 2149099900: '4800001d',
 2149099904: '80010014',
 2149099908: '83e1000c',
 2149099912: '83c10008',
 2149099916: '7c0803a6',
 2149099920: '38210010',
 2149099924: '4e800020'}

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
    return dict(schema=1,scope='Complete smart compaction/file-move requests;backend identities linked;no relocation/allocator/platform/source/runtime closure',bodies=len(BODIES),instruction_anchors=len(ANCHORS),data_spans=len(CONSTANTS))


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
