#!/usr/bin/env python3
"""Check original fresh pouchInit, equipped badge scan, pointer and zero constant."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
SPANS=[(0x800d59c8,0x4e8,'c6e29f88e574c1d5caa1aaf5c7a73a53bc2d952cbad00a9e21ab2bfd3113def1'),(0x800d3300,0xc8,'2800af1700923fd4deabfe11a78b7b85163bb9be22c7fdab73bf456b29febec1'),(0x800d5eb0,8,'dbcd2f415efe4aa4e5684ee546cf73d97cc641b5a0290f1b46b14ab606786537'),(0x8042187c,4,'df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119')]
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
 print('Verified4 original fresh pouch/init/equipped badge/zero spans')
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);verify(p.parse_args().iso)
