#!/usr/bin/env python3
"""Verify complete original US status-window arbitration bodies, read-only."""
import hashlib
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO
SPANS={2148496660: (184, 'f8480d86b324df66a420f7f89bf775729bfe32591b87370c36290fc6f0763cd5'), 2148548148: (124, '6d48ef497be8d8e150e8c6d1dd162c894584aea7304f9dd4c3becc3f19e4d427'), 2148496844: (44, '1219e2623566834a2ce98fa5d3b8728d351167cba4f106986d847c47443e4f97'), 2148496932: (44, 'b615d36e0766e350b8e65ed67a1dda294cf22b2c6dfa22ea1e641227cb992fa7'), 2148496976: (44, '8f6d361123ef06e1225446824ef7e5fbd50094e449b4f1b10a4c2312ee3ba398'), 2148496888: (44, '6e154231c772a58be5aedf910d385c17783713ea0e19a54590e8f8288fa04581')}
def verify(iso=DEFAULT_ISO):
 with Path(iso).open('rb') as disc:
  header=disc.read(0x440)
  if header[:8]!=b'G8ME01\0\0':raise ValueError('Original US disc0 revision0 required')
  base=struct.unpack_from('>I',header,0x420)[0];disc.seek(base);sections=disc.read(256)
  for address,(size,digest) in SPANS.items():
   for i in range(18):
    off,start,length=[struct.unpack_from('>I',sections,k+4*i)[0] for k in (0,0x48,0x90)]
    if off and start<=address and address+size<=start+length:
     disc.seek(base+off+address-start)
     if hashlib.sha256(disc.read(size)).hexdigest()!=digest:raise ValueError('Status-window body mismatch')
     break
   else:raise ValueError('Unmapped original source body')
 return {'bodies':len(SPANS),'scope':'Static original US evidence; no runtime trace'}
if __name__=='__main__':print(verify())
