#!/usr/bin/env python3
"""Read-only complete US GetCoin/StoreCoin/unit getter/registry witnesses."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
PINS=[(2148587796, 108, '98fb58310fc71245e9fd614ba24c83d5635aa4984abdb75350c8cf7ef6610929'), (2148691112, 120, 'ed708e847f5c7567136163676b555615cb26af21d2d1cf24212ad0754e8a801f'), (2148501708, 60, '6a40e068a63873b3f93f3cf0bdc72c59323e60ff5b961ac740b22eddb58276fa'), (2148587736, 60, 'ad50da3fb0de26f23215aa750c01b353d2ff94d0d628fb7635c189f48aa0f8d6'), (2148498444, 20, 'b52af599a9d93c003e3b0d6abddd4ef2b408dce3d0f306e88e2d98dfb86515c3')]
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
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);a=p.parse_args();print('Verified',verify(a.iso),'complete original bodies')
