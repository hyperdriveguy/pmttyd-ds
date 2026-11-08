#!/usr/bin/env python3
"""Import original harbor payload and original relocation table, never runtime objects."""
import argparse,hashlib,json,struct,tempfile
from pathlib import Path
from iso_assets import DEFAULT_ISO,extract
from field_map_npc_assets import Dol
LICENSE='Nintendo/Intelligent Systems original game asset; no redistribution license granted'
SOURCE='m/gor_00/d'
SOURCE_SHA256='d942568687b82ede86c7ea5aab0a3aea6c0df23b538257fde8ae1632bd13089b'
PAYLOAD_OFFSET=0x20
DEFAULT_GUEST_BASE=0x8a000000
# Actual reached map/SDK literals. Existing light definitions remain editable.
DOL_SPANS=((0x8041f930,0x20),(0x8041f9b8,4),(0x8041e458,8),(0x80429358,8))
def import_source(iso,destination,guest_base=DEFAULT_GUEST_BASE):
 destination=Path(destination)
 if destination.exists() and (not destination.is_dir() or any(destination.iterdir())):raise ValueError('New imports require an empty destination; existing edits are preserved')
 with tempfile.TemporaryDirectory() as temporary:
  original=Path(temporary)/'d';extract(iso,SOURCE,original);raw=original.read_bytes()
 if hashlib.sha256(raw).hexdigest()!=SOURCE_SHA256:raise ValueError('Expected original US harbor map')
 word=lambda offset:struct.unpack_from('>I',raw,offset)[0]
 payload_size=word(4);relocation_count=word(8);chunk_count=word(12)
 if guest_base&3 or guest_base<0 or guest_base+payload_size>0x100000000:raise ValueError('Invalid native guest-base policy')
 payload=bytearray(raw[PAYLOAD_OFFSET:PAYLOAD_OFFSET+payload_size]);bindings=[]
 if len(payload)!=payload_size:raise ValueError('Truncated source payload')
 fixups=PAYLOAD_OFFSET+payload_size
 for index in range(relocation_count):
  site=word(fixups+index*4)
  if site&3 or site+4>payload_size:raise ValueError('Invalid source fixup site')
  pointer=struct.unpack_from('>I',payload,site)[0]
  if pointer>=payload_size:raise ValueError('Invalid source pointer')
  bindings.append(dict(site=site,original_relative=pointer))
  struct.pack_into('>I',payload,site,guest_base+pointer)
 chunks_at=fixups+relocation_count*4;names_at=chunks_at+chunk_count*8
 chunks={}
 for index in range(chunk_count):
  entry=chunks_at+8*index;name_at=names_at+word(entry+4);end=raw.index(b'\0',name_at)
  chunks[raw[name_at:end].decode('ascii')]=word(entry)
 information=chunks['information'];scene_name_pointer=word(PAYLOAD_OFFSET+information+8)
 end=raw.index(b'\0',PAYLOAD_OFFSET+scene_name_pointer);scene_name=raw[PAYLOAD_OFFSET+scene_name_pointer:end].decode('ascii')
 root=word(PAYLOAD_OFFSET+information+4)
 def name(joint):
  address=PAYLOAD_OFFSET+word(PAYLOAD_OFFSET+joint);return raw[address:raw.index(b'\0',address)].decode('ascii')
 visited=set();scene=None
 def walk(joint):
  nonlocal scene
  if joint in visited:raise ValueError('Source joint graph cycles or aliases')
  visited.add(joint)
  if name(joint)==scene_name:scene=joint
  for field in (0x0c,0x10):
   child=word(PAYLOAD_OFFSET+joint+field)
   if child:walk(child)
 walk(root)
 if scene is None or scene_name!='S':raise ValueError('Unexpected original scene selection')
 scene_nodes=[]
 def subtree(joint,siblings=True):
  scene_nodes.append(dict(relative=joint,name=name(joint)))
  child=word(PAYLOAD_OFFSET+joint+0x0c)
  if child:subtree(child)
  sibling=word(PAYLOAD_OFFSET+joint+0x10)
  if siblings and sibling:subtree(sibling)
 subtree(scene,False)
 if len(scene_nodes)!=540 or scene_nodes[52]['name']!='S_fune_b':raise ValueError('Original scene census changed')
 dol=Dol(iso);segments=[dict(address=guest_base,bytes=payload.hex())];spans=[]
 try:
  for address,size in DOL_SPANS:
   data=dol.read(address,size);segments.append(dict(address=address,bytes=data.hex()));spans.append(dict(address=address,size=size,sha256=hashlib.sha256(data).hexdigest()))
 finally:dol.close()
 document=dict(schema=1,guest_base=guest_base,scene=guest_base+scene,scene_name=scene_name,segments=segments,license=LICENSE)
 provenance=dict(schema=1,iso_id='G8ME01 revision0',source=SOURCE,source_sha256=SOURCE_SHA256,payload_file_offset=PAYLOAD_OFFSET,payload_size=payload_size,payload_sha256=hashlib.sha256(raw[PAYLOAD_OFFSET:PAYLOAD_OFFSET+payload_size]).hexdigest(),bindings=bindings,chunks=chunks,scene_nodes=scene_nodes,dol_spans=spans,license=LICENSE,policy='Exact complete initialized map payload, applying every original fixup once at explicit native guest base. No MapObj/MapEnt/matrix/light output is imported. Native scene selection is an asset admission reference; full map-load process remains separate.')
 destination.mkdir(parents=True,exist_ok=True)
 (destination/'event.json').write_text(json.dumps(document,indent=2)+'\n');(destination/'provenance.json').write_text(json.dumps(provenance,indent=2)+'\n')
 return document
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--destination',default='assets/original/maps/harbor-source');p.add_argument('--guest-base',type=lambda x:int(x,0),default=DEFAULT_GUEST_BASE)
 a=p.parse_args();import_source(a.iso,a.destination,a.guest_base)
