#!/usr/bin/env python3
"""Read-only pins for the original mandatory system owner and its actual hooks."""
import argparse,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
SPANS=(
 ('definition',0x80416a80,48,'29a87c6292a7ee3751af6e947f4dff66c64ef31b94da486b0dce14686fddb290'),
 ('kind',0x803ade74,196,'6aa38ac4e5b3e6f97beb5c9ea97f66109ecf2a53e10c184d79f92584538f38ca'),
 ('part',0x803ade28,76,'f3a2f6405672e7ecfb977c1332f60bf33bd8f8f765ae19959da0ae62ab4bb811'),
 ('init',0x803adf38,84,'745f7c75dc66e032fd67c16eb246bf105f08ef22df793e3b1fd78fd28e02ecec'),
 ('hooks_and_lookahead',0x80419ef8,28,'eeaa8bdf4e0ba14a1e57311add0d899922deefc479d0424f3253f241f9b80dfc'),
)
def verify(iso):
 with Path(iso).open('rb') as disc:
  header=disc.read(0x440)
  if header[:8]!=b'G8ME01\0\0':raise ValueError('US disc0 revision0 required')
  dol=struct.unpack_from('>I',header,0x420)[0];disc.seek(dol);sections=disc.read(256)
  def read(address,size):
   for i in range(18):
    off,base,length=[struct.unpack_from('>I',sections,k+4*i)[0] for k in (0,0x48,0x90)]
    if base<=address and address+size<=base+length:
     disc.seek(dol+off+address-base);data=disc.read(size)
     if len(data)!=size:raise ValueError('Truncated system data')
     return data
   raise ValueError('Unmapped system data')
  data={}
  for name,address,size,digest in SPANS:
   data[name]=read(address,size)
   if hashlib.sha256(data[name]).hexdigest()!=digest:raise ValueError('Unexpected system '+name)
  word=lambda data,offset:struct.unpack_from('>I',data,offset)[0]
  if word(data['definition'],0)!=0x803ade74 or word(data['kind'],0)!=220 or struct.unpack_from('>HH',data['kind'],8)!=(999,0):raise ValueError('Unexpected system identity/resources')
  if data['kind'][0xb4]!=1 or word(data['kind'],0xb8)!=0x803ade28 or word(data['kind'],0xbc)!=0x803adf38:raise ValueError('Unexpected system part/init')
  if struct.unpack('>7I',data['hooks_and_lookahead'])!=(2,1,2,1,2,1,0x100):raise ValueError('Unexpected original hooks/lookahead')
 return dict(source='US G8ME01 revision0',spans=[dict(name=n,address=a,size=s,sha256=h) for n,a,s,h in SPANS],kind=220,hp=999,fp=0,lookahead_word=0x100,executed_original=False)
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);print(json.dumps(verify(p.parse_args().iso),indent=2))
