#!/usr/bin/env python3
"""Explicit read-only DOL light-definition import into an empty editable source."""
import argparse,hashlib,json
from pathlib import Path
from field_map_npc_assets import Dol
from iso_assets import DEFAULT_ISO
LICENSE='Nintendo/Intelligent Systems original game asset; no redistribution license granted'
SPANS=((0x802bf870,0xe4),(0x802bfc88,0x3c),(0x8041f8b0,0x2c),(0x8041f944,4),(0x80429358,8))
def import_source(iso,destination):
 destination=Path(destination)
 if destination.exists() and (not destination.is_dir() or any(destination.iterdir())):
  raise ValueError('New imports require an empty destination; existing asset edits are preserved')
 dol=Dol(iso);segments=[];provenance=[]
 try:
  for address,size in SPANS:
   data=dol.read(address,size)
   segments.append(dict(address=address,bytes=data.hex()))
   provenance.append(dict(source='sys/main.dol',address=address,size=size,sha256=hashlib.sha256(data).hexdigest()))
 finally:dol.close()
 document=dict(schema=1,segments=segments,license=LICENSE)
 metadata=dict(schema=1,iso_id='G8ME01 revision0',spans=provenance,
  policy='Exact initialized DOL preset vectors, attenuation table, duplicate-name format, names, scalar constants and SDK ONE/ZERO. Native BSS records/allocations are runtime owners, not copied initialized presets.',license=LICENSE)
 destination.mkdir(parents=True,exist_ok=True)
 (destination/'event.json').write_text(json.dumps(document,indent=2)+'\n')
 (destination/'provenance.json').write_text(json.dumps(metadata,indent=2)+'\n')
 return document
if __name__=='__main__':
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--iso',default=DEFAULT_ISO)
 parser.add_argument('--destination',default='assets/original/lights/registry-definition')
 args=parser.parse_args();import_source(args.iso,args.destination)
