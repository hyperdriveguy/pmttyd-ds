#!/usr/bin/env python3
"""Original mutable area registry and bounded gor module registration data."""
import csv,hashlib,struct
from pathlib import Path
from field_battle_setup_assets import GOR_SHA256
from rel_data_pack import bounded_segments

REGISTRY_SPANS=[(0x80305320,0x5cc,'3be7223e5d25e22fcd6e13cea93df50275d8c295b641af80a1be8d2e01f9d195'),
 (0x80415b30,0x270,'efce67bc5c2db7be0a0729b5dc249295e66b70c82b396e39cd2d6b93b5977723'),
 (0x8041fa60,0xa4,'86e4147bb144ef34256589ab6503657b0b5bd8794c20a627d2afb299f5caf272')]

def gor_registry_segments(iso,raw,symbols):
 if hashlib.sha256(raw).hexdigest()!=GOR_SHA256:raise ValueError('Unexpected original US gor REL')
 views=[(0x13d60,4,'gor.o','str_gor_gor_00013d60'),(0x6100c,0x38,'battle_database_gor.o','btl_gor_setup_no_tbl')]
 names=['BTLNO_GOR_TEST1','BTLNO_GOR_TEST2','BTLNO_GOR_00_01','BTLNO_GOR_02_ACLEC','BTLNO_GOR_02_SACLEC','BTLNO_GOR_02_01']
 for a,n,name in zip([0x1dc80,0x1dc90,0x1dca0,0x1dcb0,0x1dcc4,0x1dcd8],[0x10,0x10,0x10,0x14,0x14,0x10],names):views.append((a,n,'battle_database_gor.o',f'str_{name}_gor_{a:08x}'))
 with Path(symbols).open() as source:rows=list(csv.DictReader(source))
 for begin,size,namespace,name in views:
  matches=[r for r in rows if r['area']=='gor' and r['namespace']==namespace and r['name']==name and r['sec_type']=='data']
  if len(matches)!=1 or int(matches[0]['file_addr'],16)!=begin or (int(matches[0]['size'],16)+3)&~3!=size:raise ValueError('Unexpected module view owner: '+name)
 segments,rel_meta=bounded_segments(raw,[(a,n) for a,n,_,_ in views])
 for m,(_,_,namespace,name) in zip(rel_meta,views):m.update(namespace=namespace,name=name)
 def read(a,n):
  for base,data in segments:
   if base<=a and a+n<=base+len(data):return data[a-base:a-base+n]
  raise ValueError('Registry dependency outside admitted views')
 if read(0x90400260,4)!=b'gor\0':raise ValueError('Unexpected original module name')
 table=struct.unpack('>14I',read(0x90540f84,0x38))
 for i,(address,name) in enumerate(zip([0x9040a180,0x9040a190,0x9040a1a0,0x9040a1b0,0x9040a1c4,0x9040a1d8],names)):
  if table[2*i:2*i+2]!=(address,i) or read(address,len(name)+1)!=name.encode()+b'\0':raise ValueError('Unexpected original battle number mapping')
 if table[-2:]!=(0,0):raise ValueError('Missing original battle number terminator')
 dol_meta=[]
 with Path(iso).open('rb') as disc:
  if disc.read(8)!=b'G8ME01\0\0':raise ValueError('Original US disc0 revision0 required')
  disc.seek(0x420);dol=struct.unpack('>I',disc.read(4))[0];disc.seek(dol);header=disc.read(256)
  for address,size,expected in REGISTRY_SPANS:
   matches=[]
   for i in range(18):
    off,va,length=(struct.unpack_from('>I',header,b+4*i)[0] for b in (0,0x48,0x90))
    if va<=address and address+size<=va+length:matches.append(off+address-va)
   if len(matches)!=1:raise ValueError('Missing original registry DOL span')
   disc.seek(dol+matches[0]);data=disc.read(size);digest=hashlib.sha256(data).hexdigest()
   if digest!=expected:raise ValueError('Unexpected original registry bytes')
   segments.append((address,data));dol_meta.append(dict(address=address,size=size,sha256=digest))
 records=[]
 for i in range(34):
  record=struct.unpack('>I',read(0x803057ec+4*i,4))[0]
  if not record:
   if i!=33:raise ValueError('Unexpected original registry terminator')
   break
  name=struct.unpack('>I',read(record,4))[0];label=read(name,4)
  if label[3] or not all(97<=b<=122 for b in label[:3]) or read(record+8,8)!=bytes(8):raise ValueError('Unexpected original area record')
  records.append(dict(address=record,name=label[:3].decode(),name_address=name))
 if len(records)!=33 or next(r for r in records if r['name']=='gor')['address']!=0x80305364:raise ValueError('Unexpected gor registry identity')
 # Exact _prolog5e4..6ec relSetBtlAddr call at6d4. Numeric admission is a
 # native bounded-data domain, not the original table's full extent.
 registration=(2,0x90400260,0x90540da8,0x90540f84,0,1)
 return segments,registration,dict(registry=0x803057e4,records=records,dol_views=dol_meta,rel_views=rel_meta,
  prolog=dict(file_offset=0x5e4,size=0x108,sha256=hashlib.sha256(raw[0x5e4:0x6ec]).hexdigest(),registration_call=0x6d4),
  registration=dict(room=2,name=registration[1],setup=registration[2],numbers=registration[3],first=0,count=1),
  policy='Original zeroed DOL registry; runtime-owned writable copies registered through source helper. Number mappings0..5 do not admit battle data beyond numeric0. No epilog registry clear is invented.')
