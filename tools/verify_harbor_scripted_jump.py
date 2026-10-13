#!/usr/bin/env python3
"""Verify the scoped stationary harbor jump's arithmetic in the read-only US DOL."""
import argparse,hashlib,struct
from iso_assets import DEFAULT_ISO

def verify(iso):
 with open(iso,'rb') as f:
  if f.read(8)!=b'G8ME01\0\0':raise ValueError('US disc0 revision0 required')
  f.seek(0x420);dol=struct.unpack('>I',f.read(4))[0];f.seek(dol);header=f.read(0x100)
  def read(a,n):
   for i in range(18):
    off=struct.unpack_from('>I',header,i*4)[0];base=struct.unpack_from('>I',header,0x48+i*4)[0];size=struct.unpack_from('>I',header,0x90+i*4)[0]
    if base and base<=a and a+n<=base+size:f.seek(dol+off+a-base);return f.read(n)
   raise ValueError('Unmapped DOL range')
  constants={0x80421b00:'c47a0000',0x80421afc:'41a00000',0x80421af8:'3f000000',0x80421ad8:'3f800000',0x80421adc:'00000000'}
  for a,expected in constants.items():
   if read(a,4).hex()!=expected:raise ValueError('Jump constant changed')
  anchors={0x800e4d10:0x809c0014,0x800e4d24:0xc002a6c0,0x800e4d34:0xc342a6bc,
   0x800e4dd0:0xc042a6b8,0x800e4ddc:0xefe20772,0x800e4df4:0xec3ffff8,
   0x800e4df8:0xec220824,0x800e4e00:0xfc200850,0x800e4ed0:0xec00f82a,
   0x800e4f2c:0xec01002a,0x800e4f38:0x807e0080,0x800e4f44:0x418101bc,
   0x800e4f64:0xec252978,0x800e4f78:0xd03f0080,0x800e509c:0xc01f007c,
   0x800e50a0:0xec01002a,0x800e50fc:0x48000008,0x800e50f8:0x38600001,
   0x80094068:0xec01002a,0x8009407c:0xec01002a}
  for a,expected in anchors.items():
   if struct.unpack('>I',read(a,4))[0]!=expected:raise ValueError(f'Jump instruction changed: {a:08x}')
  print('stationary jump:',len(anchors),'source instructions,',len(constants),'constants; body SHA256',hashlib.sha256(read(0x800e4c54,0x500)).hexdigest())
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);verify(p.parse_args().iso)
