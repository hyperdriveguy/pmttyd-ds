#!/usr/bin/env python3
"""Read-only source seeds/waits/interpolation/heading branch for used muj20 opening."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
from rel_event_inventory import inventory_events
HASHES={'muj_20_init_evt':'131ac2bc41c846b6d8ea0c57d026eccde545dd4d80927307109c0bf4f03b31b2','evt_prologue':'f3ba8a3667a705bc6875d19ade724d0dbb934b47a466f2642c660c7f7d3f6024','anm_kaji':'ab40a48a12e55db7bf44dda0af91d2c0d2e4bf0094531091cfdc8de8ddf8fc97','anm_wakka':'8e51b6d57826701f54931eb4b5de3a2054c7c5180ea74df12c2d22204d9515d8','anm_entotsu':'a20837790f1c5c3e33f0926c18975649989d552345105313c4a97bfbcd2daf29','anm_neji':'b7589f48361ba47682e878e2b057bd475fd37591acdc32b17e5678f1ec91d0a2','anm_yure':'85e56968dc13a7a357ab3a45b13b8f96f95e94debf2c87df5ae790303c3c1b9f'}
def verify(iso,symbols):
 e=next(f for f in inventory(iso)['files'] if f['path']=='rel/muj.rel')
 with Path(iso).open('rb') as s:s.seek(e['offset']);raw=s.read(e['size'])
 data=inventory_events(raw,symbols,'muj',list(HASHES));calls={}
 for event in data['events']:
  if event['sha256']!=HASHES[event['name']]:raise ValueError('Original voyage event changed')
  calls.update((c['address'],c) for c in event['commands'])
 checks=[(0x34674,52,[0xfd050f80,0xf22b3a80]),(0x34680,52,[0xfd050f81,0xf24a7a80]),(0x3468c,52,[0xfd050f82,0xf21b9a80]),(0x33ab8,10,[500]),(0x33ae0,10,[3500]),(0x33b34,91,['evt_sub_intpl_msec_init',11,0xfffff8f8,0,10000]),(0x33b98,9,[1]),(0x33bb8,50,[0xfaa2b580,1]),(0x33bc8,5,[5]),(0x33bd0,10,[1000]),(0x33c50,10,[4000]),(0x33c58,91,['evt_snd_bgmoff_f',512,6000]),(0x33c78,91,['evt_fade_set_mapchange_type',1,17,800,16,800]),(0x33cdc,91,['evt_sub_intpl_msec_init',11,0,1800,10000]),(0x338e0,10,[1500])]
 for address,opcode,expected in checks:
  c=calls[address];values=[v.get('annotation') if opcode==91 and i==0 else v['raw'] for i,v in enumerate(c['operands'])]
  if c['opcode']!=opcode or values!=expected:raise ValueError(f'Original voyage request changed:{address:x}')
 with Path(iso).open('rb') as source:
  source.seek(0x420);dol=struct.unpack('>I',source.read(4))[0];source.seek(dol+0x26de4);body=source.read(0x26c)
  if hashlib.sha256(body).hexdigest()!='7fdf537b820f81b6f7de2a753ef9dc3a23307ae650f5789c27de46f1db8240cd':raise ValueError('Original map animation play owner changed')
  # Mode2 backdates the timestamp by1000*duration/FPS rather than playing
  # the initial sky frame: r30==2, duration load, multiply/divide, subtraction.
  for offset,word in [(0x1f8,0x2c1e0002),(0x224,0xc0450008),(0x230,0xec4300b2),(0x238,0xec220024),(0x248,0x7c040010)]:
   if struct.unpack_from('>I',body,offset)[0]!=word:raise ValueError('Original completed map pose branch changed')
 return len(HASHES)+len(checks)+6
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--symbols',default='artifacts/reference/us_symbols.csv');a=p.parse_args();print('Original used voyage:',verify(a.iso,a.symbols),'event/request anchors verified')
