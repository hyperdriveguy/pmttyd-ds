#!/usr/bin/env python3
"""Import unchanged original harbor camera file and exact source literals."""
import argparse,hashlib,json
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
from field_map_npc_assets import Dol
from import_harbor_map_source import LICENSE
SPANS=((2151806504, 28, '5dcfb9fab9c516b8561fe6042750d7189e064f9f6b44abbb2efc9ef51e9848e7'), (2151806624, 16, '5f16ef13e6a4567ff1b267a6e9a691a11f2f018478f76a1f15831cbe0e654cd9'), (2151806644, 4, 'df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119'), (2151806668, 4, 'b4be016994a699fb6f1a6b61fa14065baacfc6d51af90abee79ebee422889a93'), (2151817192, 16, '2bdfd121d0c511aaa219cdd76fadf9a44500c0c4af7693a9bb3ba3133cd8f71a'), (2151763880, 4, '6c8eca4cb5f362ac72b2bf30be380ae1e60c1f786952973d297e6d45a5485ed9'), (2151770568, 4, 'bc7dc434a8b5aa7f90892f5d9a6425bf172136242b433cbdf86400871871194b'), (2150365528, 576, 'a90f9d0c7fe83f77349a9192f5842b872436cee9cc08d688e7a78f35fb8d2b8b'))
CAMERA_SHA='3d6f746ec51b1ba3165ba67aee2ca1fe17deb2d11a8fda9d3181a72356246d0c'
def import_camera(iso,destination):
 out=Path(destination)
 if out.exists() and (not out.is_dir() or any(out.iterdir())):raise ValueError('New imports require an empty destination; existing edits are preserved')
 dol=Dol(iso);segments=[];spans=[]
 try:
  for a,n,sha in SPANS:
   raw=dol.read(a,n)
   if hashlib.sha256(raw).hexdigest()!=sha:raise ValueError('Original camera literal changed')
   segments.append(dict(address=a,bytes=raw.hex()));spans.append(dict(address=a,size=n,sha256=sha))
 finally:dol.close()
 info=inventory(iso);rows=[r for r in info['files']if r['path']=='m/gor_00/c']
 if len(rows)!=1 or rows[0]['size']!=2048:raise ValueError('Expected original US harbor camera')
 row=rows[0]
 with Path(iso).open('rb')as disc:disc.seek(row['offset']);camera=disc.read(row['size'])
 if hashlib.sha256(camera).hexdigest()!=CAMERA_SHA:raise ValueError('Original camera file changed')
 outputs={'c.bin':camera,'literals.json':(json.dumps(dict(schema=1,segments=segments,license=LICENSE),indent=2)+'\n').encode(),
 'provenance.json':(json.dumps(dict(schema=1,iso_id='G8ME01 revision0',source='m/gor_00/c',source_offset=row['offset'],logical_bytes=len(camera),transfer_bytes=len(camera),sha256=CAMERA_SHA,dol_spans=spans,license=LICENSE,policy='Unchanged source file; source setup owns mutable map-heap copy and exact relocation. Original initialized pointer words/literals only; no runtime camera/road/shift objects imported.'),indent=2)+'\n').encode()}
 out.mkdir(parents=True,exist_ok=True)
 for name,data in outputs.items():(out/name).write_bytes(data)
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--destination',required=True);a=p.parse_args();import_camera(a.iso,a.destination)
