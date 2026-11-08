#!/usr/bin/env python3
"""Import exact original harbor light inputs with explicit native pointer relocation."""
import argparse,hashlib,json,struct,tempfile
from pathlib import Path
from iso_assets import DEFAULT_ISO,extract
from dol_event_closure import merge_segments
LICENSE='Nintendo/Intelligent Systems original game asset; no redistribution license granted'
SOURCE='m/gor_00/d'
SOURCE_SHA256='d942568687b82ede86c7ea5aab0a3aea6c0df23b538257fde8ae1632bd13089b'
TABLE_OFFSET=0xe0d9c
RECORD_BYTES=0x44
PAYLOAD_OFFSET=0x20
DEFAULT_GUEST_BASE=0x89000000

def import_source(iso,destination,guest_base=DEFAULT_GUEST_BASE):
 destination=Path(destination)
 if destination.exists() and (not destination.is_dir() or any(destination.iterdir())):
  raise ValueError('New imports require an empty destination; existing asset edits are preserved')
 with tempfile.TemporaryDirectory() as temporary:
  source=Path(temporary)/'gor_00.d';extract(iso,SOURCE,source);raw=source.read_bytes()
 if hashlib.sha256(raw).hexdigest()!=SOURCE_SHA256:raise ValueError('Expected original US harbor map')
 if guest_base<0 or guest_base&3 or guest_base+len(raw)>0x100000000:raise ValueError('Invalid native input address policy')
 word=lambda address:struct.unpack_from('>I',raw,address)[0]
 count=word(TABLE_OFFSET)
 if count!=5:raise ValueError('Unexpected original harbor light census')
 intervals=[];bindings=[];records=[]
 def add(offset,size):
  begin=offset&~3;end=(offset+size+3)&~3
  if begin<0 or end>len(raw):raise ValueError('Original input extent outside map')
  intervals.append((begin,end))
 def string(offset):
  end=raw.find(b'\0',offset,offset+256)
  if end<0:raise ValueError('Unterminated original map string')
  add(offset,end-offset+1)
 def relocate(site):
  relative=word(site);target=PAYLOAD_OFFSET+relative
  if target>=len(raw):raise ValueError('Original pointer outside map')
  bindings.append(dict(file_offset=site,original_relative=relative,target_file_offset=target,native=guest_base+target))
  return target
 add(TABLE_OFFSET,4+4*count)
 for index in range(count):
  offset=relocate(TABLE_OFFSET+4+4*index);add(offset,RECORD_BYTES)
  for field in (0,4):string(relocate(offset+field))
  records.append(dict(index=index,file_offset=offset,address=guest_base+offset,size=RECORD_BYTES,sha256=hashlib.sha256(raw[offset:offset+RECORD_BYTES]).hexdigest()))
 spans=[]
 for begin,end in intervals:
  data=bytearray(raw[begin:end])
  for binding in bindings:
   site=binding['file_offset']
   if begin<=site<end:struct.pack_into('>I',data,site-begin,binding['native'])
  spans.append((guest_base+begin,bytes(data)))
 packed=merge_segments(spans);segments=[]
 for index in range(struct.unpack_from('>I',packed,8)[0]):
  address,size,offset=struct.unpack_from('>III',packed,12+12*index)
  segments.append(dict(address=address,bytes=packed[offset:offset+size].hex()))
 document=dict(schema=1,room=1,guest_base=guest_base,table=guest_base+TABLE_OFFSET,segments=segments,license=LICENSE)
 provenance=dict(schema=1,iso_id='G8ME01 revision0',source=SOURCE,source_sha256=SOURCE_SHA256,table_file_offset=TABLE_OFFSET,payload_file_offset=PAYLOAD_OFFSET,records=records,bindings=bindings,
  policy='Exact original light-table/68-byte records/strings and real alignment neighbors. Only documented file-relative pointer fields relocate to an explicit native guest base. No map-entry runtime record, matrices, light outputs, code or invented light arguments are imported. Whole map loader remains separate.',license=LICENSE)
 destination.mkdir(parents=True,exist_ok=True)
 (destination/'event.json').write_text(json.dumps(document,indent=2)+'\n')
 (destination/'provenance.json').write_text(json.dumps(provenance,indent=2)+'\n')
 return document
if __name__=='__main__':
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--iso',default=DEFAULT_ISO)
 parser.add_argument('--destination',default='assets/original/lights/harbor-map-input')
 parser.add_argument('--guest-base',type=lambda value:int(value,0),default=DEFAULT_GUEST_BASE)
 args=parser.parse_args();import_source(args.iso,args.destination,args.guest_base)
