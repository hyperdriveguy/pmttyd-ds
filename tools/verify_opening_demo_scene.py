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
        # Original _load compares dmo_00 and calls mapLoad with gor_01 only
        # on equality. Other names use the ordinary requested map pointer.
        alias={0x80007608:0x3c80802c,0x80007614:0x3be4f268,
               0x80007668:0x389f0064,0x8000766c:0x48265df9,
               0x80007670:0x2c030000,0x80007674:0x40820010,
               0x80007678:0x387f006c,0x8000767c:0x4802168d}
        for address,word in alias.items():
            if struct.unpack('>I',read(address,4))[0]!=word:raise ValueError('Original dmo map alias changed')
        if string(0x802bf2cc)!='dmo_00' or string(0x802bf2d4)!='gor_01':raise ValueError('Demo map alias names changed')
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
    flags={0x4070:(1,'S'),0x2dac:(0,'S'),
           0x2dc4:(1,'S_ie_01_naka'),0x2ddc:(1,'S_ie_02_f1_naka'),0x2df4:(1,'S_ie_02_f2_naka'),
           0x2e0c:(1,'S_ie_03_naka'),0x2e24:(1,'S_ie_04_naka'),
           0x2e3c:(1,'S_gai_kabe_k'),0x2e54:(1,'S_gai_kabe_k_01'),
           0x34d4:(0,'S_gai_kabe_k'),0x34ec:(0,'S_gai_kabe_k_01')}
    for address,(action,name) in flags.items():
        c=commands[address];args=c['operands']
        if c['opcode']!=91 or args[0].get('annotation')!='evt_mapobj_flag_onoff' or [o['raw'] for o in (args[1],args[2],args[4])]!=[1,action,1] or args[3].get('identifier')!=name:raise ValueError('Demo visibility request changed')
    for address,name,kind,duration in [(0x3994,'crystal_n64',2,0),(0x39d4,'crystal_n64',6,60),(0x3a24,'radiation_n64',3,0)]:
        c=commands[address];args=c['operands']
        expected=[kind,4264967296,4264967297,4264967298,4064967808,duration,0,0,0,0,0,0]
        if c['opcode']!=91 or args[0].get('annotation')!='evt_eff64' or args[2].get('identifier')!=name or [o['raw'] for o in args[3:]]!=expected:raise ValueError('Demo effect request changed')
    for address,name in [(0x3a14,'eff_cam'),(0x3a64,'eff_cam1')]:
        if commands[address]['operands'][0].get('annotation')!=name:raise ValueError('Demo effect camera owner changed')
    if commands[0x3a1c]['opcode']!=10 or commands[0x3a1c]['operands'][0]['raw']!=500:raise ValueError('Radiation creation wait changed')
    if commands[0x3988]['opcode']!=53 or [o['raw'] for o in commands[0x3988]['operands']]!=[4264967297,10]:raise ValueError('Effect chest anchor height changed')
    return len(anchors)+2+5+3+5+len(flags)+len(alias)+2+7


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO)
    p.add_argument('--symbols',default='artifacts/reference/us_symbols.csv');a=p.parse_args()
    print(f'US demo392/button12/init63 commands and {verify(a.iso,a.symbols)} scene/guard/wait/float/tribe/visibility/alias anchors verified read-only')
if __name__=='__main__':main()
