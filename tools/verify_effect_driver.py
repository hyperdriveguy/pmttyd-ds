#!/usr/bin/env python3
"""Pin effect driver/pool init/release and selected US texture source read-only."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO, inventory

BODIES = {'__memAlloc': (2147680496,
                100,
                '51d2010e5ae0c3265971a8dfaee60b24489d8bf8eb2ca386f77c1d0ceb98712b'),
 '__memFree': (2147680452, 44, 'f4246f72f5658f00617a3a9672ef79813c867577a1079cde2f7830cef8c3f696'),
 '_unload': (2147515060, 376, 'e217052388ebb30ee45513043dab0e0a1008cddcb6ab92c8c3b78f117a575099'),
 'battle_exit': (2147956460,
                 448,
                 '500c637c3049e79b92269464c281a9064a518f136293594ba39c3aae63979c42'),
 'effAutoRelease': (2147871952,
                    136,
                    '42a4df29243ff186f507f9ada0e9fd6e851b91e2f828c50b7edec9dd0ca6b281'),
 'effInit': (2147872544, 136, '10803263b67111c6863a30e42a0f3651b99cfd6359163265374fa7a471736e2d'),
 'effInit64': (2149415456, 56, '5544117e2e22fb2f9e8d7ac5a366f0ac139019278b8f666da37b2bf323871275'),
 'effMain': (2147871356, 368, 'eea44f593e4a47a6bce6bb2d448383bbe830907ecd2f1955cfecc147b50d39e8'),
 'fileAllocf': (2149099036,
                200,
                '21c3389d3c270faf5d2ddd82752fe1e0117730ddba5e2dd75c5705bcab7ff67d'),
 'fileAsyncf': (2149097704,
                184,
                'a9d734557b2befccff653848a56af4b016f5c553e278bdcad661e14f4a17e150'),
 'fileFree': (2149098080, 68, '684126212d2c5ab90eac038c79edb88fa85ea3ed59cb02bcbb1297db7432b017'),
 'getMarioStDvdRoot': (2147513224,
                       8,
                       '20e17ad249044e215aba5f071125d98aee73da26094bbb4ff323cfcdf3574f7d'),
 'marioStMain': (2147940612,
                 568,
                 '74db581fe5f8665204b63c9706e483031f73edcc99e1596ce50b2a53db672f54'),
 'memset': (2147504308, 48, '4176c17fce5f28ccea737946be853ea45a125b52e9acab356ba54454aa352b5c')}

ANCHORS = {2147513224: '38628054',
 2147513228: '4e800020',
 2147515180: '38600000',
 2147515184: '480571a1',
 2147515188: '38600000',
 2147515192: '480334f1',
 2147515196: '4802fc75',
 2147871356: '9421ffe0',
 2147871360: '7c0802a6',
 2147871364: '90010024',
 2147871368: '93e1001c',
 2147871372: '3be00000',
 2147871376: '93c10018',
 2147871380: '93a10014',
 2147871384: '806d8fc0',
 2147871388: '83c30000',
 2147871392: '83a30004',
 2147871396: '4800005c',
 2147871400: '806d90b0',
 2147871404: '80030014',
 2147871408: '2c000000',
 2147871412: '41820014',
 2147871416: '801d0004',
 2147871420: '2c000001',
 2147871424: '40820038',
 2147871428: '48000010',
 2147871432: '801d0004',
 2147871436: '2c000000',
 2147871440: '40820028',
 2147871444: '801d0000',
 2147871448: '540007ff',
 2147871452: '4182001c',
 2147871456: '819d0010',
 2147871460: '280c0000',
 2147871464: '41820010',
 2147871468: '7fa3eb78',
 2147871472: '7d8903a6',
 2147871476: '4e800421',
 2147871480: '3bff0001',
 2147871484: '3bbd0028',
 2147871488: '7c1ff000',
 2147871492: '4180ffa4',
 2147871496: '80ad8fc0',
 2147871500: '806d90b0',
 2147871504: '80850014',
 2147871508: '8003016c',
 2147871512: '7c040040',
 2147871516: '418200b4',
 2147871520: '80650010',
 2147871524: '28030000',
 2147871528: '41820014',
 2147871532: '4812b735',
 2147871536: '806d8fc0',
 2147871540: '38000000',
 2147871544: '90030010',
 2147871548: '808d90b0',
 2147871552: '386d8fc4',
 2147871556: '8004016c',
 2147871560: '5400103a',
 2147871564: '7fe3002e',
 2147871568: '4bfa8839',
 2147871572: '3c80802c',
 2147871576: '7c661b78',
 2147871580: '38a41f20',
 2147871584: '7fe7fb78',
 2147871588: '7fe8fb78',
 2147871592: '38600004',
 2147871596: '38800000',
 2147871600: '4cc63182',
 2147871604: '4812b575',
 2147871608: '28030000',
 2147871612: '41820054',
 2147871616: '808d90b0',
 2147871620: '386d8fc4',
 2147871624: '8004016c',
 2147871628: '5400103a',
 2147871632: '7fe3002e',
 2147871636: '4bfa87f5',
 2147871640: '3c80802c',
 2147871644: '7c651b78',
 2147871648: '38841f20',
 2147871652: '7fe6fb78',
 2147871656: '7fe7fb78',
 2147871660: '38600004',
 2147871664: '4cc63182',
 2147871668: '4812ba69',
 2147871672: '808d8fc0',
 2147871676: '90640010',
 2147871680: '808d90b0',
 2147871684: '806d8fc0',
 2147871688: '8004016c',
 2147871692: '90030014',
 2147871696: '80010024',
 2147871700: '83e1001c',
 2147871704: '83c10018',
 2147871708: '83a10014',
 2147871712: '7c0803a6',
 2147871716: '38210020',
 2147871720: '4e800020',
 2147871952: '9421ffe0',
 2147871956: '7c0802a6',
 2147871960: '90010024',
 2147871964: 'bf61000c',
 2147871968: '7c7b1b78',
 2147871972: '3bc00000',
 2147871976: '3be00000',
 2147871980: '808d8fc0',
 2147871984: '83a40000',
 2147871988: '83840004',
 2147871992: '48000044',
 2147871996: '807c0000',
 2147872000: '546007ff',
 2147872004: '41820030',
 2147872008: '801c0004',
 2147872012: '7c00d800',
 2147872016: '40820024',
 2147872020: '281c0000',
 2147872024: '4182001c',
 2147872028: '28030000',
 2147872032: '41820014',
 2147872036: '809c000c',
 2147872040: '38600003',
 2147872044: '4bfd1399',
 2147872048: '93fc0000',
 2147872052: '3bde0001',
 2147872056: '3b9c0028',
 2147872060: '7c1ee800',
 2147872064: '4180ffbc',
 2147872068: 'bb61000c',
 2147872072: '80010024',
 2147872076: '7c0803a6',
 2147872080: '38210020',
 2147872084: '4e800020',
 2147872544: '9421fff0',
 2147872548: '7c0802a6',
 2147872552: '38600000',
 2147872556: '90010014',
 2147872560: '38000200',
 2147872564: '808d8fc0',
 2147872568: '90040000',
 2147872572: '808d8fc0',
 2147872576: '80040000',
 2147872580: '1c800028',
 2147872584: '4bfd11a9',
 2147872588: '80ad8fc0',
 2147872592: '38800000',
 2147872596: '90650004',
 2147872600: '806d8fc0',
 2147872604: '80030000',
 2147872608: '80630004',
 2147872612: '1ca00028',
 2147872616: '4bfa614d',
 2147872620: '806d8fc0',
 2147872624: '38800000',
 2147872628: '3800ffff',
 2147872632: '90830010',
 2147872636: '806d8fc0',
 2147872640: '90030014',
 2147872644: '806d8fc0',
 2147872648: '90830008',
 2147872652: '806d8fc0',
 2147872656: '9083000c',
 2147872660: '48178a8d',
 2147872664: '80010014',
 2147872668: '7c0803a6',
 2147872672: '38210010',
 2147872676: '4e800020',
 2147956588: '38600001',
 2147956592: '4bfeb561',
 2147956596: '38600001',
 2147956600: '4bfc78b1',
 2147956604: '4bfc4035',
 2149415456: '9421fff0',
 2149415460: '7c0802a6',
 2149415464: '38800000',
 2149415468: '38a00008',
 2149415472: '90010014',
 2149415476: '806dcb20',
 2149415480: '4be2d67d',
 2149415484: '806dcb20',
 2149415488: '38000000',
 2149415492: '90030004',
 2149415496: '80010014',
 2149415500: '7c0803a6',
 2149415504: '38210010',
 2149415508: '4e800020'}

CONSTANTS = {2147505176: '3c40804260427440',
 2147505184: '3da0804161adcf20',
 2150375200: '25732f652f25732f6566666563745f25732e74706c00',
 2151767776: '8041e908',
 2151767780: '8042018080420184804201888042018c804201908042019480420198',
 2151782976: '8041ecd0',
 2151806100: '2e00',
 2151809408: '6a700000757300006765000066720000737000006974000064750000'}

DIRECT_CALLS = {(2147515184, 2147871952),
 (2147940772, 2147871356),
 (2147941876, 2147872544),
 (2147956592, 2147871952)}

US_TEXTURE_SOURCE = {'path': 'e/us/effect_us.tpl',
 'sha256': '93ef5d667ded662d6abf13133082bef138f9c88132365ff8b11f8abf2129dd14',
 'size': 106880}

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
        calls=set()
        for i in range(7):
            va,size=(struct.unpack_from('>I',header,j+i*4)[0] for j in (0x48,0x90))
            if not size:
                continue
            code=read(va,size)
            for offset in range(0,len(code)-3,4):
                word=struct.unpack_from('>I',code,offset)[0]
                if word>>26!=18 or not word&1:
                    continue
                displacement=word&0x03fffffc
                if displacement&0x02000000:
                    displacement-=0x04000000
                site=va+offset
                target=(displacement if word&2 else site+displacement)&0xffffffff
                if target in (0x8005ea7c,0x8005ecd0,0x8005ef20):
                    calls.add((site,target))
        if calls!=DIRECT_CALLS:
            raise ValueError(f'DOL branch-shaped direct call census differs: {sorted(calls)}')
        for name,(address,size,digest) in BODIES.items():
            if hashlib.sha256(read(address,size)).hexdigest()!=digest:
                raise ValueError(f'Body mismatch: {name}')
        for address,value in {**ANCHORS,**CONSTANTS}.items():
            if read(address,len(value)//2).hex()!=value:
                raise ValueError(f'Instruction/constant mismatch: {address:#x}')
    item=next(r for r in inventory(iso)['files'] if r['path']==US_TEXTURE_SOURCE['path'])
    with Path(iso).open('rb') as disc:
        disc.seek(item['offset']);data=disc.read(item['size'])
    if item['size']!=US_TEXTURE_SOURCE['size'] or hashlib.sha256(data).hexdigest()!=US_TEXTURE_SOURCE['sha256']:
        raise ValueError('Original US effect texture library differs')
    return dict(schema=1,scope='Complete effect driver/init/release/N64 reset/root helper;file/heap/caller identities linked;selected US texture fingerprint;no runtime execution claim',
                bodies=len(BODIES),instruction_anchors=len(ANCHORS),data_spans=len(CONSTANTS),
                dol_direct_call_sites=len(DIRECT_CALLS))


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
