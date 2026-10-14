#!/usr/bin/env python3
"""Pin original ring-center/offset bodies and immutable source scalar constants."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
BODIES = [('GetRingCenter', 2148637936, 132, '0e47a302109352b583545d99ac17bf462e68f2bf9b97a07c796b7b2f661d95e5'), ('GetRingOffset', 2148637820, 116, 'e3001c2f1623c03fe887d4cb9c6d11722cc354f580c03d0d3fa0f9af949c4b6c'), ('BattleGetUnitPtr', 2148501708, 60, '6a40e068a63873b3f93f3cf0bdc72c59323e60ff5b961ac740b22eddb58276fa')]
SPANS = [(2151818560, 16, '193fd1877df54ee6bb554d80260b8969342ca4163cd3949a57d9e17b2a977f7d'), (2151818544, 4, '2b2149a3e7e9e2dab48aca85f843233452d08c2b97b15f8f8402aa38caf8f7eb'), (2151818500, 4, '25daa80c2db5345b78a217e02b173cf202d3d9fe141e44c7a04828512fe28db4'), (2151818284, 4, 'df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119')]
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
 return dict(bodies=3,data_spans=4,scope='Complete local ring center/offset bodies; projection/libm remain linked providers; static source, no original camera or GPU parity claim')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO)
 print(verify(p.parse_args().iso))
