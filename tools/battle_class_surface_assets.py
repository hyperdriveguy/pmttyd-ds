#!/usr/bin/env python3
"""Original battle_common.tpl circle0/bubble1; private native atlas, no resampling."""
import argparse,hashlib,json,shutil,struct,subprocess
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO,extract,convert_tpl
SOURCE='battle/common/battle_common.tpl'
SOURCE_SIZE=217920
SOURCE_HASH='8eb685317056f42b3ef73138eaad85d16fa4943d830b44a149f09971a5959bb1'
COMMON_OUTPUT='battle-class-common.tpl'
def package_common_tpl(source,output):
 # Keep original BE offsets/flags: type4 relocation belongs to the native file
 # owner. This is an ignored compiled-pack product, not an editable-source import.
 raw=Path(source).read_bytes()
 if len(raw)!=SOURCE_SIZE or hashlib.sha256(raw).hexdigest()!=SOURCE_HASH:
  raise ValueError('Original US common battle texture changed')
 destination=Path(output)/COMMON_OUTPUT
 destination.parent.mkdir(parents=True,exist_ok=True)
 destination.write_bytes(raw)
 return dict(path=COMMON_OUTPUT,size=len(raw),sha256=SOURCE_HASH,
  conversion='Exact original TPL bytes; no decoding, relocation or resampling')
def build(iso=DEFAULT_ISO,output='romfs',work='local-assets/battle-class-ui'):
 out=Path(output);out.mkdir(parents=True,exist_ok=True);work=Path(work);work.mkdir(parents=True,exist_ok=True)
 tpl=work/'battle_common.tpl';extract(iso,SOURCE,tpl)
 common=package_common_tpl(tpl,out)
 atlas=Image.new('RGBA',(256,128));payload=bytearray(struct.pack('<8sI',b'PMBCLS01',2));records=[]
 for index,(width,height,x,y) in enumerate([(40,40,2,2),(128,80,46,2)]):
  image=work/f'common-{index}.png';convert_tpl(tpl,image,index);bitmap=Image.open(image).convert('RGBA')
  if bitmap.size!=(width,height):raise ValueError('Original class surface dimensions changed')
  # Original wrap0/min-mag1: extrude clamp texels, never transparent seams.
  for dx in range(-2,width+2):
   for dy in range(-2,height+2):
    atlas.putpixel((x+dx,y+dy),bitmap.getpixel((min(width-1,max(0,dx)),min(height-1,max(0,dy)))))
  uv=(x/256,1-y/128,(x+width)/256,1-(y+height)/128)
  payload+=struct.pack('<3I4f',index,width,height,*uv)
  records.append(dict(index=index,width=width,height=height,x=x,y=y,uv=uv,pixel_sha256=hashlib.sha256(bitmap.tobytes()).hexdigest()))
 atlas.save(work/'atlas.png')
 tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
 subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(out/'battle-class-surfaces.t3x'),str(work/'atlas.png')],check=True,stdout=subprocess.DEVNULL)
 (out/'battle-class-surfaces.bin').write_bytes(payload)
 metadata=dict(source=SOURCE,source_sha256=SOURCE_HASH,common_tpl=common,dimensions=[256,128],texture_bytes=131072,images=records,
  conversion='Original RGBA pixels;2px clamp-edge extrusion;source interiors/catalogUV unchanged;no resampling;circle quadrants and bubble geometry remain source draw consumers',
  license='Nintendo/Intelligent Systems resources;no redistribution rights')
 (out/'battle-class-surfaces.json').write_text(json.dumps(metadata,indent=2)+'\n');return metadata
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs');p.add_argument('--work',default='local-assets/battle-class-ui');a=p.parse_args();build(a.iso,a.output,a.work)
