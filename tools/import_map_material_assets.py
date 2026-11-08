#!/usr/bin/env python3
"""Import exact initialized map material literals into a new editable directory."""
import argparse,hashlib,json
from pathlib import Path
from field_map_npc_assets import Dol
from iso_assets import DEFAULT_ISO
ROOT=Path(__file__).resolve().parents[1]
LICENSE='Nintendo/Intelligent Systems original game asset; no redistribution license granted'
def import_source(iso,destination):
 destination=Path(destination)
 if destination.exists() and (not destination.is_dir() or any(destination.iterdir())):
  raise ValueError('New imports require an empty destination; existing edits are preserved')
 evidence=json.loads((ROOT/'tools/map_material_source_evidence.json').read_text())
 spans=[(c['address'],len(bytes.fromhex(c['hex'])),hashlib.sha256(bytes.fromhex(c['hex'])).hexdigest()) for c in evidence['constants']]
 spans += [(d['address'],d['size'],d['sha256']) for d in evidence['data'] if d['name']!='ten projected matrix IDs']
 segments=[];provenance=[];dol=Dol(iso)
 try:
  for address,size,sha in spans:
   data=dol.read(address,size)
   if hashlib.sha256(data).hexdigest()!=sha:raise ValueError('Original material literal fingerprint differs')
   segments.append(dict(address=address,bytes=data.hex()))
   provenance.append(dict(source='sys/main.dol',address=address,size=size,sha256=sha))
 finally:dol.close()
 document=dict(schema=1,segments=segments,license=LICENSE)
 metadata=dict(schema=1,iso_id='G8ME01 revision0',spans=provenance,
  policy='Exact initialized material scalar constants, wrap/light IDs, direction seed, unsigned LOD bias and ambient fallback. Runtime map/light/GX state is never imported.',license=LICENSE)
 destination.mkdir(parents=True,exist_ok=True)
 (destination/'event.json').write_text(json.dumps(document,indent=2)+'\n')
 (destination/'provenance.json').write_text(json.dumps(metadata,indent=2)+'\n')
 return document
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--destination',default='assets/original/maps/material-source');a=p.parse_args();import_source(a.iso,a.destination)
