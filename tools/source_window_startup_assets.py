#!/usr/bin/env python3
"""Original cold window/archive state and literals; not WindowReady output."""
import argparse,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
SPANS=[('TEX-unsigned-bias-and-panic',0x802c0140,0x48),('window-source-paths',0x802c3080,0x28),('archive-descriptors',0x803ebff0,160),('window-source-globals',0x8041ea18,16)]
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
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs/source-window-startup');a=p.parse_args();m=build(a.iso,a.output);print(m['pack_sha256'])
