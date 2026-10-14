#!/usr/bin/env python3
"""Pin original class footer bodies, audience item producer and source data."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
BODIES = [('DrawMenuPartyChangeButton', 2148631208, 468, 'e9069e28ba84b25cdcc9ead2fada6ff583b8bfeac7328c312db7d8eb57211144'), ('DrawMenuPartyChangeButton_Sub', 2148631056, 152, '47206fba8164b9c50591380fba9c66ae6c10d1ded6235300d6e7766ad6a14049'), ('DrawMenuPinchMark', 2148630000, 88, '0c74933ea5b252a28eadc6509c70408954f0ca825a48a62a8ace37f00081ae57'), ('N_DrawMenuMarioPinchMark', 2148630576, 480, '0eaa0da1055fe4006e8f53b267997f568bb18be7692a05a76bc6643e0505ddd4'), ('N_DrawMenuPartyPinchMark', 2148630088, 488, '4a12b554aacccdd35891a0c9504fc82cdc334fce44292eac57290926d8a02b21'), ('BattleAudience_GetItemOn', 2149199192, 336, 'c837d7ee64166bb5a9721c3b5b59f609bfcbc9e8856c7046cd7670317268561a'), ('BattleAudienceBaseGetPtr', 2149222048, 12, '17096f3ef21a7a0336e1d70a08dc681fa1d734a641d0eef0da749a779d749f79'), ('BattleAudienceGetPtr', 2149221988, 60, 'b9357ba56748bd7cca833c66858c0d7c8552b2f84196298ab4e054eefcc73d56'), ('BattleAudienceItemGetPtr', 2149221924, 64, '7761387b8d9f71a7e88dbc500f1461f49ecafd0c8ad71aaa24b4ea43cb4a0213'), ('btlDispGXInit2D', 2148547092, 308, 'a815d669d03bfa71bfa8da8173cbaa77f022a9263fc0422af71b7a3ac00d3ae2')]
SPANS = [(2151771428, 24, 'c4d2a9b5b2c82b1394ef24a99cd2f0abd4ef340ee86a0485ca8fe6aa9a986ddc'), (2151818276, 160, 'b59bc6f204e717fb5189af740d4f2369fda05c9b9c7ecf1a8078b8174328ca99'), (2151827916, 4, '638354a0b185ec05318029e52610c8de48d33523e2b4854e71154187583d1037'), (2150561700, 32, '9f0a7e270435926c2de86a7e0196b1bda521701b3cb4aaf6e2d0254d21f4fdf7')]
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
 return dict(bodies=10,data_spans=4,scope='Five complete footer bodies and four audience query/getter bodies; Init2D linked/finite backend state binding; projection/texture/sound/GX parity remains separate')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO)
 print(verify(p.parse_args().iso))
