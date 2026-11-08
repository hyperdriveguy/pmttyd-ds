#!/usr/bin/env python3
"""Import original hit-constructor/Distance literals to a new empty destination."""
import argparse,hashlib,json
from pathlib import Path
from field_map_npc_assets import Dol
from iso_assets import DEFAULT_ISO
from import_harbor_map_source import LICENSE
SPANS=((0x8041f838,32,'0055f7787261b2154c6b4b92b355e4bb235c694398d2e0cf500393942104a471'),
       (0x80429394,8,'f612898144a3878d184ef21814fd68d405df49bc2289b8191737d037c2ef2229'))
def import_literals(iso,destination):
 out=Path(destination)
 if out.exists() and (not out.is_dir() or any(out.iterdir())):raise ValueError('New imports require an empty destination')
 dol=Dol(iso);segments=[];records=[]
 try:
  for address,size,sha in SPANS:
   raw=dol.read(address,size)
   if hashlib.sha256(raw).hexdigest()!=sha:raise ValueError('Original hit literal changed')
   segments.append(dict(address=address,bytes=raw.hex()));records.append(dict(address=address,size=size,sha256=sha))
 finally:dol.close()
 files={'event.json':dict(schema=1,segments=segments,license=LICENSE),
        'provenance.json':dict(schema=1,source='G8ME01 revision0 read-only DOL',spans=records,license=LICENSE,policy='Original initialized hit and Distance constants; no code, estimate table or synthesized radii.')}
 out.mkdir(parents=True,exist_ok=True)
 for name,data in files.items():(out/name).write_text(json.dumps(data,indent=2)+'\n')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--destination',required=True);a=p.parse_args();import_literals(a.iso,a.destination)
