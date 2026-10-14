#!/usr/bin/env python3
"""Pin complete original US TimingA display callbacks and unchanged A icons."""
import argparse
import hashlib
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
BODIES=[
 (0x8012a6b0,0x110,'96b9a5b1281a4dc4a87773a3e3381cacac231bbb61d9c159c49224860e392254'),
 (0x8012a7cc,0xf8,'ec432739a4ddb6978f6c28527f56036997531c46a297ebf57833d60016b2dd8b'),
 (0x8001a918,0x24,'140c49735d09568fc933f8eca36b6fc8788f0878ac8b5ecf60f19e3079b3041d'),
]
def verify(iso=DEFAULT_ISO):
 info=inventory(iso)
 with Path(iso).open('rb') as disc:
  if disc.read(8)!=b'G8ME01\0\0':raise ValueError('US disc0 revision0 required')
  disc.seek(0x420);dol=struct.unpack('>I',disc.read(4))[0];disc.seek(dol);h=disc.read(256)
  def read(a,n):
   matches=[]
   for i in range(18):
    off,base,size=[struct.unpack_from('>I',h,o+4*i)[0] for o in [0,0x48,0x90]]
    if base<=a and a+n<=base+size:matches.append(dol+off+a-base)
   if len(matches)!=1:raise ValueError('Unmapped/ambiguous original address')
   disc.seek(matches[0]);b=disc.read(n)
   if len(b)!=n:raise ValueError('Truncated original span')
   return b
  for a,n,digest in BODIES:
   if hashlib.sha256(read(a,n)).hexdigest()!=digest:raise ValueError(f'Display body mismatch {a:#x}')
  if read(0x80005418,8).hex()!='3c40804260427440':raise ValueError('Original r2 base mismatch')
  if read(0x80422710,20)!=struct.pack('>5f',-200,50,1,-300,30):raise ValueError('Display constants mismatch')
  if read(0x802f01c8,24)!=bytes(24):raise ValueError('Original position templates mismatch')
  resource=next(f for f in info['files'] if f['path']=='icon.bin')
  disc.seek(resource['offset']);icons=disc.read(resource['size'])
  for icon,offset in [(108,2442),(109,2454)]:
   if struct.unpack_from('>H',icons,2*(icon+1))[0]!=offset:raise ValueError('Icon offset mismatch')
   if icons[offset:offset+12]!=struct.pack('>6H',0,0,0,1,icon,0):raise ValueError('Icon descriptor/frame mismatch')
 return dict(bodies=3,constants=5,templates=2,icons=[108,109],scope='Static source; no original runtime measurement')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO)
 print(verify(p.parse_args().iso))
