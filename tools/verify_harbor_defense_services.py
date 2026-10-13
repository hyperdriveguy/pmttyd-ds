#!/usr/bin/env python3
"""Check original US tutorial defense wrapper, core and private-pad reset."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
SPANS=[(0x80107694,0x74,'5a1de272dc6f1096c393122e008047fe300d925f7630c2feb34817c453e5543d'),(0x800fa1b8,0x268,'bcc6bae48919416891fafdebf90493fd65df36b2c0e785fa0c539429f1149336'),(0x8011a38c,0xb0,'fe033e4e86f4a6df97ecf01b2f12d14b4b8e7b7694235c865f63b6bf1bcce958')]
def verify(iso=DEFAULT_ISO):
 with Path(iso).open('rb') as disc:
  header=disc.read(0x440)
  if header[:8]!=b'G8ME01\0\0':raise ValueError('US disc0 revision0 required')
  dol=struct.unpack_from('>I',header,0x420)[0];disc.seek(dol);h=disc.read(256)
  for address,length,expected in SPANS:
   for i in range(18):
    off,base,size=[struct.unpack_from('>I',h,o+4*i)[0] for o in [0,0x48,0x90]]
    if base<=address and address+length<=base+size:
     disc.seek(dol+off+address-base);data=disc.read(length)
     if len(data)!=length or hashlib.sha256(data).hexdigest()!=expected:raise ValueError('Unexpected original span '+hex(address))
     break
   else:raise ValueError('Unmapped source function '+hex(address))
 print('Verified3 original defense/reset spans; producer cadence pinned separately by verify_input_cadence_contracts.py')
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);verify(p.parse_args().iso)
