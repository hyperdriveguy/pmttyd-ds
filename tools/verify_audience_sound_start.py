#!/usr/bin/env python3
"""Pin original positional slot provisioning and audience Sound1/2 wrappers."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
BODIES = [('__psndSFXOn', 2148374836, 1344, '3a3a312954b07a93a0468dd3edfc5f1d2110d9b92e36a8aff2a9d46caec22e71'), ('psndSFXOn', 2148374716, 60, 'eeb7479a31260ba74b0b7cee77816220a8f80919d3ffa05e3e0fb515652a8d73'), ('psndSFXOn_3D', 2148374600, 60, '29fdab1a516235c9bfbb11635067a2f53e115e54d41985990fbd5e09b6d1e971'), ('psndSFXOnEx_3D', 2148374536, 64, '9c8394b2c1bf9a5308b7c3520cfa6a4bc160f66b6e9b9f5f37371c320fe33015'), ('BattleAudienceSound1', 2149186796, 92, '55ff9d0c5738ce89e4fe57b47c5d27900dc099cbdeacef4d57a25ad3c7247b73'), ('BattleAudienceSound2', 2149186472, 64, '1f363503887d34c3c2614f7c60e1d40b68de80c440c26a9cfeae50574f704cd1'), ('BattleAudienceSoundGetInfo1', 2149186536, 260, '015372ad3e3459acf08bd9d5b0f3b753d0fa6c071bec2349fd8c23406de5580c'), ('BattleAudienceSoundGetInfo2', 2149186404, 68, '4f57673c8e0aaa3cf71f673b672fdbcf730d69663d59fe83c4b48505d6a49a73'), ('psndSFX_vol', 2148374480, 56, '9d478d41ed735650da5a1638dd94cea0326e697eb86a6d5dc7c4d44dd3e49e23'), ('psndSFXOff', 2148374008, 200, '98f5ff0126a346b87beb80bd7782bf707447ae48e4c1a0ef3663b018bed69300')]
SPANS = [(2150722344, 84840, '42a9025dfc22cc47ac95e67bfb2d2fb5f460c1a12ea93e067e793daaaff1f683'), (2150513760, 16, '6c1e5bd5fd6010fd447a298fb6c7e30190d7294583b11d4e671d5dea51b965ec'), (2151815372, 4, '2412b43a42f11c1cb3ef7956d3e293016e671a9061fbb76a4282c8b15139cc48'), (2151827848, 64, 'e55fde47393d310b6248421510fbb37e69f58f2d6e6c8a158e5e82eb479c022c'), (2150606196, 176, 'c7b0cd943d89622692f4a2e1050987724f53d0cfd414cdeff9a856d3f36b4455')]
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
 return dict(bodies=10,data_spans=5,scope='Original On and audience Sound1/2 bodies and linked original Off/volume bodies pinned; pending slot allocation is separate from backend dispatch/voice playback')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO)
 print(verify(p.parse_args().iso))
