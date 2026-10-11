#!/usr/bin/env python3
"""Verify original Rogueport damage-return extraction and preserved sentinels."""
from pathlib import Path
import struct
from room_convert import damage_returns
ROOT=Path(__file__).resolve().parents[1]
def main():
    tables=damage_returns(ROOT/'local-assets/gor.rel')
    assert [(t['call_offset'],t['table_offset'],len(t['entries'])) for t in tables]==[(0x2f364,0x2c594,5),(0x4d6d0,0x4c9d4,12)]
    harbor={e['node']:e for e in tables[0]['entries']}
    assert harbor['A_yuka_03']['position']==[84,15,220]
    assert harbor['A_yuka_04']['position']==[475,15,-180]
    assert harbor['A_yuka']['automatic_position']
    assert harbor['Ship_Change01']['position']==[-550,15,0]
    assert sum(e['automatic_position'] for t in tables for e in t['entries'])==5
    print('Two original init calls; 17 ordered damage-return entries; 5 automatic-position sentinels retained')
    packed={}
    for room,count in [('gor_00',5),('gor_02',16)]:
        raw=(ROOT/'romfs'/f'{room}.returns').read_bytes()
        assert struct.unpack_from('<4sII',raw)==(b'RPOS',1,count) and len(raw)==12+count*16
        bound={}
        for n in range(count):
            node,x,y,z=struct.unpack_from('<I3f',raw,12+n*16);assert node not in bound;bound[node]=[x,y,z]
        packed[room]=bound
    for node,point in [(0xd1958,[.84,.15,2.2]),(0xd19c0,[4.75,.15,-1.8]),(0xd1888,[.85,.15,2.2])]:
        assert all(abs(a-b)<1e-5 for a,b in zip(packed['gor_00'][node],point))
    print('21 native hierarchy bindings; literal engine-unit conversion and original harbor automatic center verified')
if __name__=='__main__':main()
