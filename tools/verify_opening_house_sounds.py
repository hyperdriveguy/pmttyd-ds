#!/usr/bin/env python3
"""Read-only source sound loop/caller/object/environment evidence for aaa."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
from rel_event_inventory import inventory_events
FUNCTIONS=[('evt_snd_sfxon_3d',0x801521dc,0xb4,'d611e0a1effcf2308e96379afa65c698b24b0ed3b453dc746069f12dd13cd9d0'),
 ('evt_mapobj_get_position',0x800382b0,0xcc,'0d4d00fc30a50bae05931e6225d0beaed3376312da2090999c09dd62a8506130'),
 ('mapObjGetPos',0x8001da88,0x140,'9b79a0f4f4f300c40eb933bd2edf366cd512338b1964ff82bfbfebbf288c8723'),
 ('mapObjGetPosSub',0x8001dbc8,0x56c,'5c963398fcfcb788e2ecd79bf912bcd05e999478a67c2e91e5bb6fb91c57b6fc'),
 ('_mapEnt',0x8002a780,0x588,'3bb8d600e61cc48de1452ddb60f6e63e4ba496cbd2dc5720ce19cbf126f06678'),
 ('psndENVOn_f_d',0x800d76e0,0x45c,'19b48a65e610f4842cd72cdeee6c3866be7967fc810b57a511f9b93c4a0904f2'),
 ('psndENVMain',0x800d7b64,0x1684,'a01ef91d990dd7f3ed8d2554bca6d864649703e4444e3b7a3ce9c6f2a307e155')]
def verify(iso,symbols):
    info=inventory(iso);f=next(f for f in info['files'] if f['path']=='rel/aaa.rel')
    with Path(iso).open('rb') as s:
        if s.read(8)!=b'G8ME01\0\0':raise ValueError('US disc0/revision0 required')
        s.seek(f['offset']);raw=s.read(f['size']);e=inventory_events(raw,symbols,'aaa',['evt_prologue'])['events'][0]
        if len(e['commands'])!=187 or e['sha256']!='833162e1181ca1e25faef38ecaaa34dfba08de0ad72c95cd6693f87a5461cf5f':raise ValueError('Original house event changed')
        c={r['address']:r for r in e['commands']};anchors=0
        for address,opcode,value in [(0xf10,10,3000),(0xf18,5,14),(0xf54,10,200),(0xf60,5,0),(0xf9c,10,400),(0xfbc,5,17),(0xff8,10,200)]:
            if c[address]['opcode']!=opcode or [o['raw'] for o in c[address]['operands']]!=[value]:raise ValueError('Source flight loop timing changed')
            anchors+=1
        for address,opcode,value in [(0xf00,50,0),(0x10fc,50,1),(0xfa4,24,1)]:
            if c[address]['opcode']!=opcode or [o['raw'] for o in c[address]['operands']]!=[4204967297,value]:raise ValueError('Source flight WW1 writer/condition changed')
            anchors+=1
        if c[0xfb0]['opcode']!=7:raise ValueError('Source loop break changed')
        anchors+=1
        for address,name,flight in [(0xf38,'SFX_EVT_OPN_PARETTA_MOVE1',True),(0xf80,'SFX_EVT_OPN_PARETTA_MOVE1',True),(0xfdc,'SFX_EVT_OPN_PARETTA_MOVE1',True),(0x1060,'SFX_EVT_POST_SHAKE1',False),(0x1308,'SFX_EVT_POST_SHAKE1',False),(0x118c,'SFX_DOOR_OPEN1',False),(0x1438,'SFX_DOOR_OPEN1',False),(0x1228,'SFX_DOOR_SHUT1',False),(0x15c4,'SFX_DOOR_SHUT1',False)]:
            args=c[address]['operands']
            if args[0].get('annotation')!='evt_snd_sfxon_3d' or args[1].get('identifier')!=name or [o['raw'] for o in args[2:]]!=[4264967296,4264967297,4294967196 if flight else 4264967298,0]:raise ValueError('Source house SFX key/position/control changed')
            anchors+=1
        for address,name,args in [(0xd20,'evt_snd_bgm_scope',[0,1]),(0x1590,'evt_snd_envoff',[0x4800]),(0x159c,'evt_snd_env_lpf',[0,800])]:
            row=c[address]['operands']
            if row[0].get('annotation')!=name or [o['raw'] for o in row[1:]]!=args:raise ValueError('Original environment/scope request changed')
            anchors+=1
        args=c[0xd10]['operands']
        if args[0].get('annotation')!='evt_snd_envon' or args[1]['raw']!=0x110 or args[2].get('identifier')!='ENV_OPN_AAA1':raise ValueError('Original environment name/control changed')
        anchors+=1
        s.seek(0x420);dol=struct.unpack('>I',s.read(4))[0];s.seek(dol);h=s.read(256)
        for name,a,n,digest in FUNCTIONS:
            for j in range(18):
                off,va,size=(struct.unpack_from('>I',h,b+4*j)[0] for b in [0,0x48,0x90])
                if va<=a and a+n<=va+size:s.seek(dol+off+a-va);body=s.read(n);break
            else:raise ValueError('Unmapped original service')
            if hashlib.sha256(body).hexdigest()!=digest:raise ValueError('Original service changed: '+name)
    return anchors
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--symbols',default='artifacts/reference/us_symbols.csv');a=p.parse_args()
    print('Original house audio: event187/hash, seven service hashes and',verify(a.iso,a.symbols),'sound/loop/environment anchors verified')
