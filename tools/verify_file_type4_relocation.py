#!/usr/bin/env python3
"""Pin complete type4 nested relocation handlers and containing dispatch."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO, inventory

BODIES = {'OSPanic': (2150201492,
             300,
             '603b450ea9e0952a1978ddffd0aaa26e5b008c84fdf1e76e1e5435673e9bc2ce'),
 'fileGarbageDataAdrClear': (2149101604,
                             1692,
                             'd1ac6fa343ee6316812f7fddff6e05b3ddc1cb80121737c7fc926d1c1c9ecb1e'),
 'fileGarbageDataAdrSet': (2149099928,
                           1676,
                           '6b0ebc30871104e5cbb4142d6d771ddc1a5bef525ebce63ab877f0a36f5767cf')}

ANCHORS = {2149099928: '9421fff0',
 2149099932: '7c0802a6',
 2149099936: '2804000a',
 2149099940: '90010014',
 2149099944: '93e1000c',
 2149099948: '7c7f1b78',
 2149099952: '41810660',
 2149099956: '3c608039',
 2149099960: '5480103a',
 2149099964: '38637618',
 2149099968: '7c03002e',
 2149099972: '7c0903a6',
 2149099976: '4e800420',
 2149100424: '807f0000',
 2149100428: '3c03ffe0',
 2149100432: '2800af30',
 2149100436: '41820020',
 2149100440: '3c608030',
 2149100444: '3ca08030',
 2149100448: '38639470',
 2149100452: '38800070',
 2149100456: '38a5947c',
 2149100460: '4cc63182',
 2149100464: '4810cce5',
 2149100468: '801f0008',
 2149100472: '7c00f840',
 2149100476: '40800454',
 2149100480: '7c00fa14',
 2149100484: '38800000',
 2149100488: '901f0008',
 2149100492: '480000a0',
 2149100496: '807f0008',
 2149100500: '54851b78',
 2149100504: '7c03282e',
 2149100508: '28000000',
 2149100512: '4182003c',
 2149100516: '7c1f0214',
 2149100520: '7c03292e',
 2149100524: '807f0008',
 2149100528: '7cc3282e',
 2149100532: '88060023',
 2149100536: '28000000',
 2149100540: '40820020',
 2149100544: '80660008',
 2149100548: '38000001',
 2149100552: '7c7f1a14',
 2149100556: '90660008',
 2149100560: '807f0008',
 2149100564: '7c63282e',
 2149100568: '98030023',
 2149100572: '807f0008',
 2149100576: '38a50004',
 2149100580: '7c03282e',
 2149100584: '28000000',
 2149100588: '4182003c',
 2149100592: '7c1f0214',
 2149100596: '7c03292e',
 2149100600: '807f0008',
 2149100604: '7cc3282e',
 2149100608: '88060002',
 2149100612: '28000000',
 2149100616: '40820020',
 2149100620: '80660008',
 2149100624: '38000001',
 2149100628: '7c7f1a14',
 2149100632: '90660008',
 2149100636: '807f0008',
 2149100640: '7c63282e',
 2149100644: '98030002',
 2149100648: '38840001',
 2149100652: '801f0004',
 2149100656: '5483043e',
 2149100660: '7c030040',
 2149100664: '4180ff58',
 2149100668: '48000394',
 2149101584: '80010014',
 2149101588: '83e1000c',
 2149101592: '7c0803a6',
 2149101596: '38210010',
 2149101600: '4e800020',
 2149101604: '9421fff0',
 2149101608: '7c0802a6',
 2149101612: '90010014',
 2149101616: '93e1000c',
 2149101620: '88030001',
 2149101624: '806300a0',
 2149101628: '7c000774',
 2149101632: '2800000a',
 2149101636: '83e30000',
 2149101640: '41810664',
 2149101644: '3c608039',
 2149101648: '5400103a',
 2149101652: '38637644',
 2149101656: '7c03002e',
 2149101660: '7c0903a6',
 2149101664: '4e800420',
 2149102112: '807f0000',
 2149102116: '3c03ffe0',
 2149102120: '2800af30',
 2149102124: '41820020',
 2149102128: '3c608030',
 2149102132: '3ca08030',
 2149102136: '38639470',
 2149102140: '38800099',
 2149102144: '38a5947c',
 2149102148: '4cc63182',
 2149102152: '4810c64d',
 2149102156: '801f0008',
 2149102160: '7c00f840',
 2149102164: '41800458',
 2149102168: '38a00000',
 2149102172: '480000a4',
 2149102176: '807f0008',
 2149102180: '54a61b78',
 2149102184: '7c63302e',
 2149102188: '28030000',
 2149102192: '4182003c',
 2149102196: '88030023',
 2149102200: '28000000',
 2149102204: '41820020',
 2149102208: '38000000',
 2149102212: '98030023',
 2149102216: '807f0008',
 2149102220: '7c63302e',
 2149102224: '80030008',
 2149102228: '7c1f0050',
 2149102232: '90030008',
 2149102236: '807f0008',
 2149102240: '7c03302e',
 2149102244: '7c1f0050',
 2149102248: '7c03312e',
 2149102252: '807f0008',
 2149102256: '38860004',
 2149102260: '7c63202e',
 2149102264: '28030000',
 2149102268: '41820040',
 2149102272: '88030002',
 2149102276: '28000000',
 2149102280: '41820020',
 2149102284: '38000000',
 2149102288: '98030002',
 2149102292: '807f0008',
 2149102296: '7c63202e',
 2149102300: '80030008',
 2149102304: '7c1f0050',
 2149102308: '90030008',
 2149102312: '809f0008',
 2149102316: '38660004',
 2149102320: '7c04182e',
 2149102324: '7c1f0050',
 2149102328: '7c04192e',
 2149102332: '38a50001',
 2149102336: '801f0004',
 2149102340: '54a3043e',
 2149102344: '7c030040',
 2149102348: '4180ff54',
 2149102352: '801f0008',
 2149102356: '7c1f0050',
 2149102360: '901f0008',
 2149102364: '48000390',
 2149103276: '80010014',
 2149103280: '83e1000c',
 2149103284: '7c0803a6',
 2149103288: '38210010',
 2149103292: '4e800020'}

CONSTANTS = {2150601840: '66696c656d67722e6300',
 2150601852: '696e76616c69642076657273696f6e206e756d62657220666f7220746578747572652070616c6574746500',
 2151249432: '8018b0108018a9cc8018ab048018ab708018ab888018ac808018ae588018ae708018aea08018afc08018affc',
 2151249476: '8018b6ac8018b0648018b19c8018b2088018b2208018b3208018b4f48018b50c8018b53c8018b65c8018b698'}

US_EFFECT_SOURCE = {'path': 'e/us/effect_us.tpl', 'size': 106880, 'sha256': '93ef5d667ded662d6abf13133082bef138f9c88132365ff8b11f8abf2129dd14', 'magic': '0x20af30', 'count': 82, 'table': '0xc', 'image_slots': 82, 'image_headers': 82, 'image_flags': [0], 'palette_slots': 0, 'palette_headers': 0, 'palette_flags': []}

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
    row=next(r for r in inventory(iso)['files'] if r['path']==US_EFFECT_SOURCE['path'])
    with Path(iso).open('rb') as disc:
        disc.seek(row['offset']);data=disc.read(row['size'])
    if row['size']!=US_EFFECT_SOURCE['size'] or hashlib.sha256(data).hexdigest()!=US_EFFECT_SOURCE['sha256']:
        raise ValueError('US effect TPL source fingerprint differs')
    magic,count,table=struct.unpack_from('>III',data)
    images=[];palettes=[]
    for i in range(count):
        image,palette=struct.unpack_from('>II',data,table+i*8)
        if image:
            images.append((image,data[image+0x23]))
        if palette:
            palettes.append((palette,data[palette+2]))
    observed=dict(path=row['path'],size=row['size'],sha256=hashlib.sha256(data).hexdigest(),magic=hex(magic),count=count,table=hex(table),image_slots=len(images),image_headers=len(set(x[0] for x in images)),image_flags=sorted(set(x[1] for x in images)),palette_slots=len(palettes),palette_headers=len(set(x[0] for x in palettes)),palette_flags=sorted(set(x[1] for x in palettes)))
    if observed!=US_EFFECT_SOURCE:
        raise ValueError('US effect TPL descriptor/flag source summary differs')
    return dict(schema=1,scope='Containing Set/Clear and panic identities;complete dispatch/type4 handlers;type5/texture/platform/source/runtime remain open',bodies=len(BODIES),instruction_anchors=len(ANCHORS),data_spans=len(CONSTANTS),us_effect_source=observed)


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
