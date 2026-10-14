#!/usr/bin/env python3
"""Pin original audience population, seat allocation, entry and local state bodies."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
BODIES = [('BattleAudienceSettingAudience', 2149212356, 2636, '21467d6044dd1773f586a7721fd0064ab1bfca57de5f8ba9495831c045cf2e8e'), ('BattleAudience_Entry', 2149200896, 464, 'a79390037fb66521479fcfcd921cee24de4523a32e63a452101d9d9f3885b5c1'), ('BattleAudience_Entry_Sub', 2149200296, 600, '42a7abe7a2285271e3bf1b68d710781e80efaa140b0a19b2137bc34038c6cff1'), ('BattleAudience_SetAnim', 2149200044, 172, '3d472703a8fb9353eb4d8bbc31d7fad3b52dd654c2400a640c660c4f073b5b92'), ('BattleAudience_ChangeStatus', 2149198372, 260, '0e20b6da0cc04b91b4941527003f2ba5df0edfea8ca56de2f7ca99a8d68bbf9e'), ('BattleAudience_Delete', 2149200216, 80, '565ac484eb86db6f07fc6856065d0a6f7e560ab11e0628225ec1b439845ff5e4'), ('BattleAudience_GetAnimEnd', 2149199972, 72, 'e453fb431fbb7441f52874f046e9958a33d28fc0531d9e248d0e35bf8d314914'), ('BattleAudience_GetAudienceNum', 2149196152, 40, 'c092cec20582e9516137553352d6375ead97d82ff6c18fdcd0537257b955e20d'), ('BattleAudience_NoUsedFCHaitiRand', 2149196192, 64, '7475443a317d26a2835c79004007bbde529e76ee7dd559d39f5ba312ab76a6a0'), ('unk_801a21e0', 2149196256, 512, '5d3cc8116607c6ec8b4acf4a0254c08fac4eb0d974e7e56cff4b08c4d31b3409'), ('unk_801a23e0', 2149196768, 512, '56104f97833edc6eebacfbda67c1e89c71fba0674579130ddbcb1638e8a0d20f'), ('BattleAudience_NoUsedHaitiRand', 2149197280, 244, '9e795d9c24fd88033a43dc19b3495b6969da663b53137816fedf18112b30bd9a'), ('BattleStageGetPtr', 2148767844, 16, 'a57bcf41694fc253d3277e8b7a18da97f062317ed8cbd4a78305fae4fad005de'), ('BattleAudienceBaseGetPtr', 2149222048, 12, '17096f3ef21a7a0336e1d70a08dc681fa1d734a641d0eef0da749a779d749f79'), ('BattleAudienceGetPtr', 2149221988, 60, 'b9357ba56748bd7cca833c66858c0d7c8552b2f84196298ab4e054eefcc73d56'), ('rand', 2150023972, 32, '6b19fd52fb70db058f43afa9f5b9f1922cdbd69d70b461055e6ba104576326b5'), ('irand', 2147507616, 68, '0523fae52d8e314f0e46dafe5c3f04f5c4c90ced7628011fdb0cf4efa4ba0daf')]
SPANS = [(0x8041c638,0x14e8,'27efa23577efb6deb3204baf406fdda61546d5700ec4c25f04e8b59fad724cc4'),(2150604460, 260, 'd9c8727be8860337f45b67ad87641ba086e34bb08d911065a4ef995847c92e1d'), (2150604720, 44, '9b140ea6c4e7562bce284f741cf74dad6ff5a31f8a0f9d421668f9b625eee73f'), (2150604976, 132, '12021aa3d9440fa8a2236587c9f924738c0ae9cf07b5b69c64088461cbf48078'), (2150605108, 240, '350895942356fed73f605e5ba14821bdcc8c56378e827ee10efdc416b9efeeb3'), (2151271296, 320, 'fe8b54315a18b956992f8565d3e01438e82f1eaec7ee6ce692af89b957b6a442'), (2151271816, 52, 'bf9f58344d809243fea2122ade338d985bc4e5749dfb4250ad77674c8623b5f7'), (2151370212, 512, 'ab001d0da86fd78add6643502a1edd111540b2ef03448a86bccf5f0157dd30f6'), (2151827848, 244, '177427ad24035605c75d096c1a58020e4142e15886dd821c814102ea518125fb')]
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
 return dict(bodies=17,data_spans=9,scope='Whole population/seat/entry/state local bodies and source data pinned; caller supplies original RNG and float conversion; active control/draw and canonical story audience admission remain separate')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO)
 print(verify(p.parse_args().iso))
