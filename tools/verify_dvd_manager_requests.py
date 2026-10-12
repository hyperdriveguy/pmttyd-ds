#!/usr/bin/env python3
"""Pin complete DVD manager slot opening,synchronous read and global callback setup."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'DVDConvertPathToEntrynum': (2150235580,
                              756,
                              '0b71fb2e5d4ef127af42d242578ee784e9ec4baf19f9f867eb1a2e97fa31258b'),
 'DVDMgrMain': (2148814884,
                612,
                '0c36f07bb2d0e9bba569a99e997e5fa02ae0e7e55d9f07e17fca2169984b30e8'),
 'DVDMgrOpen': (2148814472,
                412,
                '81238bae8554f5f1f28b208d0ceded5a06fc5c3edbfa6af16504e91b0066d62f'),
 'DVDMgrRead': (2148814360,
                112,
                '7cef027054d65e0bbe4f9ec8ba687a90d8f5b1dab485555ef085d623c5a80bda'),
 'DVDMgrSetupCallback': (2148814276,
                         8,
                         '23066b5b1759c11085e5b7773c68e2ba7064c136aa5d19e6c3d637d4b7ad6317'),
 'DVDOpen': (2150236336,
             200,
             '8f9b4d680ae191ba8991ba1db413b2c6c827bc9559763c392f01572dfb6e8b18'),
 'OSYieldThread': (2150223092,
                   60,
                   'e143f15719272fc1847dd173f2bdc753a7ac8fc90a642bb3c12120084bdbb9c5'),
 'memset': (2147504308,
            48,
            '4176c17fce5f28ccea737946be853ea45a125b52e9acab356ba54454aa352b5c'),
 'strcpy': (2150028796,
            184,
            'd68e2a1981b1703890d7213032425b317c6d466dc5c88d957e473f2cc126426d')}

ANCHORS = {2148814276: '906d1cd4',
 2148814280: '4e800020',
 2148814360: '9421fff0',
 2148814364: '7c0802a6',
 2148814368: '90010014',
 2148814372: '38000000',
 2148814376: '93e1000c',
 2148814380: '7c7f1b78',
 2148814384: '9083007c',
 2148814388: '90a30080',
 2148814392: '90c30084',
 2148814396: 'a0630090',
 2148814400: '60630001',
 2148814404: 'b07f0090',
 2148814408: 'a07f0090',
 2148814412: '546307fa',
 2148814416: 'b07f0090',
 2148814420: '901f008c',
 2148814424: '901f0088',
 2148814428: 'a01f0090',
 2148814432: '540007bd',
 2148814436: '4082000c',
 2148814440: '48157e8d',
 2148814444: '4bfffff0',
 2148814448: '80010014',
 2148814452: '807f0074',
 2148814456: '83e1000c',
 2148814460: '7c0803a6',
 2148814464: '38210010',
 2148814468: '4e800020',
 2148814472: '9421ffe0',
 2148814476: '7c0802a6',
 2148814480: '90010024',
 2148814484: '93e1001c',
 2148814488: '93c10018',
 2148814492: '7cbe2b78',
 2148814496: '93a10014',
 2148814500: '7c9d2378',
 2148814504: '93810010',
 2148814508: '7c7c1b78',
 2148814512: '4815af0d',
 2148814516: '2c03ffff',
 2148814520: '4082000c',
 2148814524: '38600000',
 2148814528: '48000144',
 2148814532: '38000020',
 2148814536: '83ed1cd8',
 2148814540: '38600000',
 2148814544: '7c0903a6',
 2148814548: 'a01f0090',
 2148814552: '54000421',
 2148814556: '4182009c',
 2148814560: 'a01f0128',
 2148814564: '38630001',
 2148814568: '3bff0098',
 2148814572: '54000421',
 2148814576: '41820088',
 2148814580: 'a01f0128',
 2148814584: '38630001',
 2148814588: '3bff0098',
 2148814592: '54000421',
 2148814596: '41820074',
 2148814600: 'a01f0128',
 2148814604: '38630001',
 2148814608: '3bff0098',
 2148814612: '54000421',
 2148814616: '41820060',
 2148814620: 'a01f0128',
 2148814624: '38630001',
 2148814628: '3bff0098',
 2148814632: '54000421',
 2148814636: '4182004c',
 2148814640: 'a01f0128',
 2148814644: '38630001',
 2148814648: '3bff0098',
 2148814652: '54000421',
 2148814656: '41820038',
 2148814660: 'a01f0128',
 2148814664: '38630001',
 2148814668: '3bff0098',
 2148814672: '54000421',
 2148814676: '41820024',
 2148814680: 'a01f0128',
 2148814684: '38630001',
 2148814688: '3bff0098',
 2148814692: '54000421',
 2148814696: '41820010',
 2148814700: '38630001',
 2148814704: '3bff0098',
 2148814708: '4200ff60',
 2148814712: '2c030100',
 2148814716: '4180000c',
 2148814720: '38600000',
 2148814724: '48000080',
 2148814728: '7fe3fb78',
 2148814732: '38800000',
 2148814736: '38a00098',
 2148814740: '4bec0121',
 2148814744: '7fe3fb78',
 2148814748: '7f84e378',
 2148814752: '4812865d',
 2148814756: '38a00000',
 2148814760: '57a0063e',
 2148814764: 'b0bf0090',
 2148814768: '7fe3fb78',
 2148814772: '389f0040',
 2148814776: 'b01f0092',
 2148814780: 'b3df0094',
 2148814784: '90bf007c',
 2148814788: '90bf0080',
 2148814792: '90bf0084',
 2148814796: '90bf0088',
 2148814800: 'a01f0090',
 2148814804: '60008000',
 2148814808: 'b01f0090',
 2148814812: '4815b0d5',
 2148814816: '2c030000',
 2148814820: '4082001c',
 2148814824: '7fe3fb78',
 2148814828: '38800000',
 2148814832: '38a00098',
 2148814836: '4bec00c1',
 2148814840: '38600000',
 2148814844: '48000008',
 2148814848: '7fe3fb78',
 2148814852: '80010024',
 2148814856: '83e1001c',
 2148814860: '83c10018',
 2148814864: '83a10014',
 2148814868: '83810010',
 2148814872: '7c0803a6',
 2148814876: '38210020',
 2148814880: '4e800020'}

CONSTANTS = {2147505184: '3da0804161adcf20'}

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
    return dict(schema=1,scope='Complete DVD manager Open/Read/global callback setup;backend identities linked;no worker/platform/source/runtime closure',bodies=len(BODIES),instruction_anchors=len(ANCHORS),data_spans=len(CONSTANTS))


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
