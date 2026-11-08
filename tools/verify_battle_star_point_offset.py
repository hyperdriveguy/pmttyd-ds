#!/usr/bin/env python3
"""Read-only complete US star-point offset/registry bodies and signed-conversion data witnesses."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
PINS=[(2148614024, 372, 'c2081a004d7fce49b00d3193bbdff49880953e3c44f93d52cd2ddda57d4e71a3'), (2148501708, 60, '6a40e068a63873b3f93f3cf0bdc72c59323e60ff5b961ac740b22eddb58276fa'), (2150558264, 8, '07af8387624cf8f36152797d2f561246f21237d1d22b5c13006eacfa6f475e43')]
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
