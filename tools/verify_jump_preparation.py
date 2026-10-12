#!/usr/bin/env python3
"""Verify US Jump preparation/position/movement-sound contracts read-only."""
import argparse
import hashlib
import json
from pathlib import Path
import struct

from iso_assets import DEFAULT_ISO

BODIES = {
    'btlevtcmd_GetMoveFrame': (0x8010c8c0,0x1b4,'4a6037f29d16423fc5592085b654c88240f91a96a89300e181913d17296c2fc0'),
    'btlevtcmd_MovePosition': (0x8010ca74,0x334,'af9ff1f74966b242c18629523645afc9ce856b681f94f4942e4fcf71bd7a0c19'),
    'btlevtcmd_GetTakeoffPosition': (0x80109054,0x354,'8526e7e1fe793874d7ab406629c24983cf0ac09ee233cdc280f3b9a539015edb'),
    'BtlUnit_GetHomePos': (0x801289b0,0x1c,'6639432924f29e2decbdf026355694d80be38e03c1f91247af3e80060acbb1e5'),
    'BtlUnit_GetHitPos': (0x801289ec,0x124,'1e4075a1a0394e7a6ca9147538850c5ee1b6f9da6ec19c2b867fc1a78cb01e0c'),
    'BtlUnit_GetPartsWorldPos': (0x80128b10,0xe0,'c65dcce55ccd95528006827b2a7e6117ec84046d1200e4b94d957bf436cac56e'),
    'BtlUnit_GetPartsOffsetPos': (0x801283bc,0x1c,'b5b046aad569ac630bce20e1ad527e3ea1fd5de73a8f537724b4c09b0033de07'),
    'BattleGetUnitPartsPtr': (0x800f8874,0x40,'9316955e42b9b4e89e32ed369ba66b744315939ba43a4cfdabbdaab2debf9289'),
    'btlevtcmd_GetHitPos': (0x801120a8,0x11c,'3e457d5ba05522ade7f6c78fd6ea85bef5cc64c7af790c2c14c46faf79f0b983'),
    '_MoveSoundControlCore': (0x80104850,0x180,'ecddbf2486b3e80b8b80bb0e568dea9dd8f751ce6d2ea6805db10492ba580af5'),
    '_MoveSoundControlEndCore': (0x801047bc,0x44,'ce845c3eeadd4d3c7d4fa9fd36a05b98698f270791ed348918da87275c315b91'),
    '_UnitMoveSoundControl': (0x80104a24,0x54,'bf3f69c4da79a3f7f55c9f6ecc7749e66c9160f44e5e277d57f6e4cf711bea28'),
    '_UnitMoveSoundControlEnd': (0x8010482c,0x24,'24c7ac14331b92349630b2b969d476e6a3368775d4edfc8f4e42a4af1510b8b0'),
}
ANCHORS = {
    0x8010c944:'4bf2491d',0x8010c980:'4bf23ec1',0x8010c988:'83bd0014',
    0x8010c9d8:'c3a2ae4c',0x8010c9f4:'fc40101e',0x8010ca10:'ec220f7c',
    0x8010ca18:'40810008',0x8010ca28:'38a00001',0x8010ca30:'28004d80',
    0x8010cb9c:'41820170',0x8010cbb4:'933e016c',0x8010cbe0:'4801b5c9',
    0x8010cc64:'901e016c',0x8010cc78:'901e016c',0x8010ccb4:'ec22093c',
    0x8010ccc4:'d01e0170',0x8010ccf4:'d01e0170',0x8010cd08:'48019881',
    0x8010cd1c:'48018e55',0x8010cd48:'41810028',0x8010cd64:'4bff7ac9',
    0x8010cd68:'38600001',0x8010cd74:'4bff7cb1',
    0x801090d0:'7ca62b78',0x8010912c:'4801f8c1',0x80109138:'7c660774',
    0x8010914c:'38e41240',0x80109158:'38800000',0x80109164:'7c090774',
    0x80109168:'4bff6459',0x80109180:'4bff5a49',0x80109184:'88010a94',
    0x8010918c:'40810178',0x801091f4:'40810008',0x801091fc:'7c1ab840',
    0x80109224:'1c00ffc4',0x80109260:'1c00003c',0x801092a4:'1c00ffe2',
    0x801092e0:'1c00001e',0x8010931c:'1c00003c',0x80109348:'fc00001e',
    0x801289b0:'c0030030',0x801289c0:'c0030038',0x80128b38:'540000c7',
    0x80128b84:'889f0189',0x80128bb4:'ec03007a',0x80128bc4:'ec01002a',
    0x80128a30:'8be5000a',0x80128a58:'c01a0114',0x80128a64:'ec210032',
    0x80128a7c:'ec22007a',0x80128a9c:'ec04007a',0x80128ab0:'ec02007a',
    0x80128ac8:'ec03007a',0x80128ae0:'ec04007a',0x80128af4:'ec02007a',
    0x8011210c:'4bfe6769',0x8011213c:'41820020',0x80112174:'fc00001e',
    0x801048dc:'3c030001',0x80104900:'907f0018',0x80104940:'4bfd4db9',
    0x80104958:'b01f0012',0x80104964:'40800058',0x80104970:'b01f0014',
    0x8010497c:'41820014',0x801049b8:'480216d1',0x801047e8:'901f0018',
}
CONSTANTS = {0x80422280:'00000000',0x8042228c:'3f800000',
             0x802ee608:'4330000080000000',0x802f00b8:'4330000080000000',
             0x804226a8:'3f800000'}


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
        actor=0xfffffffe
        local=lambda i:0xfe363c80+i
        for address in (0x80355f64,0x803560a0):
            if struct.unpack('>8I',read(address,32))!=(0x7005b,0x80109054,
                actor,local(3),local(4),local(0),local(1),local(2)):
                raise ValueError('Jump takeoff caller mismatch')
        if struct.unpack('>8I',read(0x80355fb4,32))!=(0x7005b,0x8010c8c0,
            actor,local(0),local(1),local(2),0,local(7)):
            raise ValueError('Jump move-frame query mismatch')
        for address in (0x80356054,0x803561ac,0x80356ba8):
            if struct.unpack('>9I',read(address,36))!=(0x8005b,0x8010ca74,
                actor,local(0),local(1),local(2),0,0xffffffff,0):
                raise ValueError('Jump approach/return movement mismatch')
    return dict(schema=1,scope='Static US preparation/position/sound bodies;no execution/tick claim',
                bodies=len(BODIES),instruction_anchors=len(ANCHORS),constants=len(CONSTANTS),
                ordinary_callers=6)


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
