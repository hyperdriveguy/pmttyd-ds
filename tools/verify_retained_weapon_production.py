#!/usr/bin/env python3
"""Read-only original US full weapon producer/evaluation source evidence."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_weapon_table_builder import verify as verify_table
from verify_weapon_menu_main import verify as verify_main
from verify_weapon_menu_domains import verify as verify_domains
CHECK_COST_SHA = '384f3b85454bd3a739440e4d352fa4869693a5eabf0e4e83a64e776c5bca27bb'

def verify(iso=DEFAULT_ISO):
 table=verify_table(iso);main=verify_main(iso);domains=verify_domains(iso)
 with Path(iso).open('rb') as f:
  if f.read(8)!=b'G8ME01\0\0':raise ValueError('Original US revision0 required')
  f.seek(0x420);base,=struct.unpack('>I',f.read(4));f.seek(base);h=f.read(256)
  found=[]
  for i in range(18):
   off,va,size=[struct.unpack_from('>I',h,x+4*i)[0] for x in (0,0x48,0x90)]
   if off and va<=0x801268fc and 0x80126968<=va+size:found.append(base+off+0x801268fc-va)
  if len(found)!=1:raise ValueError('Unowned CheckWeaponCost body')
  f.seek(found[0]);raw=f.read(0x6c)
  if hashlib.sha256(raw).hexdigest()!=CHECK_COST_SHA:raise ValueError('Whole original CheckWeaponCost changed')
  calls=[]
  for at in range(0,len(raw),4):
   word,=struct.unpack_from('>I',raw,at)
   if word&0xfc000003==0x48000001:
    displacement=word&0x3fffffc
    if displacement&0x2000000:displacement-=0x4000000
    calls.append((0x801268fc+at,(0x801268fc+at+displacement)&0xffffffff))
  if calls!=[(0x80126914,0x80126c64),(0x80126924,0x80126968),(0x80126938,0x800d3c44)]:raise ValueError('CheckWeaponCost child ordering changed')
 source=(Path(__file__).resolve().parents[1]/'source/battle/battle_weapon_evaluation_continuation.cpp').read_text()
 if source.index('c.sameSide=(c.word(c.w+0x68)&0x10000000)!=0;')>source.index('c.row=c.buffer+4+36*uint32_t(sb(c.byte(c.buffer+0xa6d+c.index)))'):raise ValueError('Decision lost source flag-before-permutation ordering')
 return {'table':table,'menu':main,'domains':domains,'extra_whole_check_cost':1,'scope':'Static original source pins; native child ownership/tests remain separate'}
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);print(verify(p.parse_args().iso))
