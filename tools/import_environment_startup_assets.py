#!/usr/bin/env python3
"""Import original environment startup storage, literals and texture read-only."""
import argparse,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
LICENSE='Nintendo/Intelligent Systems original game asset; no redistribution license granted'
SPANS=[(2150603896, 8, 'be6137e557bf04ddf841fd5d8ce657ce4132dbf30371191c625528cd6b0ac986'), (2150603904, 12, '0725a3294ef205d82d8facd785033bb73c433eaf4bdd527ded92b53eb610d4df'), (2151804048, 8, 'af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc'), (2151576184, 480, '4b48f21a4b7a02bfbec19ef880a967a02334a3cdcef8ae83de2ef327ba8bc5dd')]
BODIES=[(2149179268, 212, '029c1a106da0972f86555a2f5c500116ceb848b76b174ee6ff5e28074d1c4e36'), (2149179548, 84, '42f70180a6bdf676cbcbe9935ccd4fe353c7538b0862ced2e21cef8756e98cdb'), (2149179632, 64, '8c485d49121a487fa3b466b9265f35bd30877273886e738a289ed008b98ccb65')]
ASSETS=[('env.tpl', 37248, '365bce8eb8114c72636429a3cf32910594fa3ffdcdd8c84810343b3cba967744')]

def build(iso,destination):
 destination=Path(destination)
 if destination.exists() and (not destination.is_dir() or any(destination.iterdir())):raise ValueError('Import destination must be empty')
 segments=[];provenance=[];textures=[]
 files={e['path']:e for e in inventory(iso)['files']}
 with Path(iso).open('rb') as f:
  f.seek(0x420);dol,=struct.unpack('>I',f.read(4));f.seek(dol);h=f.read(256)
  bss,bss_size=struct.unpack_from('>II',h,0xd8)
  sections=[tuple(struct.unpack_from('>I',h,k+4*i)[0] for k in (0,0x48,0x90)) for i in range(18)]
  for address,size,sha in BODIES+SPANS:
   offsets=[dol+off+address-base for off,base,n in sections if base<=address and address+size<=base+n]
   if len(offsets)==1:
    f.seek(offsets[0]);data=f.read(size);offset=offsets[0];origin='initialized DOL'
   elif not offsets and bss<=address and address+size<=bss+bss_size:
    data=bytes(size);offset=None;origin='original DOL BSS zero initialization'
   else:raise ValueError('Source mapping differs')
   if len(data)!=size or hashlib.sha256(data).hexdigest()!=sha:raise ValueError('Original source differs')
   if (address,size,sha) in SPANS:segments.append(dict(address=address,bytes=data.hex()))
   provenance.append(dict(address=hex(address),size=size,sha256=sha,iso_offset=offset,source=origin,license=LICENSE))
  for path,size,sha in ASSETS:
   item=files[path];f.seek(item['offset']);data=f.read(item['size'])
   if len(data)!=size or hashlib.sha256(data).hexdigest()!=sha:raise ValueError('Original texture differs')
   textures.append((Path(path).name,data,dict(path=path,size=size,sha256=sha,iso_offset=item['offset'],license=LICENSE)))
 destination.mkdir(parents=True,exist_ok=True)
 (destination/'memory.json').write_text(json.dumps(dict(schema=1,segments=segments,license=LICENSE),indent=2)+'\n')
 for name,data,_ in textures:(destination/name).write_bytes(data)
 (destination/'provenance.json').write_text(json.dumps(dict(source='US G8ME01 disc0 revision0',spans=provenance,textures=[p for _,_,p in textures],conversion='Unchanged initialized DOL literals, original BSS initialization and unrelocated original TPL. Shared SDK/root/archive/heap remain borrowed; no reimport/reset of those owners.',license=LICENSE),indent=2)+'\n')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--destination',required=True);a=p.parse_args();build(a.iso,a.destination)
