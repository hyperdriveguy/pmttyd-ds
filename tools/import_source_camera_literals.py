#!/usr/bin/env python3
"""Import initialized original camera constructor/SDK/selected NTSC literals, never process BSS."""
import argparse,hashlib,json
from pathlib import Path
from field_map_npc_assets import Dol
from iso_assets import DEFAULT_ISO
from import_harbor_map_source import LICENSE
SPANS=((2150364632, 144, 'be933e2af6bde701d97c19de683d3a715a0b55baf302e5afdc4b1ff5f4f3e48a'), (2151763888, 36, '5372dfac66c8844e2ce8c9ad1d1e182204258d8d535e3f91e7206335015c150e'), (2151806644, 168, 'af907f60fff209d82b86b3310a707c59397fc94759bdaf3fc644ec440a33374f'), (2151846776, 36, 'f813595347a52f5c6b964cf5518d9ea4ae5c7d9c433ddddfae930b109037271b'), (2150679208, 120, '2b41364ca856013a7d182ee377db148f6e70da0861ef31b45168973ec0115a1b'))
def import_literals(iso,destination):
 out=Path(destination)
 if out.exists() and (not out.is_dir() or any(out.iterdir())):raise ValueError('New imports require an empty destination; existing edits are preserved')
 d=Dol(iso);segments=[];records=[]
 try:
  for address,size,sha in SPANS:
   raw=d.read(address,size)
   if hashlib.sha256(raw).hexdigest()!=sha:raise ValueError('Changed original camera literal')
   segments.append(dict(address=address,bytes=raw.hex()));records.append(dict(address=address,size=size,sha256=sha))
 finally:d.close()
 out.mkdir(parents=True,exist_ok=True)
 (out/'event.json').write_text(json.dumps(dict(schema=1,segments=segments,license=LICENSE),indent=2)+'\n')
 (out/'provenance.json').write_text(json.dumps(dict(schema=1,iso_id='G8ME01 revision0',source='main.dol initialized data',spans=records,license=LICENSE,policy='Unchanged original camera names/vectors, constructor/SDK/double constants and both MarioSt NTSC descriptors; no camera table, runtime camera record, render-mode pointer/BSS, heap or GX state imported.'),indent=2)+'\n')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--destination',required=True);a=p.parse_args();import_literals(a.iso,a.destination)
