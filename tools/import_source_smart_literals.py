#!/usr/bin/env python3
"""Import initialized original smart arena/five-heap literals, never process BSS."""
import argparse,hashlib,json
from pathlib import Path
from field_map_npc_assets import Dol
from iso_assets import DEFAULT_ISO
from import_harbor_map_source import LICENSE
SPANS=((2151764408, 44, 'f7082911b10bdedc9fdcbd5610672f5044d98ed797477ff1b1c2a4cdcfc40db6'),)
def import_literals(iso,destination):
 out=Path(destination)
 if out.exists() and (not out.is_dir() or any(out.iterdir())):raise ValueError('New imports require an empty destination; existing edits are preserved')
 d=Dol(iso);segments=[];records=[]
 try:
  for address,size,sha in SPANS:
   raw=d.read(address,size)
   if hashlib.sha256(raw).hexdigest()!=sha:raise ValueError('Changed original smart literal')
   segments.append(dict(address=address,bytes=raw.hex()));records.append(dict(address=address,size=size,sha256=sha))
 finally:d.close()
 out.mkdir(parents=True,exist_ok=True)
 (out/'event.json').write_text(json.dumps(dict(schema=1,segments=segments,license=LICENSE),indent=2)+'\n')
 (out/'provenance.json').write_text(json.dumps(dict(schema=1,iso_id='G8ME01 revision0',source='main.dol initialized data',spans=records,license=LICENSE,policy='Unchanged original five process-heap configuration rows and smart manager pointer; no BSS, heap bounds, live headers or arena payload imported.'),indent=2)+'\n')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--destination',required=True);a=p.parse_args();import_literals(a.iso,a.destination)
