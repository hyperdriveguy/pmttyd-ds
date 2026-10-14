#!/usr/bin/env python3
"""Verify complete original US horizontal camera shake bodies, read-only."""
import hashlib
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO
SPANS={2148709328: (240, '785067a7bcfe471d24d9806dc89f63af04afb980a47d1e7035bfd14d287423bb'), 2148709568: (96, '2689486718b939ab794bbcaaf0a41815f007ea541edfbfdeb839cae2be6f2771')}
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
     if hashlib.sha256(disc.read(size)).hexdigest()!=digest:raise ValueError('Camera shake body mismatch')
     break
   else:raise ValueError('Unmapped original source body')
 return {'bodies':len(SPANS),'scope':'Static original US evidence; no runtime trace'}
if __name__=='__main__':print(verify())
