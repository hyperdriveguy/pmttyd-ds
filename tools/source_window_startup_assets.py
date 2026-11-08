#!/usr/bin/env python3
"""Original cold window/archive state and literals; not WindowReady output."""
import argparse,hashlib,json,struct,tempfile
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
ARCHIVE_DESCRIPTORS=0x803dbff0
ARCHIVE_DESCRIPTOR_COUNT=4
ARCHIVE_DESCRIPTOR_BYTES=40
ARCHIVE_TABLE_BYTES=ARCHIVE_DESCRIPTOR_COUNT*ARCHIVE_DESCRIPTOR_BYTES
SPANS=[('TEX-unsigned-bias-and-panic',0x802c0140,0x48),('window-source-paths',0x802c3080,0x28),('archive-descriptors',ARCHIVE_DESCRIPTORS,ARCHIVE_TABLE_BYTES),('window-source-globals',0x8041ea18,16)]
DIGESTS={'TEX-unsigned-bias-and-panic': 'b45a3d95f9827ad01df8c3ccd5d626a57053f5071c0ef1b1da1e87a68c1e2524', 'window-source-paths': 'a7443510b0d8b9963a7231329373d02a07571692133a05f415463da46fc56015', 'archive-descriptors': 'b393978842a0fa3d3e1470196f098f473f9678e72463cb65ec4ab5581856c2e4', 'window-source-globals': '374708fff7719dd5979ec875d56cd2286f6d3cf7ec317a3b25632aab28ec37bb'}
ARC_SHA='988c2090fbce7356e08ed4b5714683f8b7026669f3f6232e7767bce04b7166fa'
def read_source(iso):
 with Path(iso).open('rb')as f:
  h=f.read(0x440)
  if h[:8]!=b'G8ME01\0\0':raise ValueError('US revision0 source required')
  word=lambda v,a:struct.unpack_from('>I',v,a)[0]
  dol=word(h,0x420);f.seek(dol);d=f.read(256);sections=[(word(d,4*i),word(d,0x48+4*i),word(d,0x90+4*i))for i in range(18)];bss,n=word(d,0xd8),word(d,0xdc)
  def data(a,size,code=False):
   out=bytearray();origins=[]
   for at in range(a,a+size):
    m=[(i,o+at-v)for i,(o,v,l)in enumerate(sections)if l and v<=at<v+l]
    if len(m)==1:
     i,o=m[0]
     if i<7 and not code:raise ValueError('No executable import')
     f.seek(dol+o);raw=f.read(1)
     if len(raw)!=1:raise ValueError('Short source')
     out+=raw;origins.append(['initialized-code'if code else'initialized-data',dol+o])
    elif not m and not code and bss<=at<bss+n:out.append(0);origins.append(['source-loader-BSS',None])
    else:raise ValueError('Unavailable original source span')
   return bytes(out),origins
  raw,_=data(0x800782e4,0xd4,True);digest=hashlib.sha256(raw).hexdigest()
  if ARC_SHA and digest!=ARC_SHA:raise ValueError('Original archive dispatcher changed')
  # Derive the descriptor address independently from the pinned whole original
  # arcOpen body. The addi immediate is signed; accepting an unsigned low half
  # previously admitted an unrelated cold BSS range with identical zero bytes.
  high=word(raw,8)&0xffff;low=word(raw,24)&0xffff
  address=((high<<16)+(low-0x10000 if low&0x8000 else low))&0xffffffff
  count=word(raw,0x94)&0xffff;stride=word(raw,0x90)&0xffff
  configured=[(a,size)for name,a,size in SPANS if name=='archive-descriptors']
  if (address,count,stride)!=(ARCHIVE_DESCRIPTORS,ARCHIVE_DESCRIPTOR_COUNT,ARCHIVE_DESCRIPTOR_BYTES) or configured!=[(address,count*stride)]:
   raise ValueError('Original signed archive descriptor address/layout changed')
  chunks=[];meta=[]
  for name,a,size in SPANS:
   raw,origins=data(a,size);h=hashlib.sha256(raw).hexdigest()
   if DIGESTS and DIGESTS.get(name)!=h:raise ValueError('Original Window span changed')
   chunks.append((a,raw));meta.append(dict(name=name,address=a,size=size,sha256=h,origins=origins))
 chunks.sort();offset=12+12*len(chunks);pack=bytearray(struct.pack('>8sI',b'PMEVT001',len(chunks)))
 for a,raw in chunks:pack+=struct.pack('>III',a,len(raw),offset);offset+=len(raw)
 pack+=b''.join(raw for a,raw in chunks)
 inv=inventory(iso)
 if any(e['path'].startswith('font/')for e in inv['files']):raise ValueError('Original absent font archive paths changed')
 return bytes(pack),dict(source='US revision0 original initialized data and loader BSS; cold process only',dol_offset=dol,bss_address=bss,bss_bytes=n,archive_dispatcher_address=0x800782e4,archive_dispatcher_bytes=0xd4,archive_dispatcher_sha256=digest,spans=meta,pack_sha256=hashlib.sha256(pack).hexdigest(),license='Original Nintendo/Intelligent Systems rights retained; no redistribution license')
def build(iso,output):
 raw,meta=read_source(iso);p=Path(output)
 if p.exists()and any(p.iterdir()):
  if {q.name for q in p.iterdir()}=={'memory.pmevt','memory.json'}and(p/'memory.pmevt').read_bytes()==raw and json.loads((p/'memory.json').read_text())==meta:return meta
  raise ValueError('New import requires empty destination; existing edits preserved')
 p.mkdir(parents=True,exist_ok=True);(p/'memory.pmevt').write_bytes(raw);(p/'memory.json').write_text(json.dumps(meta,indent=2)+'\n');return meta
def migrate_generated_cache(iso,output):
 """Replace only the exact known generated legacy pack; preserve any edits.

 The ordinary importer remains empty-destination/exact-output only. Normal
 builds explicitly opt into this migration for their ignored RomFS cache.
 """
 raw,meta=read_source(iso);p=Path(output)
 if not p.exists() or not any(p.iterdir()):return build(iso,p)
 if {q.name for q in p.iterdir()}!={'memory.pmevt','memory.json'}:return build(iso,p)
 if (p/'memory.pmevt').read_bytes()==raw and json.loads((p/'memory.json').read_text())==meta:return meta
 legacy=bytearray(raw)
 # The old pack differs only in this address row; source zero bytes and their
 # hashes are identical. Pin the entire old generated pack and metadata.
 for index in range(struct.unpack_from('>I',legacy,8)[0]):
  offset=12+12*index
  if struct.unpack_from('>I',legacy,offset)[0]==ARCHIVE_DESCRIPTORS:
   struct.pack_into('>I',legacy,offset,0x803ebff0);break
 else:raise ValueError('Missing source archive address row')
 legacy_sha='cbb8a9711aff88735eb8b2ee129f93eb8d35484a2f12ebfc1f8bca76677da8be'
 if hashlib.sha256(legacy).hexdigest()!=legacy_sha:raise ValueError('Legacy generated cache identity changed')
 legacy_meta=json.loads(json.dumps(meta));legacy_meta['pack_sha256']=legacy_sha
 for span in legacy_meta['spans']:
  if span['name']=='archive-descriptors':span['address']=0x803ebff0
 if (p/'memory.pmevt').read_bytes()!=legacy or json.loads((p/'memory.json').read_text())!=legacy_meta:
  raise ValueError('Generated cache migration preserves edited or unknown outputs')
 with tempfile.TemporaryDirectory(dir=p.parent) as temporary:
  fresh=Path(temporary)/'new';previous=Path(temporary)/'previous';build(iso,fresh)
  p.rename(previous)
  try:fresh.rename(p)
  except BaseException:
   previous.rename(p);raise
 return meta
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs/source-window-startup');p.add_argument('--migrate-generated-cache',action='store_true');a=p.parse_args();m=(migrate_generated_cache if a.migrate_generated_cache else build)(a.iso,a.output);print(m['pack_sha256'])
