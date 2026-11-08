#!/usr/bin/env python3
"""Import pinned original constructor data and BSS; never overwrite edits."""
import argparse,hashlib,json,struct
from pathlib import Path
from field_map_npc_assets import Dol
from iso_assets import DEFAULT_ISO
ROOT=Path(__file__).resolve().parents[1]
LICENSE='Nintendo/Intelligent Systems original game asset; no redistribution license granted'

def build(iso,destination):
 destination=Path(destination)
 if destination.exists() and (not destination.is_dir() or any(destination.iterdir())):raise ValueError('Import destination must be empty')
 dol=Dol(iso);segments=[];provenance=[]
 try:
  for body in json.loads((ROOT/'tools/mario_constructor_bodies.json').read_text())['bodies']:
   a=int(body['address'],16);raw=dol.read(a,body['size'])
   if hashlib.sha256(raw).hexdigest()!=body['sha256']:raise ValueError('Original whole body differs')
  bss,bss_size=struct.unpack_from('>II',dol.header,0xd8)
  for span in json.loads((ROOT/'tools/mario_constructor_assets.json').read_text())['spans']:
   a=int(span['address'],16);n=span['size'];offset=None
   sections=[]
   for i in range(18):
    off,base,size=[struct.unpack_from('>I',dol.header,k+4*i)[0] for k in (0,0x48,0x90)]
    if base<=a and a+n<=base+size:sections.append(dol.base+off+a-base)
   if len(sections)==1:raw=dol.read(a,n);offset=sections[0];origin='initialized DOL'
   elif not sections and bss<=a and a+n<=bss+bss_size:raw=bytes(n);origin='original DOL BSS zero'
   else:raise ValueError('Original source mapping differs')
   if hashlib.sha256(raw).hexdigest()!=span['sha256']:raise ValueError('Original data differs')
   segments.append({'address':a,'bytes':raw.hex()});provenance.append(dict(span,source=origin,iso_offset=offset,license=LICENSE))
 finally:dol.close()
 destination.mkdir(parents=True,exist_ok=True)
 (destination/'memory.json').write_text(json.dumps({'schema':1,'segments':segments,'license':LICENSE},indent=2)+'\n')
 (destination/'provenance.json').write_text(json.dumps({'source':'US G8ME01 disc0 revision0','spans':provenance,'bodies':json.loads((ROOT/'tools/mario_constructor_bodies.json').read_text())['bodies'],'conversion':'Unchanged source initialized data and original BSS zero point. Existing immutable boot prefixes are borrowed and validated, never overwritten; source Init performs all runtime writes. Actual Mario/pouch allocations and stacks are runtime owners, not imported addresses.','license':LICENSE},indent=2)+'\n')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--destination',required=True);a=p.parse_args();build(a.iso,a.destination)
