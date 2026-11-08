#!/usr/bin/env python3
"""Import original Logo work, mode records, path and title TPL read-only."""
import argparse,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
LICENSE='Nintendo/Intelligent Systems original game asset; no redistribution license granted'
SPANS=[(2151768328, 4, 'e7f01a429c356dee8dfc87ebbdc8f5b42ff043894470b8040d6c79553a4f609b'), (2151803520, 32, '66687aadf862bd776c8fc18b8e9f8e20089714856ee233b3902a591d0d5f2925'), (2150382440, 16, 'a5515982f6734fd70ab2e935a5a14ba236d72e03417a57d094c74d95c5dc5c63'), (2150679088, 60, '62d87f5a2e00870f6a0904c78e3e8235c63ca518a9a7f265449ab076a829cb8d'), (2150679148, 60, 'f9d4282efa8ce3f7502091fb7ebf63f0194e39015ac3810ec8738b8a07d82e2c')]
BODIES=[(2148037808, 396, 'f48865f05382fae7aba7ffe8d636f0d27ffefee61b7232a76c06317989903860'), (2148037740, 68, '54b7691885ff90c84e987575559e093c599c4c125b11f62e9cc6358bb2da3500'), (2147859024, 152, '08c3dce0e69c0c1e23422a3dbdfb3aabe5fc117761965731e76f5c30260f304a')]
ASSETS=[('mariost.tpl', 2128288, 'e6916a8a208b5de452ab49ec7381b5540b1ece86f919575f3beb4508caf402cb')]

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
 (destination/'provenance.json').write_text(json.dumps(dict(source='US G8ME01 disc0 revision0',spans=provenance,textures=[p for _,_,p in textures],conversion='Unchanged initialized Logo work pointer, path and VI mode inputs, original work BSS initialization and unrelocated title TPL. Shared SDK/root/GP/cache and raw Mario remain borrowed; no reimport/reset of those owners.',license=LICENSE),indent=2)+'\n')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--destination',required=True);a=p.parse_args();build(a.iso,a.destination)
