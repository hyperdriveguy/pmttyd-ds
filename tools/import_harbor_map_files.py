#!/usr/bin/env python3
"""Import unrelocated harbor D/T and exact rounded-read bytes from read-only ISO."""
import argparse,hashlib,json
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
from field_map_npc_assets import Dol
from import_harbor_map_source import LICENSE,SOURCE_SHA256
CACHE_LINE=32
# Full source loader literals; constructor/SDK literals remain in harbor-source.
DOL_SPANS=((0x802bfb98,0x140),(0x803049d4,15*4),(0x8041f90c,8),
           (0x8041f928,8),(0x8041f94c,4),(0x8041f9ac,12))
def import_files(iso,destination):
 destination=Path(destination)
 if destination.exists() and (not destination.is_dir() or any(destination.iterdir())):
  raise ValueError('New imports require an empty destination; existing edits are preserved')
 info=inventory(iso)
 if info['disc_id']!='G8ME01':raise ValueError('Expected original US disc')
 outputs={};records=[]
 with Path(iso).open('rb') as disc:
  for name in ('d','t'):
   source='m/gor_00/'+name
   rows=[r for r in info['files'] if r['path']==source]
   if len(rows)!=1:raise ValueError('Missing exact original file')
   row=rows[0];length=row['size'];transfer=(length+CACHE_LINE-1)&~(CACHE_LINE-1)
   disc.seek(row['offset']);raw=disc.read(transfer)
   if len(raw)!=transfer:raise ValueError('Truncated rounded disc transfer')
   original_sha=hashlib.sha256(raw[:length]).hexdigest()
   if name=='d' and original_sha!=SOURCE_SHA256:raise ValueError('Expected revision0 harbor D')
   outputs[name+'.bin']=raw
   records.append(dict(file=name+'.bin',source=source,source_offset=row['offset'],logical_bytes=length,
    transfer_bytes=transfer,source_sha256=original_sha,transfer_sha256=hashlib.sha256(raw).hexdigest(),
    trailing_hex=raw[length:].hex(),license=LICENSE))
 dol=Dol(iso);segments=[];spans=[]
 try:
  for address,size in DOL_SPANS:
   raw=dol.read(address,size);segments.append(dict(address=address,bytes=raw.hex()))
   spans.append(dict(address=address,size=size,sha256=hashlib.sha256(raw).hexdigest()))
 finally:dol.close()
 outputs['literals.json']=(json.dumps(dict(schema=1,segments=segments,license=LICENSE),indent=2)+'\n').encode()
 outputs['provenance.json']=(json.dumps(dict(schema=1,iso_id='G8ME01 revision0',files=records,dol_spans=spans,
  policy='Unrelocated original files. Bytes after logical EOF are exact ISO transfer padding, not invented zeroes. Runtime reports logical lengths and executes original relocation/TPL unpack in mutable map-owned copies.',license=LICENSE),indent=2)+'\n').encode()
 destination.mkdir(parents=True,exist_ok=True)
 for name,raw in outputs.items():(destination/name).write_bytes(raw)
 return records
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO)
 p.add_argument('--destination',default='assets/original/maps/harbor-files');a=p.parse_args();import_files(a.iso,a.destination)
