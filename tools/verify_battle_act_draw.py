#!/usr/bin/env python3
"""Pin original class ring lifecycle, icon routing and linked position helpers."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
BODIES = [('DrawMainMenu', 2148622564, 2964, 'e1d48badb45e4e2bd97cc8a79e75136a784f11de0b8061172f713dcd8453e604'), ('DrawMainIcon', 2148637372, 448, '1e8888a2416cbdc6b5436972cbadd882f6e7a632c34ee56e076c38f8592da715'), ('GetRingCenter', 2148637936, 132, '0e47a302109352b583545d99ac17bf462e68f2bf9b97a07c796b7b2f661d95e5'), ('GetRingOffset', 2148637820, 116, 'e3001c2f1623c03fe887d4cb9c6d11722cc354f580c03d0d3fa0f9af949c4b6c')]
SPANS = [(2151036376, 60, '034a6f967e09d4e7dd11ba4fb28d611dacdfefb020a3c87fc020a1c1d17f1c5b'), (2151036436, 48, 'ae54820a35b500b3305409a901b01580ee009d56737cf09eea5400e06cc1078c'), (2151818284, 52, '7b2d378dc082fc4a920b2e7c8326ea0eef03a5593a31eeab2b33b4c52f305a07')]
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
 return dict(bodies=4,data_spans=3,scope='Two complete local draw bodies; center/offset identities linked; static source, no original execution or GPU parity claim')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO)
 print(verify(p.parse_args().iso))
