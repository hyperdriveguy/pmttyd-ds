#!/usr/bin/env python3
"""Check event operand namespace and saved storage contracts against read-only US ISO."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'evtGetValue': (2147684960,
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
                 '07b1e305d3c2e3c1d3432397aa243d56c39c07fcafdb124586f1204b04f82921'),
 'evtGetWork': (2147711584, 32, 'fb90cf4f7cca69ea6a6b1e08fdada64981606fbeb9df3dbf43d2d2930532cdc4'),
 'swByteGet': (2148030508, 36, '3636c058f262a6dbbc9376acc145c21db6654a8a7040d27a49b17e61a96f31b9'),
 'swByteSet': (2148030544, 36, '64a3448bb937ea56ff3ce7601593c806241afbe6f1c0b8ec768d5ad7309ef0cd'),
 '_swByteGet': (2148030276, 16, 'e9b21259146c4db001c32a9b91d5c6ea63a326fc15c31e57531a62a3d0305140'),
 '_swByteSet': (2148030292, 16, '3d3c414d10b99307cbf0c02d156f47c12de782f4fea527bb9fc900c6638cebf2'),
 'swGet': (2148030644, 72, 'a59574da57fff9285c9a41134ae49f470ebb69e641a7f575d18f600d82f1dd8d'),
 'swSet': (2148030716, 64, 'd247509a985ee4d951d3508e437c7e43ba2105ba37f6087033b8980ac6491108'),
 'swClear': (2148030580, 64, '7a234c4d6e502221663ff1218ec97aaa23adc5461ae05fd6719b5be06a139f80'),
 '_swGet': (2148030372, 72, 'f8f0125d36ff1182bd86fc6bc8ad631b8aef6d671e277a1cd92b3d82c9d2039d'),
 '_swSet': (2148030444, 64, '6755b2cef1b0a553a8ab9398ae8552ed454ed32fb5bf9a2aaac318e6a6b2a45b'),
 '_swClear': (2148030308, 64, '96260fdfe35299b7b9cb98da893b1a399033d3a043639e91e21e7b30f76918ef')}

ANCHORS = {2147684988: '480067e5',
 2147685004: '4181000c',
 2147685028: '4181000c',
 2147685052: '4181006c',
 2147685144: 'fc00081e',
 2147685232: '7c7f0038',
 2147685284: '7c65182e',
 2147685292: '41810008',
 2147685304: '418102e8',
 2147685396: 'fc00081e',
 2147685436: '480543f1',
 2147685468: '480542e9',
 2147685500: '48054439',
 2147685532: '48054309',
 2147685600: '80630084',
 2147685692: '80a500dc',
 2147685756: '80630004',
 2147685916: '8063009c',
 2147685924: '41810008',
 2147685936: '41810070',
 2147686028: 'fc00081e',
 2147683616: '48006d41',
 2147683632: '4181000c',
 2147683656: '41810070',
 2147683752: 'fc00081e',
 2147683852: '7c05212e',
 2147683892: '7c05212e',
 2147683936: '7c64002e',
 2147683940: '7fe4012e',
 2147683972: '480549a9',
 2147683992: '480549b9',
 2147684028: '48054889',
 2147684044: '3863d180',
 2147684048: '48054885',
 2147684084: '480549c1',
 2147684108: '480549f1',
 2147684124: '48054959',
 2147684160: '48054865',
 2147684184: '48054895',
 2147684200: '480547fd',
 2147684296: '90040084',
 2147684432: '900400dc',
 2147684516: '80640004',
 2147684520: '93e40004',
 2147684560: '8064009c',
 2147684564: '93e4009c',
 2147682396: '48007205',
 2147682464: '41810028',
 2147682516: '41810060',
 2147682624: '41810078',
 2147682644: '7fe4182e',
 2147682652: '41810038',
 2147682768: '48054e5d',
 2147682900: '48054cf1',
 2147683020: '41810054',
 2147683088: 'c02285e4',
 2147683116: '41810054',
 2147683184: 'c02285e4',
 2147683232: '80630004',
 2147683364: '8063009c',
 2147683372: '41810038',
 2147681508: '4800757d',
 2147681548: '4181000c',
 2147681572: '41810098',
 2147681592: 'ec1f0032',
 2147681600: '7ce5202e',
 2147681604: 'fc00001e',
 2147681628: '7c05212e',
 2147681736: '41810080',
 2147681756: 'fc1f0000',
 2147681812: '90040084',
 2147681852: '90040084',
 2147681876: '41810080',
 2147681896: 'fc1f0000',
 2147681952: '900400dc',
 2147681992: '900400dc',
 2147682032: 'ec1f0032',
 2147682044: '80a40004',
 2147682072: '90040004',
 2147682176: '7c1e0000',
 2147682208: '80a4009c',
 2147682236: '9004009c',
 2148030520: '80630174',
 2148030536: '88630578',
 2148030556: '90830174',
 2148030572: '98830578',
 2148030284: '88630db8',
 2148030300: '98830db8',
 2148030688: '80650178',
 2148030772: '90050178',
 2148030636: '90050178',
 2148030416: '80650d78',
 2148030500: '90050d78',
 2148030364: '90050d78'}

CONSTANTS = {2151807512: '44800000',
 2151807516: '3a800000',
 2151807520: '00000000',
 2151807524: '3f800000',
 2150367152: '4330000080000000',
 2147505176: '3c40804260427440',
 2147505184: '3da0804161adcf20'}

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
    return dict(schema=1,scope='Complete local operand APIs/storage leaves; no persistence/execution claim',
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
