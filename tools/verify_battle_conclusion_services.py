#!/usr/bin/env python3
"""Pin portable conclusion/barrier/end-selection source spans, ISO read-only."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
SPANS=[(0x8011aa34,0x180,'aeaf27e4689a224325f4ba55a69b83eaf9c6039b7b056a4f68f9bca4dce5d9f7'),(0x8011abb4,0x134,'efd6c35c40ea6ce07aa6981c3fdff8d3d6307eca109ecc7288d1798c2b9babc5'),(0x8011ace8,0x104,'7ac7aa532228ec3b1c77c4919e59bdde81928201a33654e77512faa75cbbe6b4'),(0x8021529c,0xac,'ec1a0b8683496d8181ae2af4396b516b5df9fe11527f44b1b34f8315184a287d'),(2148549784, 112, '90b1d68c0b2cd2bd19ce43d6bcaea60fe20d1dfb436232aad4e82b65803a739d'),(2149553752, 328, '5262827b862a315b47cf004a28f56dd70307c0d96bb69b203ce4c67e61f7e8d9')]
def verify(iso=DEFAULT_ISO):
 with Path(iso).open('rb') as disc:
  header=disc.read(0x440)
  if header[:6]!=b'G8ME01' or header[6:8]!=b'\0\0':raise ValueError('US disc0 revision0 required')
  dol=struct.unpack_from('>I',header,0x420)[0];disc.seek(dol);h=disc.read(256)
  for address,length,expected in SPANS:
   for i in range(18):
    off,base,size=[struct.unpack_from('>I',h,o+4*i)[0] for o in [0,0x48,0x90]]
    if base<=address and address+length<=base+size:
     disc.seek(dol+off+address-base);data=disc.read(length)
     if len(data)!=length or hashlib.sha256(data).hexdigest()!=expected:raise ValueError('Unexpected original function span '+hex(address))
     break
   else:raise ValueError('Unmapped source function '+hex(address))
 print('Verified6 source spans: conclusion, unit/outer event barriers, initial end selection, enemy-item wrapper/core')
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);verify(p.parse_args().iso)
