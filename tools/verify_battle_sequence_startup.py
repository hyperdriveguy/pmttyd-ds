#!/usr/bin/env python3
"""Check original battle init, sequence ownership, admission and first-action bodies."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
SPANS=[(2148501920, 104, '00dd741e83176881d659d4201d6c68033d217de3a408dce9965d23774520ab60'), (2148502024, 96, 'c37da15d68a33b6a9bc520d6c77c96a476750cfbb01c9512d645c893f498479d'), (2148501852, 68, '6dfd37fed866d7feeb50f7439621e12efb16813a0565e596e8f8f87536da31eb'), (2148658752, 408, '1e2842e3f02393f304dee29a856d0be5bea2c2913c9bd9f95de6c65e6142b746'), (2148702612, 124, 'd97604ac736741579c207a0ee4bcd2d594ea5ba6bef36e41ca40843d9264957a'), (2148659692, 176, '8b0a2896f38e2aaddc49efb4fe2edfa6e4a8c7949a7dba0ba49d6bbef14072b1'), (2148814148, 128, 'a0f69a518a49c1d71013fed4ad1628f74f06244ab2ad059b50ba2ca8e748a302'), (2148656576, 2040, '58885b6803df68a6e807afd45fe5d04ec0ed1da0b5fbf40a03506cec9b304291'), (2151038464, 60, 'ac596864e8a2b03988b4dd8c148192fd5b7a9df9296252ab63c8bfd803b7f63d'), (2151818576, 4, 'e3820096cb82366b860b8a4e668453a7aaaf423af03bdf289fa308ea03a79332'), (2148502188, 760, '7629c39a04c07b612a1f7bf3d23e61a46bf187e422a4279a31f6d8eea9dc8844'), (2150942448, 48, '458fcc7c2d981646cfaa8a64088df7fddf09a58a863d6dbd782e9fbd23a1bb0f'), (2151817592, 8, 'd981d113b3b2d53a85d04ae72e5843a5cf7d9f0e29805fd3e782278ec3eb66aa'), (2148700128, 1716, '28c393e0dda25fa7af7e6c82f477f4067e7e5e7f6b50d09766996228814707f9'), (2151818912, 12, 'cb9524c28cd8740cb47df6426714df87d536175d3251ef3bda29c62f555183e8')]
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
 print('Verified15 original sequence/startup/admission/FirstAct/UnitSetup/Entry spans')
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);verify(p.parse_args().iso)
