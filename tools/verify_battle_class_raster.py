#!/usr/bin/env python3
"""Pin original circle/bubble consumers, templates, constants and resource path."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
BODIES = [('DrawMainIconCircle', 2148635048, 1184, '492c1248214b3a4d1107307593fed3197d44246470527fe3af48b619024a3b8f'), ('DrawMainIconHukidasi', 2148636232, 1140, '8f1e3eaec0b32e8066bdd06f442afa11458c9e654e52aad4661f7de8519e675b'), ('battleMenuDispInit', 2148638112, 88, '9ad746186b635d7f0fe7bcd0555f354625ddf80e48cc4e71c806612d2bcfd8e8')]
SPANS = [(2150561360, 128, 'f01b375772ce62a1261059cc3f8a4d609202c231baed4701232ef33ca734d53d'), (2150561488, 128, '627175a9c2f0d2482e56106bc41c2e1e29bdfefa690132624cb70ab8d7d9570b'), (2151818240, 320, 'b57480c701ba5c01ee6e2df0198409d0713c28b1acbde692d561698ffe554169'), (2150561808, 35, 'd33505f55b816b811acb85380f794464b525682906c7a6e8f176aa61c77d632e')]
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
 return dict(bodies=3,data_spans=4,scope='Whole circle/bubble local bodies and common resource constructor pinned; linked GX/SDK/icon/font services remain separate; static source, no GPU parity claim')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO)
 print(verify(p.parse_args().iso))
