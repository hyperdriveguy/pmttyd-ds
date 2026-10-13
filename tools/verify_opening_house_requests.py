#!/usr/bin/env python3
"""Verify the bounded house presentation's source bindings read-only.

This is static US revision0 evidence, not an original-runtime timing trace.
"""
import argparse
from pathlib import Path
from iso_assets import DEFAULT_ISO, inventory
from rel_event_inventory import inventory_events


def verify(iso, symbols):
    info=inventory(iso)
    if info['disc_id']!='G8ME01':raise ValueError('US source required')
    entry=next(f for f in info['files'] if f['path']=='rel/aaa.rel')
    with Path(iso).open('rb') as source:
        source.seek(6)
        if source.read(2)!=bytes(2):raise ValueError('US disc0 revision0 required')
        source.seek(entry['offset']);raw=source.read(entry['size'])
    event=inventory_events(raw,symbols,'aaa',['evt_prologue'])['events'][0]
    if len(event['commands'])!=187 or event['sha256']!='833162e1181ca1e25faef38ecaaa34dfba08de0ad72c95cd6693f87a5461cf5f':
        raise ValueError('Original house event changed')
    by_address={c['address']:c for c in event['commands']}
    # Function name, resolved scalar operands. Relocated actor/string arguments
    # are checked separately below, preserving original author ownership.
    expected={
        0xdb8:('evt_cam3d_evt_set',[145,122,884,145,108,-180,0,11]),
        0xeb8:('evt_cam3d_evt_set',[145,122,717,145,108,-180,5000,11]),
        0xee8:('evt_npc_set_position',[500,500,0]),
        0x1008:('evt_npc_glide_position',[210,25,-30,0,80.,-30.,11,0]),
        0x1108:('evt_npc_glide_position',[0,300,-100,0,80.,-30.,11,0]),
        0x1134:('evt_npc_set_position',[20,30,-150]),
        0x11f0:('evt_npc_move_position',[63,-110,0,80.,1]),
        0x12a8:('evt_npc_move_position',[195,0,0,80.,1]),
        0x12c8:('evt_npc_move_position',[225,-75,0,80.,1]),
        0x14b0:('evt_npc_move_position',[25,-155,0,80.,1]),
        0x14ec:('evt_cam3d_evt_set',[-345,154,96,-32,75,-217,0,11]),
        0x1674:('evt_cam3d_evt_set',[-376,154,65,-63,75,-247,2000,11]),
        0x16ac:('evt_npc_move_position',[23,-156,0,80.,1]),
        0x16e0:('evt_mario_set_pos',[-155,30,-320]),
        0x16f4:('evt_mario_mov_pos2',[-125,-290,80.]),
        0x1708:('evt_mario_mov_pos2',[-85,-310,80.]),
        0x1738:('evt_npc_move_position',[-52,-275,0,80.,1]),
    }
    def signed(v):return v if v<0x80000000 else v-0x100000000
    def number(v):
        value=signed(v)
        return signed((v+230000000)&0xffffffff)/1024 if -250000000<value<=-210000000 else value
    for address,(name,values) in expected.items():
        c=by_address[address];args=c['operands']
        if c['opcode']!=91 or args[0].get('annotation')!=name:raise ValueError(f'Wrong source service at {address:x}')
        start=2 if name.startswith('evt_npc_') else 1
        got=[number(o['raw']) for o in args[start:]]
        if got!=values:raise ValueError(f'Wrong source operands at {address:x}: {got}')
    for address,duration in [(0xee0,4000),(0x1040,500),(0x10dc,500),(0x12e8,500),(0x1378,340),(0x1390,500),(0x14e4,500),(0x1630,500),(0x1768,500),(0x1788,300),(0x17cc,300)]:
        c=by_address[address]
        if c['opcode']!=10 or c['operands'][0]['raw']!=duration:raise ValueError('Source explicit wait changed')
    for address,name in [(0x13ac,'L_11'),(0x14a0,'L_6'),(0x1790,'L_8')]:
        c=by_address[address]
        if c['operands'][0].get('annotation')!='evt_npc_set_anim' or c['operands'][2].get('identifier')!=name:raise ValueError('Original source pose changed')
    return len(expected)+11+3


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--iso',default=DEFAULT_ISO)
    p.add_argument('--symbols',default='artifacts/reference/us_symbols.csv')
    a=p.parse_args();count=verify(a.iso,a.symbols)
    print(f'US original house event187 commands/hash and {count} request/wait/pose anchors verified read-only')
if __name__=='__main__':main()
