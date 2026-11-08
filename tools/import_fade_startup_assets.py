#!/usr/bin/env python3
"""Import original fade startup literals, pointer BSS and texture read-only."""
import argparse,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
LICENSE='Nintendo/Intelligent Systems original game asset; no redistribution license granted'
SPANS=[(2150365084, 12, '6a317bfc36d033d105f4837492b298dcfa9ffd28cf7b25918f5430df6719134c'), (2150365096, 16, 'defdcc3189ba966eaf8e1b1e675d11c37dcc591e2bff459ed177c49120d3572c'), (2151802400, 4, 'df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119'), (2151806888, 8, '35a329e9a766e1a60f366cf4599a0ecc21be601e310be696c25bb91f31cb475b')]
BODIES=[(2147564196, 236, 'c6bb7058a6a65cb509dd8fad39fbda6e2943c9d2ff76cd782d2355b31f62ad0c'), (2147564432, 276, '4d9bf07a55b5e2a997ae95ba738d0773587aac5744d4b6ed49976f14a0b56837'), (2147564708, 72, '2e610a930a107427f6da57d729d7126dba310fc846d7a40270258e866f8ad3f7')]
ASSETS=[('fade.tpl', 22720, 'ce2c99dbe41f28d5c06b777b9b0b19ed5a9fe3829f63952cae9352195fb99435')]

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
 (destination/'provenance.json').write_text(json.dumps(dict(source='US G8ME01 disc0 revision0',spans=provenance,textures=[p for _,_,p in textures],conversion='Unchanged initialized DOL literals, original BSS initialization and unrelocated original TPL. Actual fade manager must be source heap allocated at runtime. Shared SDK/root/archive/heap remain borrowed; no reimport/reset of those owners.',license=LICENSE),indent=2)+'\n')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--destination',required=True);a=p.parse_args();build(a.iso,a.destination)
