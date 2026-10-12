#!/usr/bin/env python3
"""Check original US physical-input and battle-pad cadence evidence read-only.

Reports body identities and check counts only; never executes original code.
Some containing bodies are documented only in the cited ordering regions.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {
    'makeKey': (0x80005ff8,0x504,'a948a6b1fb629e3707629d2793bcd9362221e4f32f5f5954363b2e28983fbbda'),
    'DEMOPadRead': (0x8002f0c4,0xc0,'1fcfaadfa3b4b47cc8a8c59a0c66c73b31c0a779744b748f666c045d28f2d2e9'),
    'DEMOPadCopy': (0x8002f184,0x1b8,'c5cf32aa41aba46422e4b302436a9115f6c365fc83046d829f8ed3b2cb74e2f6'),
    'BattlePadManager': (0x8011a43c,0x50,'8003c3ad38eb531c51c884b9aadff74101fc69bb5e8035bda9e1819f7517d55b'),
    'BattleMain': (0x800f8e28,0xdc,'bf6aee153d46d818476c124121653739778fdcc2d73a958962b95251be990f96'),
    'battleMain': (0x800f904c,0x48,'748302b617d70e9e5e45de1a2f9248cd2100f499a5f5a04ff9a281acaa96e28f'),
    'BattleActionCommandManager': (0x800fa5b0,0x7c,'66404be99cc28c734fec611846176691e59af5c913523e1528cd7c114d0a0df6'),
    'BtlPad_WorkUpdate': (0x8011a0ac,0x2e0,'cbfd99f6310ac669a0666fc963038b1f0859aa948465d5ef2ed2db4d4a9609b9'),
    'marioStMain': (0x8006f904,0x238,'74db581fe5f8665204b63c9706e483031f73edcc99e1596ce50b2a53db672f54'),
    'evtmgrMain': (0x800368d0,0x1ec,'6d9624873872aee5e5bda1c660a5b81401eda51cdc9643c9e464ef315e0150f4'),
}
ANCHORS = {
    0x8000602c:'a0840010',0x80006044:'90c71388',0x80006074:'34c6ffff',
    0x800060a0:'1ce70006',0x800060d4:'1ce70018',0x800060fc:'38a5001e',
    0x80006128:'a0a40000',0x80006140:'90e81338',0x80006170:'34e7ffff',
    0x8000619c:'1d080006',0x800061d0:'1d080018',0x80006204:'a0a4000e',
    0x80006234:'88ac0002',0x8000629c:'88ac0007',0x80006320:'540004ea',
    0x80006324:'28001c00',0x800063f0:'7c0b51ae',0x80006430:'2c000000',
    0x80006444:'88031310',0x80006460:'880313d4',0x800064cc:'980313d4',
    0x800064e4:'90031324',
    0x8002f108:'7c600775',0x8002f114:'2c00fffd',0x8002f12c:'2c00ffff',
    0x8002f154:'3bbd001e',0x8002f158:'3bff000c',
    0x8002f18c:'2c00fffd',0x8002f190:'41820184',0x8002f1a0:'2c00ffd0',
    0x8002f1b8:'4081000c',0x8002f258:'b0040012',0x8002f268:'b0040014',
    0x8002f280:'b004000c',0x8002f294:'b004000e',0x8002f2ac:'b0040016',
    0x8002f300:'90a40000',0x8002f30c:'90040008',0x8002f318:'b0040014',
    0x8011a458:'387f0f2c',0x8011a46c:'3bff01fc',0x8011a470:'2c1e0004',
    0x800fa5c8:'387f1d1c',0x800fa5b8:'38800000',0x800fa5d4:'280c0000',
    0x800fa5e4:'4e800421',0x800fa5f4:'901f1ca0',
    0x800f8e7c:'2c030004',0x800f8e84:'38600000',
    0x800f8e94:'546004e7',0x800f8e9c:'546004a5',0x800f8eb0:'2c030002',
    0x800f8ecc:'901f0ef4',0x800f8edc:'3ca00700',
    0x800f9064:'4182001c',0x800f9078:'64008000',
    0x8011a108:'b0c30000',0x8011a168:'98830007',
    0x8011a16c:'98a30008',0x8011a170:'98a30009',
    0x80036a40:'d0180168',0x80036a50:'ec00f828',
    0x80036a64:'4082000c',0x80036a74:'4082ffd0',
}
CALLS = {
    0x80006008:0x8002f0c4,
    0x8002f0dc:0x802a74f8,0x8002f0e4:0x802a67f8,
    0x8002f144:0x8002f184,0x8002f16c:0x802a7184,
    0x8011a464:0x8011a0ac,0x800fa5cc:0x8011a0ac,
    0x8006f928:0x80005ff8,0x8006f92c:0x8002e228,
    0x8006f940:0x800f904c,0x8006f954:0x800368d0,
    0x800f9068:0x800f8e28,
    0x800f8e40:0x800f7b28,0x800f8e44:0x8011a43c,
    0x800f8e48:0x8011f1ec,0x800f8e50:0x800fa5b0,
    0x800f8e54:0x80149798,0x800f8e58:0x801aa168,
    0x800f8e5c:0x80139150,0x800f8e60:0x802168a4,
    0x800f8e64:0x801a836c,0x800f8e68:0x80203aa0,
    0x800f8e6c:0x801024f0,
    0x800fa614:0x80010bc4,
    0x80006458:0x802a77f8,0x8000649c:0x802a77f8,
    0x800064ac:0x802a77f8,0x800064bc:0x802a77f8,
}


def verify(iso):
    with Path(iso).open('rb') as disc:
        if disc.read(8)!=b'G8ME01\0\0':raise ValueError('US G8ME01 disc0 revision0 required')
        disc.seek(0x420);dol_base=struct.unpack('>I',disc.read(4))[0]
        disc.seek(dol_base);header=disc.read(256)
        def read(address,size):
            positions=[]
            for i in range(18):
                off,va,length=(struct.unpack_from('>I',header,k+i*4)[0] for k in (0,0x48,0x90))
                if va<=address and address+size<=va+length:positions.append(dol_base+off+address-va)
            if len(positions)!=1:raise ValueError(f'Unmapped/ambiguous DOL address {address:#x}')
            disc.seek(positions[0]);data=disc.read(size)
            if len(data)!=size:raise ValueError('Truncated binary')
            return data
        for name,(address,size,digest) in BODIES.items():
            if hashlib.sha256(read(address,size)).hexdigest()!=digest:raise ValueError(f'Body mismatch: {name}')
        for address,expected in ANCHORS.items():
            if read(address,4).hex()!=expected:raise ValueError(f'Anchor mismatch: {address:#x}')
        for site,target in CALLS.items():
            expected=0x48000001|((target-site)&0x03fffffc)
            if struct.unpack('>I',read(site,4))[0]!=expected:raise ValueError(f'Direct call mismatch: {site:#x}')
        if read(0x804151a8,16).hex()!='80000000400000002000000010000000':raise ValueError('Pad reset masks mismatch')
        if read(0x804221a8,4)!=struct.pack('>f',900.0):raise ValueError('AC display sort value mismatch')
        if read(0x8041fa30,4)!=struct.pack('>f',1.0):raise ValueError('Event execution credit unit mismatch')
    return dict(schema=1,scope='Static US physical input and battle/event order; no original trace',
                bodies=[dict(name=n,address=a,size=s,sha256=d) for n,(a,s,d) in BODIES.items()],
                instruction_anchors=len(ANCHORS),direct_call_checks=len(CALLS),data_checks=3,
                limitation='Containing hashes do not imply full marioStMain/sequence/event/hardware semantics')


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',type=Path)
    args=p.parse_args()
    if args.output and args.output.resolve()==Path(args.iso).resolve():p.error('Cannot overwrite ISO')
    result=verify(args.iso)
    if args.output:
        args.output.parent.mkdir(parents=True,exist_ok=True)
        args.output.write_text(json.dumps(result,indent=2)+'\n')
    print(f'Verified {len(BODIES)} body hashes,{len(ANCHORS)} instruction anchors,'
          f'{len(CALLS)} direct calls and3 data bindings')


if __name__=='__main__':main()
