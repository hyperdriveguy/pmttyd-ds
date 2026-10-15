#!/usr/bin/env python3
"""Exact original cold UI process data, never a per-menu/reset snapshot."""
import argparse,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
# Real source data and source loader BSS; initialized sections take priority.
SPANS=[('font-names-and-literals',0x802c2b30,0x90),('SDK-format-table',0x803c4e84,60),('font-palette-spacing-name-pointers',0x80416010,0x44),('SDK-filter-table',0x8041e558,8),('font-global-object-state',0x8041e978,0x44),('original-root-string',0x8041f494,4),('font-draw-scalars',0x804203e4,0x60),('SDK-texture-scalars',0x80429440,0x28),('source-fog',0x80429578,4),('source-selected-color',0x80429598,4)]
# Filled from primary revision0 bytes and independently rechecked by tests.
SOURCE_DIGESTS={'font-names-and-literals': 'c2c7ce263b650f8f3cf827f7396a9899419cd9c13f0203864ae8cdda18ba5d22', 'SDK-format-table': '16a0d860dae00409363303437827e5134551420a4bd772d674377c6b9232bc34', 'font-palette-spacing-name-pointers': '04762b4599ae0a74c469a4854c3da10d4f6dbe64f63352e7cce66846151bd646', 'SDK-filter-table': '60a88cabebfb891f50f144f45bddaae888ebef35e3b63371b86139fc566ea9f9', 'font-global-object-state': '1751ac12e70e15b4f76c16775cd329ae55973b612521dab2de828a5cdb6c8ab3', 'original-root-string': 'c245aff91259378234ae88c46d0e6a690a75034a5fc42bd61f2e3d03c9158b33', 'font-draw-scalars': '1856c42a24bf9b3211de88cb1ae022a06c29153ca8fdc50ef62352cb260b2786', 'SDK-texture-scalars': '39000bf741678cfc740ea96d71568e60dfad9649537172e54863b0067b059236', 'source-fog': 'df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119', 'source-selected-color': 'df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119'}
SOURCE_FST_SHA='6c346f1a016d601dd75b308197017a96f09d6add2ebf4a0ca58929c979b6beb1'
GP_CODE=bytes.fromhex('806d90b038800001380000009083016c')

def u32(v,a):return struct.unpack_from('>I',v,a)[0]
def read_source(iso):
 inv=inventory(iso)
 if inv['disc_id']!='G8ME01':raise ValueError('Original US disc required')
 with Path(iso).open('rb')as f:
  h=f.read(0x440)
  if h[:8]!=b'G8ME01\0\0':raise ValueError('Original revision0 required')
  dol=u32(h,0x420);f.seek(dol);d=f.read(256);f.seek(u32(h,0x424));fst=f.read(u32(h,0x428))
  fst_sha=hashlib.sha256(fst).hexdigest()
  if SOURCE_FST_SHA and fst_sha!=SOURCE_FST_SHA:raise ValueError('Original FST changed')
  sections=[(u32(d,4*i),u32(d,0x48+4*i),u32(d,0x90+4*i))for i in range(18)];bss,n=u32(d,0xd8),u32(d,0xdc)
  def byte(a,initialized_only=False):
   matches=[(i,o+a-v)for i,(o,v,s)in enumerate(sections)if s and v<=a<v+s]
   if len(matches)>1:raise ValueError('Ambiguous original source span')
   if matches:
    i,o=matches[0]
    if i<7 and not initialized_only:raise ValueError('UI data cannot import executable text')
    f.seek(dol+o);v=f.read(1)
    if len(v)!=1:raise ValueError('Short original source')
    return v[0],('initialized-data',dol+o)
   if not initialized_only and bss<=a<bss+n:return 0,('source-loader-BSS',None)
   raise ValueError('Unavailable source byte '+hex(a))
  code=bytes(byte(0x8006fd0c+i,True)[0]for i in range(16))
  if code!=GP_CODE:raise ValueError('Original US GP producer changed')
  output=[];provenance=[]
  for name,a,size in SPANS:
   data=bytes(byte(a+i)[0]for i in range(size));digest=hashlib.sha256(data).hexdigest()
   if SOURCE_DIGESTS and SOURCE_DIGESTS.get(name)!=digest:raise ValueError('Original UI span changed: '+name)
   output.append((a,data));provenance.append({'name':name,'address':a,'bytes':size,'sha256':digest,'origins':[list(byte(a+i)[1])for i in range(size)]})
 fonts=[e for e in inv['files']if e['path'].endswith('.bfn')]
 if [e['path']for e in fonts]!=['f/papermarioset_EU.bfn','f/papermarioset_JPN.bfn','f/papermarioset_US.bfn']or any(e['path'].startswith('font/')for e in inv['files']):raise ValueError('Original font catalog changed')
 output.sort();offset=12+12*len(output);pack=bytearray(struct.pack('>8sI',b'PMEVT001',len(output)))
 for a,data in output:pack+=struct.pack('>III',a,len(data),offset);offset+=len(data)
 pack+=b''.join(data for a,data in output)
 return bytes(pack),{'source':'Original US revision0 DOL initialized data/source loader BSS; cold process only','dol_offset':dol,'bss_address':bss,'bss_bytes':n,'FST_sha256':fst_sha,'original_font_files':fonts,'original_font_directory_files':[],'US_GP_instruction_address':0x8006fd0c,'US_GP_instruction_hex':code.hex(),'spans':provenance,'pack_sha256':hashlib.sha256(pack).hexdigest(),'license':'Nintendo/Intelligent Systems original rights retained. No redistribution license.'}
def build(iso,output):
 pack,meta=read_source(iso);out=Path(output)
 if out.exists()and any(out.iterdir()):
  if {p.name for p in out.iterdir()}=={'memory.pmevt','memory.json'}and(out/'memory.pmevt').read_bytes()==pack and json.loads((out/'memory.json').read_text())==meta:return meta
  raise ValueError('New import requires an empty destination; preserving existing edits')
 out.mkdir(parents=True,exist_ok=True);(out/'memory.pmevt').write_bytes(pack);(out/'memory.json').write_text(json.dumps(meta,indent=2)+'\n');return meta
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs/source-ui-startup');a=p.parse_args();m=build(a.iso,a.output);print(json.dumps({'pack_sha256':m['pack_sha256'],'FST_sha256':m['FST_sha256'],'spans':{s['name']:s['sha256']for s in m['spans']}},indent=2))
