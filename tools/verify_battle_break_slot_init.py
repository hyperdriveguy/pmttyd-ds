#!/usr/bin/env python3
"""Pin whole BreakSlotInit, its two pointer helpers and original category tables."""
import argparse, hashlib, struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
BODIES = {'0x8020507c': {'size': 404, 'sha256': '0dc91abdce67abf87e0c2808852f078071579933c644d3e3723edd7cd505848f'}, '0x80205210': {'size': 60, 'sha256': '6dda611c2beb1de841f2a7d37826725c1a9b082f4a09f5008e221010e66d8a92'}, '0x8020524c': {'size': 16, 'sha256': '78c442cd3a8d835c51e54e160ee702bd26547edbe06c69fad9a80202fcc65231'}, '0x802fe040': {'size': 84, 'sha256': 'e9a12052128059ced9b2ef3a73329b1768a01026408ff98b20ea73023917f1fb'}}
def verify(iso=DEFAULT_ISO):
 with Path(iso).open('rb') as disc:
  if disc.read(8)!=b'G8ME01\0\0':raise ValueError('Original US revision0 disc required')
  disc.seek(0x420);dol,=struct.unpack('>I',disc.read(4));disc.seek(dol);header=disc.read(256)
  for token,pin in BODIES.items():
   address=int(token,16);locations=[]
   for i in range(18):
    off,base,size=[struct.unpack_from('>I',header,t+4*i)[0] for t in (0,0x48,0x90)]
    if base<=address and address+pin['size']<=base+size:locations.append(dol+off+address-base)
   if len(locations)!=1:raise ValueError('Unmapped or ambiguous original span')
   disc.seek(locations[0]);raw=disc.read(pin['size'])
   if hashlib.sha256(raw).hexdigest()!=pin['sha256']:raise ValueError('Original break-slot source changed: '+token)
 return {'whole_init':1,'linked_pointer_helpers':2,'table_words':21,'scope':'Retained initialization; no active slot game or battle bootstrap claim'}
if __name__=='__main__':
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--iso',default=DEFAULT_ISO);print(verify(parser.parse_args().iso))
