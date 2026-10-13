#!/usr/bin/env python3
"""Verify original US demo scene and final A/START gate read-only.

Static source anchors only; no original-runtime or numeric trace claim.
"""
import argparse
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
from rel_event_inventory import inventory_events


def verify(iso,symbols):
    info=inventory(iso)
    if info['disc_id']!='G8ME01':raise ValueError('US source required')
    resource=next(f for f in info['files'] if f['path']=='rel/dmo.rel')
    with Path(iso).open('rb') as source:
        source.seek(6)
        if source.read(2)!=bytes(2):raise ValueError('US disc0 revision0 required')
        source.seek(resource['offset']);raw=source.read(resource['size'])
        source.seek(0x420);dol=struct.unpack('>I',source.read(4))[0]
        source.seek(dol);header=source.read(256)
        def read(address,size):
            for i in range(18):
                off,va,length=(struct.unpack_from('>I',header,b+i*4)[0] for b in [0,0x48,0x90])
                if va<=address and address+size<=va+length:
                    source.seek(dol+off+address-va);data=source.read(size)
                    if len(data)!=size:raise ValueError('Truncated DOL')
                    return data
            raise ValueError('Constant outside DOL')
        def string(address):return read(address,96).split(bytes(1))[0].decode('shift_jis')
        rows={0x80332b28:['P_S_1','P_T_1','P_W_1'],0x80332b7c:['P_S_7','P_T_13','P_W_7'],
              0x80332bd0:['P_S_8','P_T_14','P_W_8'],0x80332c24:['P_S_4','P_T_4','P_W_4'],
              0x80332c78:['S_1','T_1','W_1']}
        for address,expected in rows.items():
            row=read(address,84)
            got=[string(struct.unpack_from('>I',row,offset)[0]) for offset in [8,20,24]]
            if got!=expected:raise ValueError('Original demo tribe pose mapping changed')
    events=inventory_events(raw,symbols,'dmo',['demo_evt','buttonCheck','dmo_00_init_evt'])['events']
    expected=[(392,'3e3f3b88dd5e9c8268dfdb58cf0df3120b0548814225041879830c3e637bc6c0'),
              (12,'c178ed3cafa4d6bcc0df930a54bec44c1f412b19ce91bbb8903d1de791601e33'),
              (63,'517da311a31b22dbb73a74189d18fe40d7bcef08de5734b13a44f445a5fbc77a')]
    for event,(count,digest) in zip(events,expected):
        if len(event['commands'])!=count or event['sha256']!=digest:raise ValueError('Original demo source changed')
    commands={c['address']:c for event in events for c in event['commands']}
    def signed(v):return v if v<0x80000000 else v-0x100000000
    def number(v):return signed((v+230000000)&0xffffffff)/1024 if -250000000<signed(v)<=-210000000 else signed(v)
    anchors={
        0x2fb8:('evt_npc_move_position',[0,450,0,60.,1]),
        0x3000:('evt_cam3d_evt_set',[0,100,834,0,50,437,800,11]),
        0x3060:('evt_cam3d_evt_set',[0,200,837,0,200,437,4000,11]),
        0x3110:('evt_cam3d_evt_set',[0,89,739,0,44,442,0,11]),
        0x31c0:('evt_cam3d_evt_set',[-100,89,739,-100,44,442,1000,11]),
        0x3200:('evt_npc_move_position',[-55,450,0,60.,1]),
        0x32f4:('evt_cam3d_evt_set',[-120,89,739,-120,44,442,1000,11]),
        0x331c:('evt_npc_move_position',[-80,450,0,60.,1]),
        0x3474:('evt_cam3d_evt_set',[-450,250,1030,-135,100,482,0,11]),
        0x38c4:('evt_cam3d_evt_set',[-90,89,739,-90,44,442,0,11]),
        0x3914:('evt_cam3d_evt_set',[-85,50,590,-85,28,442,15000,11]),
        0x3d78:('evt_npc_set_position',[-2,0,550]),
        0x3de8:('evt_npc_set_position',[-145,0,450]),
        0x3e58:('evt_npc_set_position',[-125,0,450]),
        0x4038:('evt_cam3d_evt_set',[0,385,1473,0,100,0,0,11])}
    for address,(name,expected) in anchors.items():
        command=commands[address];args=command['operands']
        start=2 if name.startswith('evt_npc_') else 1
        if command['opcode']!=91 or args[0].get('annotation')!=name or [number(o['raw']) for o in args[start:]]!=expected:
            raise ValueError(f'Original scene anchor changed at {address:x}')
    gate=commands[0x2374]
    if gate['opcode']!=30 or [o['raw'] for o in gate['operands']]!=[4264967296,0x1100]:raise ValueError('Button trigger guard changed')
    final=commands[0x3ab8]
    if final['opcode']!=24 or [o['raw'] for o in final['operands']]!=[4204967306,0]:raise ValueError('Final WW10 input guard changed')
    for address,amount in [(0x35f0,60),(0x30a4,500),(0x3158,300),(0x3a8c,300),(0x3aa8,2000)]:
        c=commands[address]
        if c['opcode']!=(5 if address==0x35f0 else 10) or c['operands'][0]['raw']!=amount:raise ValueError('Demo source loop/wait changed')
    for address,value in [(0x3610,.0830078125),(0x361c,.166015625),(0x33d0,.099609375)]:
        if number(commands[address]['operands'][1]['raw'])!=value:raise ValueError('Chest source float operand changed')
    return len(anchors)+2+5+3+5


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO)
    p.add_argument('--symbols',default='artifacts/reference/us_symbols.csv');a=p.parse_args()
    print(f'US demo392/button12/init63 commands and {verify(a.iso,a.symbols)} scene/guard/wait/float/tribe anchors verified read-only')
if __name__=='__main__':main()
