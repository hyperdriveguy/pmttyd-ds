#!/usr/bin/env python3
"""Pin the nine closed original battle cold-storage bodies and party scalar."""
import argparse, hashlib, struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
BODIES = {'0x800f7af0': {'size': 56, 'sha256': '1f7031158084d5ec9f57a3dc93b4bdddf84e22719d8816387fbf2917e3407723'}, '0x800f7c20': {'size': 132, 'sha256': '890163d04d56031a9ecd5e1edc8cca4e0702b011f8b92476afe0ed0314389e26'}, '0x800fa62c': {'size': 76, 'sha256': '656299e101396896e82d2a4479ae029ea5eb3015202f9b0273154cae9d90d3db'}, '0x8011a48c': {'size': 76, 'sha256': '989faa72399b2bf64d5a7d511dfd5b74e0a274948e742aa4c71532160a395626'}, '0x80129694': {'size': 76, 'sha256': '5c59fa8e9f9ae745600e470d15db21957747ba68e43e3bba6c817ba8e2770bf4'}, '0x80195a94': {'size': 92, 'sha256': '9c729556f9bc16f1121ed6ffd32b1cea366ce201e0d3c4ffa853cafb68423b61'}, '0x801aa228': {'size': 52, 'sha256': '459023ba635654a1ad9761d3290a6ff36cd5f2f7dfd722162e900b9091d95930'}, '0x8011a38c': {'size': 176, 'sha256': 'fe033e4e86f4a6df97ecf01b2f12d14b4b8e7b7694235c865f63b6bf1bcce958'}, '0x80216908': {'size': 4, 'sha256': 'f332ea5b5437103cbb6f1508679da89eec9288ad775c96c439a17fccabe3de8e'}, '0x80422174': {'size': 4, 'sha256': 'de620abd6d3615746360c1d15ce3a56291236d37424124054a49d25e947ffc4d'}}
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
   if hashlib.sha256(raw).hexdigest()!=pin['sha256']:raise ValueError('Original cold storage source changed: '+token)
 return {'whole_bodies':9,'party_scalar':1,'scope':'Storage and reset only; no native battle bootstrap completion claim'}
if __name__=='__main__':
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--iso',default=DEFAULT_ISO);print(verify(parser.parse_args().iso))
