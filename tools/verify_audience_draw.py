#!/usr/bin/env python3
"""Pin whole original audience draw/front, templates and linked SDK/libm bodies."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
BODIES = [('BattleAudienceDispAudience', 2149202168, 2348, 'dbe38033161ab320e7cbc48832b98a3d4f5cc4b4b6df55b4b04b2ea7deec8d69'), ('BattleAudience_GetFront', 2149198632, 152, '67e1a975e327bf037ba8fc9d2d9e934a55036a934d8e4809b18b0c9b89622fc7'), ('BattleAudienceBaseGetPtr', 2149222048, 12, '17096f3ef21a7a0336e1d70a08dc681fa1d734a641d0eef0da749a779d749f79'), ('BattleAudienceGetPtr', 2149221988, 60, 'b9357ba56748bd7cca833c66858c0d7c8552b2f84196298ab4e054eefcc73d56'), ('PSMTXScale', 2150228656, 40, '535aca41411c853f3a84d4d1d60c5d2a9e66e998103b03c8125cbdc091a6e143'), ('PSMTXMultVec', 2150229668, 84, '0e86b49668bd9172bfd1256aa2842a4326032efab48cb6b8df58523a63992e78'), ('PSMTXConcat', 2150227300, 204, 'de50e27eaa9d4816ce7462637664e423cba8f7a85196f965a830331828e2ad37'), ('sin', 2150048784, 216, 'c2695b37728c8ff697ab4a1075075424e27ae94fdfb10de799ec45896b085ac2'), ('cos', 2150047400, 212, '8c4ecab8366997f2b3cb5cdeb5125218365ab1a56f64cc9b4e3de176ee181ca2')]
SPANS = [(2150604824, 140, 'bfe3c173c632839091365e3c8e83c4bc8888d9c226ff137f7d7fef30266df481'), (2151370212, 512, 'ab001d0da86fd78add6643502a1edd111540b2ef03448a86bccf5f0157dd30f6'), (2151827848, 4, 'df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119'), (2151827968, 44, 'f8535d1b4944bacb9b5e187875c88998e3e08783f31a55b22d5325d39b0d059b')]
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
 return dict(bodies=9,data_spans=4,scope='Whole audience draw/front and source templates pinned; native SDK/libm and world-camera numerical adapters do not assert original GPU parity')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO)
 print(verify(p.parse_args().iso))
