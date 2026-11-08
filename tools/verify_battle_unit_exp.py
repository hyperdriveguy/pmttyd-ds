#!/usr/bin/env python3
"""Read-only complete US GetExp/StoreExp/pouch/registry bodies and numeric data witnesses."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
PINS=[(2148587964, 108, '9698c0d716e767b0a62328358489f73cbe7561872880ae6aa8e5f5ca446823e4'), (2148691232, 224, 'd0835f1048008c1faf77deec1dd0e6730354471a8cd09851e2222afa3db7ba3c'), (2148587904, 60, '693a63bb614594541cfc8947441ffe93a52dc979013a0fb16fa4523bb3bef3ee'), (2148498428, 16, '2645bdd2d013d25c1e2d5c3ea349d499846bac05d9bb28e5e33cb1437674ef69'), (2148501708, 60, '6a40e068a63873b3f93f3cf0bdc72c59323e60ff5b961ac740b22eddb58276fa'), (2148359856, 8, 'dbcd2f415efe4aa4e5684ee546cf73d97cc641b5a0290f1b46b14ab606786537'), (2150564024, 8, '07af8387624cf8f36152797d2f561246f21237d1d22b5c13006eacfa6f475e43'), (2151818920, 28, '1bc10a840183cb7081b90c4350d6416d00ee8dc95a4f70517bc367230913dbbc')]
def verify(iso):
 with Path(iso).open('rb') as disc:
  if disc.read(8)!=b'G8ME01\0\0':raise ValueError('US disc0 revision0 required')
  disc.seek(0x420);dol,=struct.unpack('>I',disc.read(4));disc.seek(dol);header=disc.read(256)
  sections=[tuple(struct.unpack_from('>I',header,k+4*i)[0]for k in(0,0x48,0x90))for i in range(18)]
  for address,size,digest in PINS:
   offsets=[dol+o+address-b for o,b,n in sections if b<=address and address+size<=b+n]
   if len(offsets)!=1:raise ValueError('Source mapping differs')
   disc.seek(offsets[0]);data=disc.read(size)
   if len(data)!=size or hashlib.sha256(data).hexdigest()!=digest:raise ValueError('Original source body differs: '+hex(address))
 return len(PINS)
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);a=p.parse_args();print('Verified',verify(a.iso),'original complete bodies/data blocks')
