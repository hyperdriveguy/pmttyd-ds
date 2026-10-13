#!/usr/bin/env python3
"""Check original NPC setup/fresh Fbat and field-to-battle information constructors."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
SPANS=[(2147761784, 640, '70a2ade8f21005d2d15f2b824dad9c77a78391ee0b8aa3b7ca02f52aa1d46624'), (2147793776, 204, 'ce07ec3389326b49ad38bb7630c3eed66d768327c46e54f2118a27a790e7a184'), (2148615448, 112, '2fb3223a113272ae55c72a5a64b0678e01ae862187616d382ec7f2092e6d9f2c'), (2147956908, 632, 'ea1bbc5bf6aef6b62b5ec39a508c0d05777f97aa8648389a01a2e71ceaf214de'), (2148503300, 312, '616f72f3468ba55fb0ac5dc234c5b950eb85fed4c8caafc20cca55bb73aa48c7'), (2148615440, 8, 'f1309f6b2cfb12dd91ec2f07ea4b087ebb9126181127fe34007d43ffcbfbcae7')]
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
 print('Verified6 original NPC/field/information/entry pointer constructor spans')
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);verify(p.parse_args().iso)
