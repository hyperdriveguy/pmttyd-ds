#!/usr/bin/env python3
"""Check selected-target, weapon selection and cost contracts against read-only US ISO."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {
    'btlevtcmd_GetSelectEnemy': (0x8010b540,0xa4,'c1f0b52f18463fd0dff03de9285079892f2d86bfd639a8435ae4797913e2f7c2'),
    'btlevtcmd_CommandGetWeaponAddress': (0x801057e8,0xa8,'29a89f4a5553572157bcc7aa3b846a811d6e99f9c4b5b18f676c9de39eaf08c4'),
    'btlevtcmd_PayWeaponCost': (0x801059a0,0x74,'b5c1d71536154c1fad345f8a59bfe7dfb8ea946f6c38a1268ace66c643fd20ca'),
    'BattleGetSelectWeapon': (0x8012021c,0x4c,'45a0fd19eb7bbc1234c94071ef9ce1097f883f108ed85ea2d6762d663e114cf7'),
    '_btlcmd_GetCursorPtr': (0x80124cb8,0x110,'503374044e2b5599babc1e0f64c05b7338460ecca99246067924e0310ad2049b'),
    'BtlUnit_GetWeaponCost': (0x80126968,0x158,'cd640b206f92d2d44e49fa12a881811e0852aed175f631799530c2ada575183c'),
    'BtlUnit_CheckWeaponCost': (0x801268fc,0x6c,'384f3b85454bd3a739440e4d352fa4869693a5eabf0e4e83a64e776c5bca27bb'),
    'BtlUnit_PayWeaponCost': (0x80126840,0xbc,'3af98cb8a5da40e1869ea5aeb8f886b55efb2667b2a18c422fed86fa4ede7e9c'),
    'BtlUnit_GetFp': (0x80126c64,0x44,'cb123e5e1db454a414ad27a139bc0a6fb3c73c27a944b899b68f7db11d4bf342'),
    'BtlUnit_SetFp': (0x80126c10,0x54,'8ff4e4fb44a117da0288ffeeb5e9c70d640d45be2bb0082bd45553c015154d70'),
    'BattleGetMarioPtr': (0x800f87a8,0x6c,'26840147b00e408e2cd371d8c23ddd7057de2da195881a7ab014b758fa28f869'),
    'BtlActRec_AddPoint': (0x8018f96c,0x24,'a1ed136b3cb8e066c697b9ba262f2173daa8aa3fa5bfe4afa37d7ed84f9e04ea'),
    'BtlActRec_AddCount': (0x8018f990,0x18,'7da17f95b0aafc891c40bda14a9da69e12ec97b1175122cd7298b314d550843f'),
    'pouchEquipCheckBadge': (0x800d3300,0xc8,'2800af1700923fd4deabfe11a78b7b85163bb9be22c7fdab73bf456b29febec1'),
    'pouchGetAP': (0x800d3c44,0xc,'79c1fc048913dd840527483bb1efe33de1736b03fbb67c1d99b11cae1a9f7aaa'),
    'pouchAddAP': (0x800d3c00,0x44,'acc2b9ee0f003c46dc0cbb4649e0992c748a1567245debe39edce36aa5a03f3c'),
}
ANCHORS = {
    0x8010b55c:'88040e94',0x8010b568:'7c000775',
    0x8010b594:'88060ab7',0x8010b5a0:'88050a6d',
    0x8010b5a8:'1ca00024',0x8010b5b4:'a8be0000',0x8010b5bc:'a8be0002',
    0x80126a84:'2c1f0000',0x80126a8c:'881d02f5',0x80126a90:'7fe0f851',
    0x80126a94:'41810008',
}
CONSTANTS = {0x802f00b8:'4330000080000000',0x802f00d0:'4000000000000000'}

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
        expected=(0x80124d28,0x80124d34,0x80124d40,0x80124d4c,
                  0x80124d58,0x80124d64,0x80124d88,0x80124d94,
                  0x80124da0,0x80124db4,0x80124dac,0x80124db4,
                  0x80124db4,0x80124db4,0x80124d04)
        if struct.unpack('>15I',read(0x80364108,60))!=expected:
            raise ValueError('Cursor dispatch mismatch')
        for weapon in (0x803538a8,0x80353de8):
            if read(weapon+8,4)!=bytes(4) or read(weapon+0x11,2)!=bytes(2):
                raise ValueError('Ordinary weapon FP/AP mismatch')
        local=lambda i:0xfe363c80+i
        calls=((0x80355db4,0x8010b540,(local(3),local(4))),
               (0x80355ddc,0x801057e8,(0xfffffffe,local(12))),
               (0x80355e08,0x801059a0,(0xfffffffe,local(12))))
        for address,callee,args in calls:
            if struct.unpack('>4I',read(address,16))!=(0x3005b,callee,*args):
                raise ValueError(f'Ordinary Jump service caller mismatch: {address:#x}')
    return dict(schema=1,scope='Static US selection/cost contracts; no execution claim',
                bodies=len(BODIES),instruction_anchors=len(ANCHORS),
                constants=len(CONSTANTS),cursor_entries=15,ordinary_weapons=2,
                ordinary_callers=3)


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
