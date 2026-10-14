#!/usr/bin/env python3
"""Unchanged original animation group prefetch payloads; local ROMFS only."""
import argparse,hashlib,json
from pathlib import Path
from iso_assets import DEFAULT_ISO,extract
SOURCES={
 'a/ag2tg.bin':(41984,'d756d29d6e8eb7848bf2ff96b1c463223c140f0c33f7385ffac1ca40918125ba'),
 'a/p_dokan_x':(41756,'4cf08bf10e8d4e3353e3d1f1ab1b52d1979c89793be0bd926e7f39bfa7009373'),
 'a/p_dokan_y':(18576,'aa93194c9a80f227e6129597b3dfa7adbec7de42c81fce85b83cf16a06b84247'),
 'a/p_slit-':(22592,'84ffe23a4bca900211eb373783cb5c363c1e5e9047f5a2d482b00ead6a6ecb3e')}
def build(iso=DEFAULT_ISO):
 root=Path('romfs/field-prefetch');meta=[]
 for name,(size,digest) in SOURCES.items():
  target=root/name;target.parent.mkdir(parents=True,exist_ok=True);extract(iso,name,target);data=target.read_bytes()
  if len(data)!=size or hashlib.sha256(data).hexdigest()!=digest:raise ValueError('Unexpected original prefetch source '+name)
  meta.append(dict(source=name,bytes=size,sha256=digest))
 (root/'sources.json').write_text(json.dumps(dict(sources=meta,policy='Unchanged group/texture/table bytes; cooperative native prefetch, no pose creation',license='Original Nintendo/Intelligent Systems assets; local use only, no redistribution rights'),indent=2)+'\n')
 return meta
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);build(p.parse_args().iso)
