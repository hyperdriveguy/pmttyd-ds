#!/usr/bin/env python3
"""Verify original Rogueport damage-return extraction and preserved sentinels."""
from pathlib import Path
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
if __name__=='__main__':main()
