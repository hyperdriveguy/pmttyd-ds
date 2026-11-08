#!/usr/bin/env python3
"""Import unchanged original effect primary/N64 banks and literals read-only."""
import argparse,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
LICENSE='Nintendo/Intelligent Systems original game asset; no redistribution license granted'
SPANS=[(2150375224, 12, '24afd76928944fec9d69a497df324615dd93382e1a0d3d048876b2eef42b299e'), (2150375236, 20, '5b3c88f17debaf3e6b2ea8563f7d95728a175456b0522cb81d58d9d789705dcf'), (2150611708, 16, '20f9c2402e67a2409c92f7b1502ab5876c69338b828aac54efec1c67bd665f4c'), (2150611724, 24, 'd84fec1829b11cbf3ff74bf387f369352dc82659434fb92653b667c81e9f7ae5'), (2151832328, 8, '3a19211ee0572847b651041e7e94133f8e27a5d403d6691a3e61e28342dd9dab')]
BODIES=[(2147872268, 276, 'e8a424af988969ead9221bfdba948bdf01400eba90828dd88db6afdb36f452ff'), (2147872680, 72, '015ce1ac69ea4997c9aa95f0780c7da5ba0de893095b1969dfcf8aa331b62886'), (2149415184, 272, '3f4919a9fa6a18a1ccb3ae7facb84121bd3f3e6c38e89cfb8f3357b52222459c'), (2149415512, 72, 'dd6c6f3548262d3f4d8b58b0c2ad639765961c6528e08e9bb54ebb67d910d448'), (2148037740, 68, '54b7691885ff90c84e987575559e093c599c4c125b11f62e9cc6358bb2da3500'), (2147975908, 212, '988c2090fbce7356e08ed4b5714683f8b7026669f3f6232e7767bce04b7166fa'), (2148814308, 52, '4c478666e9adfaa95c0af8028fac451f8b1a6b00d567054312e501f559ac2495'), (2148814292, 16, '94db90a29f286b23aa565f38431131e6cd4dd9b2b341a852cdff146164fad6b3')]
ASSETS=[('e/us/effect.tpl', 266784, '3b62850fe75afb95eebb9fc240e20204d5ed320fba0fc956729f28405c2c789b'), ('e/us/effect_n64.tpl', 235776, '3202ac8f437a2a7664d791ec5d9ee1988504f51b912c1211429708fac0a268ef')]

def build(iso,destination):
 destination=Path(destination)
 if destination.exists() and (not destination.is_dir() or any(destination.iterdir())):raise ValueError('Import destination must be empty')
 segments=[];provenance=[];textures=[]
 files={e['path']:e for e in inventory(iso)['files']}
 with Path(iso).open('rb') as f:
  f.seek(0x420);dol,=struct.unpack('>I',f.read(4));f.seek(dol);h=f.read(256)
  sections=[tuple(struct.unpack_from('>I',h,k+4*i)[0] for k in (0,0x48,0x90)) for i in range(18)]
  for address,size,sha in BODIES+SPANS:
   offsets=[dol+off+address-base for off,base,n in sections if base<=address and address+size<=base+n]
   if len(offsets)!=1:raise ValueError('Source mapping differs')
   f.seek(offsets[0]);data=f.read(size)
   if len(data)!=size or hashlib.sha256(data).hexdigest()!=sha:raise ValueError('Original source differs')
   if (address,size,sha) in SPANS:segments.append(dict(address=address,bytes=data.hex()))
   provenance.append(dict(address=hex(address),size=size,sha256=sha,iso_offset=offsets[0],license=LICENSE))
  for path,size,sha in ASSETS:
   item=files[path];f.seek(item['offset']);data=f.read(item['size'])
   if len(data)!=size or hashlib.sha256(data).hexdigest()!=sha:raise ValueError('Original texture differs')
   textures.append((Path(path).name,data,dict(path=path,size=size,sha256=sha,iso_offset=item['offset'],license=LICENSE)))
 destination.mkdir(parents=True,exist_ok=True)
 (destination/'memory.json').write_text(json.dumps(dict(schema=1,segments=segments,license=LICENSE),indent=2)+'\n')
 for name,data,_ in textures:(destination/name).write_bytes(data)
 (destination/'provenance.json').write_text(json.dumps(dict(source='US G8ME01 disc0 revision0',spans=provenance,textures=[p for _,_,p in textures],conversion='Unchanged initialized DOL literals and unrelocated original TPL bytes. Shared state/BSS and DVD root are borrowed, never reimported.',license=LICENSE),indent=2)+'\n')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--destination',required=True);a=p.parse_args();build(a.iso,a.destination)
