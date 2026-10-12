#!/usr/bin/env python3
"""Pin complete file-cache collection/init and narrow mode/thread leaves."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'OSGetCurrentThread': (2150221740,
                        12,
                        '31a585b109e5b308785a71615fcd76b34006a2cf34c9584fb803a131037a4e04'),
 'OSYieldThread': (2150223092,
                   60,
                   'e143f15719272fc1847dd173f2bdc753a7ac8fc90a642bb3c12120084bdbb9c5'),
 '__memAlloc': (2147680496,
                100,
                '51d2010e5ae0c3265971a8dfaee60b24489d8bf8eb2ca386f77c1d0ceb98712b'),
 '_fileAlloc': (2149098148,
                852,
                'ad4cb41f748acc8fa0ce1cac41a6b9530a662ae6ad0643554b1436529d6ffaf4'),
 '_fileGarbage': (2149099236,
                  492,
                  'e381020b9ed515b3bcfd081f72d79c0c532dfebd8ee5941ec43ff80f0fa9295c'),
 'fileAsync': (2149096772,
               932,
               'fcae8d391ae9cdfcc68e58b1b50be78d8e24b1a1119fc2d80614174d6013dc55'),
 'fileInit': (2149103296,
              240,
              '794bf51f56d99f91260c65639562b987585f4f0d95b346dc665b7cb9b5b7f79a'),
 'fileSetCurrentArchiveType': (2149096760,
                               12,
                               '94274e264acd8407e0ac0dbaa3c52d8307def35bed714c0d64b18d6caba1e0c8'),
 'memset': (2147504308,
            48,
            '4176c17fce5f28ccea737946be853ea45a125b52e9acab356ba54454aa352b5c'),
 'smartAlloc': (2147677504,
                824,
                'cf70872b93213211fe6a7624da0cae2a3503cdd9d794fd6f13eee91ebb380713'),
 'smartFree': (2147678328,
               340,
               'fbec03be19c1e4417fee8b9a7baa2426da8881b5191a21f40f71e7db382ccc58')}

ANCHORS = {2149096760: '808db578',
 2149096764: '90640004',
 2149096768: '4e800020',
 2149099236: '9421ffe0',
 2149099240: '7c0802a6',
 2149099244: '2c030000',
 2149099248: '90010024',
 2149099252: '93e1001c',
 2149099256: '93c10018',
 2149099260: '93a10014',
 2149099264: '93810010',
 2149099268: '40820078',
 2149099272: '806db578',
 2149099276: '83e30008',
 2149099280: '48000060',
 2149099284: '881f0000',
 2149099288: '28000003',
 2149099292: '40820050',
 2149099296: '801f00ac',
 2149099300: '28000000',
 2149099304: '4182003c',
 2149099308: '48112081',
 2149099312: '3c80803e',
 2149099316: '38045bb8',
 2149099320: '7c030040',
 2149099324: '4082001c',
 2149099328: '807f00ac',
 2149099332: '38000008',
 2149099336: 'b0030090',
 2149099340: '48000018',
 2149099344: '48000008',
 2149099348: '481125a1',
 2149099352: '801f00ac',
 2149099356: '28000000',
 2149099360: '4082fff4',
 2149099364: '38000002',
 2149099368: '981f0000',
 2149099372: '83ff00a4',
 2149099376: '281f0000',
 2149099380: '4082ffa0',
 2149099384: '48000088',
 2149099388: '2c030001',
 2149099392: '40820080',
 2149099396: '806db578',
 2149099400: '83e30008',
 2149099404: '4800006c',
 2149099408: '881f0000',
 2149099412: '28000003',
 2149099416: '4082005c',
 2149099420: '801f00ac',
 2149099424: '28000000',
 2149099428: '41820030',
 2149099432: '48112005',
 2149099436: '3c80803e',
 2149099440: '38045bb8',
 2149099444: '7c030040',
 2149099448: '4182003c',
 2149099452: '48000008',
 2149099456: '48112535',
 2149099460: '801f00ac',
 2149099464: '28000000',
 2149099468: '4082fff4',
 2149099472: '48000024',
 2149099476: 'a87f0002',
 2149099480: '3803ffff',
 2149099484: 'b01f0002',
 2149099488: 'a81f0002',
 2149099492: '2c00fffe',
 2149099496: '4181000c',
 2149099500: '38000002',
 2149099504: '981f0000',
 2149099508: '83ff00a4',
 2149099512: '281f0000',
 2149099516: '4082ff94',
 2149099520: '806db578',
 2149099524: '3bc00000',
 2149099528: '3ba00000',
 2149099532: '83e30008',
 2149099536: '48000088',
 2149099540: '881f0000',
 2149099544: '839f00a4',
 2149099548: '28000002',
 2149099552: '40820064',
 2149099556: '38600000',
 2149099560: '381f0004',
 2149099564: '987f0000',
 2149099568: '807f00a0',
 2149099572: '7c001840',
 2149099576: '41820008',
 2149099580: '4bea503d',
 2149099584: '281e0000',
 2149099588: '4182000c',
 2149099592: '801f00a4',
 2149099596: '901e00a4',
 2149099600: '806db578',
 2149099604: '80030010',
 2149099608: '28000000',
 2149099612: '4082000c',
 2149099616: '93e30010',
 2149099620: '4800000c',
 2149099624: '80630014',
 2149099628: '93e300a4',
 2149099632: '38000000',
 2149099636: '901f00a4',
 2149099640: '806db578',
 2149099644: '93e30014',
 2149099648: '48000014',
 2149099652: '281e0000',
 2149099656: '40820008',
 2149099660: '7ffdfb78',
 2149099664: '7ffefb78',
 2149099668: '7f9fe378',
 2149099672: '281f0000',
 2149099676: '4082ff78',
 2149099680: '806db578',
 2149099684: '93a30008',
 2149099688: '806db578',
 2149099692: '93c3000c',
 2149099696: '83e1001c',
 2149099700: '83c10018',
 2149099704: '83a10014',
 2149099708: '83810010',
 2149099712: '80010024',
 2149099716: '7c0803a6',
 2149099720: '38210020',
 2149099724: '4e800020',
 2149103296: '9421fff0',
 2149103300: '7c0802a6',
 2149103304: '3c800003',
 2149103308: '38600000',
 2149103312: '90010014',
 2149103316: '3884c000',
 2149103320: '4bea4a19',
 2149103324: '80cdb578',
 2149103328: '3ca00003',
 2149103332: '38000000',
 2149103336: '38800000',
 2149103340: '90660000',
 2149103344: '38a5c000',
 2149103348: '806db578',
 2149103352: '90030004',
 2149103356: '806db578',
 2149103360: '90030008',
 2149103364: '806db578',
 2149103368: '9003000c',
 2149103372: '806db578',
 2149103376: '80630000',
 2149103380: '4be799a1',
 2149103384: '806db578',
 2149103388: '38000080',
 2149103392: '80630000',
 2149103396: '7c0903a6',
 2149103400: '380300b0',
 2149103404: '900300a4',
 2149103408: '38030160',
 2149103412: '90030154',
 2149103416: '38030210',
 2149103420: '90030204',
 2149103424: '380302c0',
 2149103428: '900302b4',
 2149103432: '38030370',
 2149103436: '90030364',
 2149103440: '38030420',
 2149103444: '90030414',
 2149103448: '380304d0',
 2149103452: '900304c4',
 2149103456: '38030580',
 2149103460: '90030574',
 2149103464: '38630580',
 2149103468: '4200ffbc',
 2149103472: '808db578',
 2149103476: '38000000',
 2149103480: '80640000',
 2149103484: '90640010',
 2149103488: '808db578',
 2149103492: '80640000',
 2149103496: '3c630003',
 2149103500: '3863bf50',
 2149103504: '90640014',
 2149103508: '806db578',
 2149103512: '80630014',
 2149103516: '900300a4',
 2149103520: '80010014',
 2149103524: '7c0803a6',
 2149103528: '38210010',
 2149103532: '4e800020',
 2150221740: '3c608000',
 2150221744: '806300e4',
 2150221748: '4e800020'}

CONSTANTS = {2147505184: '3da0804161adcf20', 2151777432: '8041ec68'}

CALL_SITES = json.loads(Path(__file__).with_name('file_collection_call_sites.json').read_text())['sites']

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
        expected={(int(site['address'],16),int(site['target'],16),site['instruction']) for site in CALL_SITES}
        actual=set()
        targets={0x8018a6e4,0x8018b6c0,0x80189d38}
        for i in range(7):
            va,length=(struct.unpack_from('>I',header,j+i*4)[0] for j in (0x48,0x90))
            if not length:
                continue
            data=read(va,length)
            for offset in range(0,len(data)-3,4):
                word=struct.unpack_from('>I',data,offset)[0]
                if word>>26!=18 or not word&1:
                    continue
                displacement=word&0x3fffffc
                if displacement&0x2000000:
                    displacement-=0x4000000
                target=(displacement if word&2 else va+offset+displacement)&0xffffffff
                if target in targets:
                    actual.add((va+offset,target,data[offset:offset+4].hex()))
        if actual!=expected:
            raise ValueError('Direct DOL collection/init/mode call census changed')
    return dict(schema=1,scope='Complete collection/init/archive-mode/current-thread leaves;backend identities linked;no smart heap/worker/platform/source/runtime closure',bodies=len(BODIES),instruction_anchors=len(ANCHORS),data_spans=len(CONSTANTS),dol_direct_call_sites=len(CALL_SITES))


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
