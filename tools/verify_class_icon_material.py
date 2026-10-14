#!/usr/bin/env python3
"""Pin original disabled-icon TEV RGB average, coefficient and record alpha."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
BODIES = [('iconGX', 2147588088, 2468, 'bb9430d4030d22982f813be3bc530403e5a1fd8c24c1ddd9298ff94b1e205848')]
SPANS = [(2151807084, 4, 'e511c2e4139ff89a1cbe0ba1bb015c8bb1f42cae995dda76faffe224827beb74')]
def verify(iso=DEFAULT_ISO):
 with Path(iso).open('rb') as disc:
  if disc.read(8)!=b'G8ME01\0\0':raise ValueError('US revision0 disc required')
  disc.seek(0x420);dol=struct.unpack('>I',disc.read(4))[0];disc.seek(dol);h=disc.read(256)
  def read(a,n):
   matches=[]
   for i in range(18):
    off,base,size=[struct.unpack_from('>I',h,k+4*i)[0] for k in (0,0x48,0x90)]
    if base<=a and a+n<=base+size:matches.append(dol+off+a-base)
   if len(matches)!=1:raise ValueError('Unmapped/ambiguous source address')
   disc.seek(matches[0]);b=disc.read(n)
   if len(b)!=n:raise ValueError('Truncated source span')
   return b
  for name,a,n,digest in BODIES:
   if hashlib.sha256(read(a,n)).hexdigest()!=digest:raise ValueError('Body mismatch: '+name)
  for a,n,digest in SPANS:
   if hashlib.sha256(read(a,n)).hexdigest()!=digest:raise ValueError(f'Data mismatch: {a:#x}')
  if read(0x80005418,8).hex()!='3c40804260427440':raise ValueError('r2 base mismatch')
 return dict(bodies=1,data_spans=1,scope='Original flag32 material branch pinned within containing iconGX; other iconGX domains and native raster bit equality remain separate')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO)
 print(verify(p.parse_args().iso))
