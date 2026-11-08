#!/usr/bin/env python3
"""Read-only US pins for the Harbor source command composition.

Existing retained Input/Act/Weapon children have their own source verifiers.
No original ISO bytes are exported.
"""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
BODIES = {2148667364: {'size': 88, 'sha256': '75f25b701f26dfec05058ab44c4550d84f6de752a13ea55ce15c409850e392dd', 'calls': [['0x80120ffc', '0x800f88cc'], ['0x80121010', '0x80128c98'], ['0x8012101c', '0x80128cd8'], ['0x80121024', '0x801276ac']]}, 2148665832: {'size': 92, 'sha256': '529a57b70ff0cd00d3d566f18ad47be483b0093015b2d8b403096d76ab4e84ef', 'calls': [['0x80120a08', '0x80124cb8'], ['0x80120a14', '0x8012c028']]}, 2148625624: {'size': 4, 'sha256': 'f332ea5b5437103cbb6f1508679da89eec9288ad775c96c439a17fccabe3de8e', 'calls': []}, 2148622444: {'size': 4, 'sha256': 'f332ea5b5437103cbb6f1508679da89eec9288ad775c96c439a17fccabe3de8e', 'calls': []}, 2148764040: {'size': 4, 'sha256': 'f332ea5b5437103cbb6f1508679da89eec9288ad775c96c439a17fccabe3de8e', 'calls': []}, 2148764044: {'size': 4, 'sha256': 'f332ea5b5437103cbb6f1508679da89eec9288ad775c96c439a17fccabe3de8e', 'calls': []}, 2148764048: {'size': 4, 'sha256': 'f332ea5b5437103cbb6f1508679da89eec9288ad775c96c439a17fccabe3de8e', 'calls': []}, 2148764052: {'size': 4, 'sha256': 'f332ea5b5437103cbb6f1508679da89eec9288ad775c96c439a17fccabe3de8e', 'calls': []}, 2148764056: {'size': 4, 'sha256': 'f332ea5b5437103cbb6f1508679da89eec9288ad775c96c439a17fccabe3de8e', 'calls': []}, 2148764060: {'size': 4, 'sha256': 'f332ea5b5437103cbb6f1508679da89eec9288ad775c96c439a17fccabe3de8e', 'calls': []}}
def verify(iso=DEFAULT_ISO):
 with Path(iso).open('rb') as f:
  if f.read(8)!=b'G8ME01\0\0':raise ValueError('Original US revision0 required')
  f.seek(0x420);base,=struct.unpack('>I',f.read(4));f.seek(base);h=f.read(256)
  def read(a,n):
   offsets=[]
   for i in range(18):
    off,va,size=[struct.unpack_from('>I',h,field+4*i)[0] for field in (0,0x48,0x90)]
    if off and va<=a and a+n<=va+size:offsets.append(base+off+a-va)
   if len(offsets)!=1:raise ValueError('Unowned/ambiguous initialized original span')
   f.seek(offsets[0]);raw=f.read(n)
   if len(raw)!=n:raise ValueError('Truncated original span')
   return raw
  for a,expected in BODIES.items():
   raw=read(a,expected['size']);calls=[]
   for offset in range(0,len(raw),4):
    w,=struct.unpack_from('>I',raw,offset)
    if w&0xfc000003==0x48000001:
     d=w&0x3fffffc
     if d&0x2000000:d-=0x4000000
     calls.append([hex(a+offset),hex((a+offset+d)&0xffffffff)])
   if hashlib.sha256(raw).hexdigest()!=expected['sha256'] or calls!=expected['calls']:raise ValueError('Original whole body changed '+hex(a))
  return {'whole_bodies':len(BODIES),'data_spans':0,'scope':'Original whole target Setup/End and complete blr leaves; no normal scheduler/presentation admission claim'}
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);print(verify(p.parse_args().iso))
