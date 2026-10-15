#!/usr/bin/env python3
"""Read-only original animInit auxiliary files; unchanged local payloads."""
import argparse,hashlib,json
from pathlib import Path
from iso_assets import DEFAULT_ISO,extract
SOURCES={
 'a/vivian.bin':(3088,'478c261680ace260c1a56b4ca1a16c7076209bd0d60c3b5a54c1d7cfcf0064b1'),
 'a/ag2tg.bin':(41984,'d756d29d6e8eb7848bf2ff96b1c463223c140f0c33f7385ffac1ca40918125ba')}
def build(iso=DEFAULT_ISO,output='romfs/animation-startup'):
 root=Path(output);rows=[]
 for name,(size,digest) in SOURCES.items():
  target=root/name;target.parent.mkdir(parents=True,exist_ok=True);extract(iso,name,target);data=target.read_bytes()
  if len(data)!=size or hashlib.sha256(data).hexdigest()!=digest:raise ValueError('Original startup payload mismatch: '+name)
  rows.append(json.loads(target.with_suffix(target.suffix+'.provenance.json').read_text()))
 meta=dict(sources=rows,policy='Unchanged original animInit auxiliary data; does not establish file-manager ownership or startup history',license='Nintendo/Intelligent Systems originals; local use only, no redistribution rights')
 (root/'sources.json').write_text(json.dumps(meta,indent=2)+'\n');return meta
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs/animation-startup');a=p.parse_args();build(a.iso,a.output)
