#!/usr/bin/env python3
"""Check original US BattleCheckCounter/init and its exact source result jump table."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
SPANS=[(0x800fa678,0x5a4,'1ee4c6b65d7e00737c68e4c4549b394b848436df9c7600ffe9bae4b5ed501655'),(0x800fac1c,0x3c,'168720f1783c1a64d56000c4ad4fa5c5576908604f66a779dde4a74e73675212'),(0x8034c720,0x30,'34df5950ea24c00ba00ee855fc63fcc675cadf001d21e00fd8aaf08c8e1c33eb'),(0x8012984c,0x84,'30a65fd2e5e8a79d8d6ba3e74f406abcb8417774370c053c81e71fefec786a2e'),(0x801298d0,0xc4,'7a75bb9a5b4510b0934b3b0ee1a05336cb8c9461aaaa49c7e20cb9a23d24d014')]
def verify(iso=DEFAULT_ISO):
 with Path(iso).open('rb') as disc:
  header=disc.read(0x440)
  if header[:8]!=b'G8ME01\0\0':raise ValueError('US disc0 revision0 required')
  dol=struct.unpack_from('>I',header,0x420)[0];disc.seek(dol);h=disc.read(256)
  for address,length,expected in SPANS:
   for i in range(18):
    off,base,size=[struct.unpack_from('>I',h,o+4*i)[0] for o in [0,0x48,0x90]]
    if base<=address and address+length<=base+size:
     disc.seek(dol+off+address-base);data=disc.read(length)
     if len(data)!=length or hashlib.sha256(data).hexdigest()!=expected:raise ValueError('Unexpected original span '+hex(address))
     break
   else:raise ValueError('Unmapped source function '+hex(address))
 print('Verified5 original counter/init/table/hit-event spans; executable native counter gate tests retain source ordering')
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);verify(p.parse_args().iso)
