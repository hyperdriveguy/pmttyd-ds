#!/usr/bin/env python3
"""Read-only whole weapon-menu source pins and retained child-boundary census."""
import argparse
from pathlib import Path
import re
from iso_assets import DEFAULT_ISO
from verify_weapon_menu_main import verify as verify_main, ANCHORS
from verify_weapon_menu_domains import verify as verify_domains

def verify(iso=DEFAULT_ISO):
    result=verify_main(iso)
    domains=verify_domains(iso)
    calls={}
    for address,raw in ANCHORS.items():
        if not (0x80121cb0<=address<0x80121ce8 or 0x80121d0c<=address<0x80122068):
            continue
        word=int(raw,16)
        if word & 0xfc000003 == 0x48000001:
            relative=word & 0x03fffffc
            if relative & 0x02000000:
                relative-=0x04000000
            calls[address]=(address+relative)&0xffffffff
    calls.update({0x80122088:0x80123ec0,0x80122098:0x80124cb8,0x801220ac:0x80116070})
    implementation=(Path(__file__).resolve().parents[1]/'source/battle/battle_weapon_menu_continuation.cpp').read_text()
    retained={int(site,16):int(fn,16) for site,fn in re.findall(r'case (0x[0-9a-f]+):\s*\{\s*(?:auto result|const auto value)=c.call\((0x[0-9a-f]+)',implementation)}
    for site in re.findall(r'case (0x[0-9a-f]+):\s*\{\s*if\(!c.finishEnd\(\)\)',implementation):
        retained[int(site,16)]=0x80121cb0
    if 'c.pc=0x80121cc4;' not in implementation or 'c.call(0x8011600c,' not in implementation:
        raise ValueError('Whole End lost its original UI boundary')
    retained[0x80121cc4]=0x8011600c
    if retained!=calls:
        raise ValueError('Weapon parent original callsite boundary mismatch')
    result['retained_original_callsites']=len(retained)
    result['setup_domain_bodies']=domains['bodies']
    result['limitation']='Static source census; host contract checks are not executed-original/native cadence proof'
    return result

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
