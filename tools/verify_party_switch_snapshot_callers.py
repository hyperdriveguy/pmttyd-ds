#!/usr/bin/env python3
"""Pin party switch and field export ownership and scoped snapshot call sites."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'BattleChangeParty': (2148500792,
                       380,
                       'ba0eeff0f15f9ed6cfc422489df6f89daabc4e331f8b1fc800a5c0f7f535a0fc'),
 'BattleGetFloorHeight': (2148498308,
                          80,
                          'da651e064b9f49e0f750687f1e78ba167ddb0a72cc618e4359ec849c1a865923'),
 'BattleGetMarioPtr': (2148501416,
                       108,
                       '26840147b00e408e2cd371d8c23ddd7057de2da195881a7ab014b758fa28f869'),
 'BattleGetPartyPtr': (2148501288,
                       128,
                       '618380e524d9f6805cd215b283aa4450e7e8fb392921ef72a3bec5b00cd8dd37'),
 'BattleSetMarioParamToFieldBattle': (2148502120,
                                      68,
                                      '4dd01678725a64489e55fee6785ff71fd042c061810acc0d1ada1d6d3c810ff8'),
 'BtlUnit_Entry': (2148700128,
                   1716,
                   '28c393e0dda25fa7af7e6c82f477f4067e7e5e7f6b50d09766996228814707f9'),
 'BtlUnit_EquipItem': (2148500252,
                       200,
                       '7aae8fadca55ab82af1fe42d237ad1e48df32046c404f9da1be67b3a5b416131'),
 'BtlUnit_GetHomePos': (2148698544,
                        28,
                        '6639432924f29e2decbdf026355694d80be38e03c1f91247af3e80060acbb1e5'),
 'BtlUnit_SetHomePos': (2148698528,
                        16,
                        'e119c3860b08a1fc12999740611e85f0381bf3a90295764811ed148987f5dc95'),
 'BtlUnit_SetParamFromPouch': (2148692276,
                               280,
                               '623f0b252e57adcaca4899666da787283a8a92a548231c29fa14c7ca343a0d3f'),
 'BtlUnit_SetParamToPouch': (2148691712,
                             232,
                             '0b7d7a71d5dd144db53ce9ff66bb1bfd52b4583e2c2b5bce505e150240cf4597'),
 'BtlUnit_Spawn': (2148699424,
                   232,
                   'b06ecb4643e82b445e286c8f0104fc552294a457047b710958077aa332968610'),
 'Btl_UnitSetup': (2148502188,
                   760,
                   '7629c39a04c07b612a1f7bf3d23e61a46bf187e422a4279a31f6d8eea9dc8844'),
 'btlseqEnd': (2149667380,
               5232,
               'ea77420a93d18acc970cb2ad2402cea39b6fa7b3e72953ebc9bf5759113e0cb0')}

ANCHORS = {2148500792: '9421ffd0',
 2148500796: '7c0802a6',
 2148500800: '90010034',
 2148500804: 'bf810020',
 2148500808: '7c7d1b78',
 2148500812: '83c31c68',
 2148500816: '480001d9',
 2148500820: '2c1e00e0',
 2148500824: '83830000',
 2148500828: '7c7f1b78',
 2148500832: '4180000c',
 2148500836: '2c1e00e7',
 2148500840: '4180000c',
 2148500844: '38600000',
 2148500848: '48000130',
 2148500852: '381eff20',
 2148500856: '3c608036',
 2148500860: '1ca00030',
 2148500864: '38800001',
 2148500868: '38033790',
 2148500872: '7c602a14',
 2148500876: '48030795',
 2148500880: '38000040',
 2148500884: '7c7e1b78',
 2148500888: '38600000',
 2148500892: '7c0903a6',
 2148500896: '38830120',
 2148500900: '7cbd202e',
 2148500904: '2c05ffff',
 2148500908: '41820028',
 2148500912: '7c05e000',
 2148500916: '40820010',
 2148500920: '801e0000',
 2148500924: '7c1d212e',
 2148500928: '48000014',
 2148500932: '801e0000',
 2148500936: '7c050000',
 2148500940: '40820008',
 2148500944: '7f9d212e',
 2148500948: '38630004',
 2148500952: '4200ffc8',
 2148500956: '7fc3f378',
 2148500960: '38810010',
 2148500964: '38a1000c',
 2148500968: '38c10008',
 2148500972: '480303c5',
 2148500976: '7fe3fb78',
 2148500980: '38810010',
 2148500984: '38a10014',
 2148500988: '38c10008',
 2148500992: '480303b1',
 2148500996: '7fc3f378',
 2148501000: '38800005',
 2148501004: '38a00000',
 2148501008: '4bfffd0d',
 2148501012: '806d1c70',
 2148501016: 'c0210010',
 2148501020: 'c041000c',
 2148501024: 'c0610008',
 2148501028: '4bfff561',
 2148501032: 'c001000c',
 2148501036: '7fc3f378',
 2148501040: 'c0610008',
 2148501044: 'ec40082a',
 2148501048: 'c0210010',
 2148501052: 'd041000c',
 2148501056: '48030361',
 2148501060: '801d0420',
 2148501064: '7c00e000',
 2148501068: '4082000c',
 2148501072: '801e0000',
 2148501076: '901d0420',
 2148501080: '881f0020',
 2148501084: '7fe3fb78',
 2148501088: '981e0020',
 2148501092: '881f0022',
 2148501096: '981e0022',
 2148501100: '881f0023',
 2148501104: '981e0023',
 2148501108: '881f0024',
 2148501112: '981e0024',
 2148501116: '4802e885',
 2148501120: '801f001c',
 2148501124: '7fc3f378',
 2148501128: '60000008',
 2148501132: '901f001c',
 2148501136: '881f0311',
 2148501140: '981e0311',
 2148501144: '881f0312',
 2148501148: '981e0312',
 2148501152: 'bb810020',
 2148501156: '80010034',
 2148501160: '7c0803a6',
 2148501164: '38210030',
 2148501168: '4e800020',
 2148502120: '9421fff0',
 2148502124: '7c0802a6',
 2148502128: '90010014',
 2148502132: '93e1000c',
 2148502136: '7c7f1b78',
 2148502140: '4bfffd2d',
 2148502144: '4802e481',
 2148502148: '7fe3fb78',
 2148502152: '4bfffca1',
 2148502156: '28030000',
 2148502160: '41820008',
 2148502164: '4802e46d',
 2148502168: '80010014',
 2148502172: '83e1000c',
 2148502176: '7c0803a6',
 2148502180: '38210010',
 2148502184: '4e800020',
 2148502328: '4802e5fd',
 2148502452: '4802e581',
 2148698528: 'd0230030',
 2148698532: 'd0430034',
 2148698536: 'd0630038',
 2148698540: '4e800020',
 2148698544: 'c0030030',
 2148698548: 'd0040000',
 2148698552: 'c0030034',
 2148698556: 'd0050000',
 2148698560: 'c0030038',
 2148698564: 'd0060000',
 2148698568: '4e800020',
 2148701748: '4bffdb01',
 2149671168: '4bf11035'}

CONSTANTS = {2147505184: '3da0804161adcf20',
 2151036816: '80393bec0000000004000002447a000000000000447a00000000000000000000000000000000000000000000000000008038ef340000000004000002447a000000000000447a00000000000000000000000000000000000000000000000000008037c79c0000000004000002447a000000000000447a0000000000000000000000000000000000000000000000000000803791f40000000004000002447a000000000000447a00000000000000000000000000000000000000000000000000008038b89c0000000004000002447a000000000000447a00000000000000000000000000000000000000000000000000008038780c0000000004000002447a000000000000447a0000000000000000000000000000000000000000000000000000803818c00000000004000002447a000000000000447a0000000000000000000000000000000000000000000000000000'}

CALL_SITES = {2148501116: 2148691712,
 2148502144: 2148691712,
 2148502164: 2148691712,
 2148502328: 2148692276,
 2148502452: 2148692276,
 2148701748: 2148692276,
 2149671168: 2148692276}

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
        sites={}
        for section in range(7):
            va,length=(struct.unpack_from('>I',header,j+4*section)[0] for j in (0x48,0x90))
            if not va: continue
            code=read(va,length)
            for offset in range(0,length,4):
                address=va+offset
                word=struct.unpack_from('>I',code,offset)[0]
                if word>>26!=18 or not word&1: continue
                delta=word&0x3fffffc
                if delta&0x2000000: delta-=0x4000000
                target=(delta if word&2 else address+delta)&0xffffffff
                if target in (0x80126f00,0x80127134): sites[address]=target
        if sites!=CALL_SITES: raise ValueError('Import/export direct DOL caller census mismatch')
        for name,(address,size,digest) in BODIES.items():
            if hashlib.sha256(read(address,size)).hexdigest()!=digest:
                raise ValueError(f'Body mismatch: {name}')
        for address,value in {**ANCHORS,**CONSTANTS}.items():
            if read(address,len(value)//2).hex()!=value:
                raise ValueError(f'Instruction/constant mismatch: {address:#x}')
    return dict(schema=1,scope='Four complete party switch field export and home leaves;seven scoped import-export DOL sites;setup/end/source/save/query/Game/runtime remain open',dol_direct_call_sites=len(sites),bodies=len(BODIES),instruction_anchors=len(ANCHORS),data_spans=len(CONSTANTS))


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
