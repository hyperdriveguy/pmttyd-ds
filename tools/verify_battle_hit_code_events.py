#!/usr/bin/env python3
"""Verify complete original US hit-metadata query bodies, read-only."""
import hashlib
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO
SPANS={
 0x8010f2e8:(0x8c,'d320c8d8a16d0faaac3ef20aed3b1246d888bd569e8c56bfe752c3c335da616f'),
 0x8010f374:(0x70,'285e10cbcc9dde68d4c2df9908f4b7df796b6a032505adaa6af9c9de7018f31b'),
 0x8010f0e8:(0x8c,'71cb8012d6af5c56599063abf97d8a8c1643ad6131d3f3db06658dcfaef966f0'),
}
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
     if hashlib.sha256(disc.read(size)).hexdigest()!=digest:raise ValueError('Hit metadata body mismatch')
     break
   else:raise ValueError('Unmapped original source body')
 return {'bodies':len(SPANS),'scope':'Static original US evidence; no runtime trace'}
if __name__=='__main__':print(verify())
