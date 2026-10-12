#!/usr/bin/env python3
"""Pin complete smart arena/header initialization and reset publication."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'DCFlushRange': (2150197844,
                  48,
                  '93cccdef2db2d4edd330b23e345500e30ae5ead17cfaea13e3c8ac963a39ab33'),
 'OSAllocFromHeap': (2150196396,
                     252,
                     'a6afaeb690d682a3a3b22ad68be70939eae39a63fcbcc5ba2ec991be08a05848'),
 'memset': (2147504308,
            48,
            '4176c17fce5f28ccea737946be853ea45a125b52e9acab356ba54454aa352b5c'),
 'smartAlloc': (2147677504,
                824,
                'cf70872b93213211fe6a7624da0cae2a3503cdd9d794fd6f13eee91ebb380713'),
 'smartInit': (2147679200,
               484,
               '33d63786fb83aff1714848075aba7cec1bdcd944481c69b30cebfc9a6abe6de6')}

ANCHORS = {2147679200: '9421ffe0',
 2147679204: '7c0802a6',
 2147679208: '386d1924',
 2147679212: '90010024',
 2147679216: '93e1001c',
 2147679220: '3bed1910',
 2147679224: '93c10018',
 2147679228: '3bcd18fc',
 2147679232: '93a10014',
 2147679236: '93810010',
 2147679240: '809f0010',
 2147679244: '801e0010',
 2147679248: '80630010',
 2147679252: '7c840050',
 2147679256: '3ba4ffe0',
 2147679260: '7fa4eb78',
 2147679264: '4826688d',
 2147679268: '7c7c1b79',
 2147679272: '4182001c',
 2147679276: '7fa5eb78',
 2147679280: '38800000',
 2147679284: '4bfd5481',
 2147679288: '7f83e378',
 2147679292: '7fa4eb78',
 2147679296: '48266e15',
 2147679300: '806d82c0',
 2147679304: '38000000',
 2147679308: '38800000',
 2147679312: '93830000',
 2147679316: '806d82c0',
 2147679320: '3c630001',
 2147679324: '9003e008',
 2147679328: '806d82c0',
 2147679332: '3c630001',
 2147679336: '9003e00c',
 2147679340: '80bf0010',
 2147679344: '801e0010',
 2147679348: '806d82c0',
 2147679352: '7ca50050',
 2147679356: '3805ffe0',
 2147679360: '3c630001',
 2147679364: '9003e004',
 2147679368: '806d82c0',
 2147679372: '80bf0010',
 2147679376: '801e0010',
 2147679380: '80630000',
 2147679384: '7ca50050',
 2147679388: '38a5ffe0',
 2147679392: '4bfd5415',
 2147679396: '80cd82c0',
 2147679400: '3c600001',
 2147679404: '38a3e000',
 2147679408: '38800000',
 2147679412: '38660004',
 2147679416: '4bfd53fd',
 2147679420: '806d82c0',
 2147679424: '38000100',
 2147679428: '38830004',
 2147679432: '7c0903a6',
 2147679436: '3864001c',
 2147679440: '3804ffe4',
 2147679444: '90640014',
 2147679448: '38640038',
 2147679452: '90040018',
 2147679456: '3804001c',
 2147679460: '90640030',
 2147679464: '38640054',
 2147679468: '90840034',
 2147679472: '9064004c',
 2147679476: '38640070',
 2147679480: '90040050',
 2147679484: '38040038',
 2147679488: '90640068',
 2147679492: '3864008c',
 2147679496: '9004006c',
 2147679500: '38040054',
 2147679504: '90640084',
 2147679508: '386400a8',
 2147679512: '90040088',
 2147679516: '38040070',
 2147679520: '906400a0',
 2147679524: '386400c4',
 2147679528: '900400a4',
 2147679532: '3804008c',
 2147679536: '906400bc',
 2147679540: '386400e0',
 2147679544: '900400c0',
 2147679548: '380400a8',
 2147679552: '906400d8',
 2147679556: '900400dc',
 2147679560: '388400e0',
 2147679564: '4200ff80',
 2147679568: '806d82c0',
 2147679572: '38800000',
 2147679576: '38030004',
 2147679580: '3c630001',
 2147679584: '9003e010',
 2147679588: '806d82c0',
 2147679592: '3c630001',
 2147679596: '8063e010',
 2147679600: '90830018',
 2147679604: '806d82c0',
 2147679608: '3c630001',
 2147679612: '3803dfe8',
 2147679616: '9003e014',
 2147679620: '806d82c0',
 2147679624: '3c630001',
 2147679628: '8063e014',
 2147679632: '90830014',
 2147679636: '806d82c0',
 2147679640: '3c630001',
 2147679644: '9083e018',
 2147679648: '908d18f0',
 2147679652: '83e1001c',
 2147679656: '83c10018',
 2147679660: '83a10014',
 2147679664: '83810010',
 2147679668: '80010024',
 2147679672: '7c0803a6',
 2147679676: '38210020',
 2147679680: '4e800020'}

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
    return dict(schema=1,scope='Complete smart arena/header initialization;backend identities linked;no heap provisioning/platform/source/runtime closure',bodies=len(BODIES),instruction_anchors=len(ANCHORS),data_spans=len(CONSTANTS))


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
