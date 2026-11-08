#!/usr/bin/env python3
"""Import initialized original shadow/SDK literals, never process BSS."""
import argparse,hashlib,json
from pathlib import Path
from field_map_npc_assets import Dol
from iso_assets import DEFAULT_ISO
from import_harbor_map_source import LICENSE
SPANS=((2151764192, 16, '9952b93874baddbc91739fda14d4872168373bb19fff8ceacf08f241feeebb86'), (2151807424, 76, '2a3564436d1ea771b605d9629cc6d82420a67899de0c9ecd98a633904e06cb78'), (2151846976, 48, 'b5d68875a4cc1e901d2728f1f81f76c386d094c303944ea58a87a1bc15b716cf'), (2151435908, 60, '16a0d860dae00409363303437827e5134551420a4bd772d674377c6b9232bc34'), (2151802200, 8, '60a88cabebfb891f50f144f45bddaae888ebef35e3b63371b86139fc566ea9f9'), (2151435420, 244, 'bc17e09c1a6c3c53dc706accbd364c7e73d176b612b095ab2b92907983a9680b'))
def import_literals(iso,destination):
 out=Path(destination)
 if out.exists() and (not out.is_dir() or any(out.iterdir())):raise ValueError('New imports require an empty destination; existing edits are preserved')
 d=Dol(iso);segments=[];records=[]
 try:
  for address,size,sha in SPANS:
   raw=d.read(address,size)
   if hashlib.sha256(raw).hexdigest()!=sha:raise ValueError('Changed original shadow literal')
   segments.append(dict(address=address,bytes=raw.hex()));records.append(dict(address=address,size=size,sha256=sha))
 finally:d.close()
 out.mkdir(parents=True,exist_ok=True)
 (out/'event.json').write_text(json.dumps(dict(schema=1,segments=segments,license=LICENSE),indent=2)+'\n')
 (out/'provenance.json').write_text(json.dumps(dict(schema=1,iso_id='G8ME01 revision0',source='main.dol initialized data',spans=records,license=LICENSE,policy='Unchanged initialized pointers, dimensions, scalar/color constants and SDK format jump table. No BSS, initialized shadow owners, generated texture ramps or heap allocations imported.'),indent=2)+'\n')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--destination',required=True);a=p.parse_args();import_literals(a.iso,a.destination)
