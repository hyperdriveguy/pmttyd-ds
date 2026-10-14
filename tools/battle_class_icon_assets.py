#!/usr/bin/env python3
"""Read-only original class icons334..343 in a dedicated256-square sheet."""
import argparse,json,struct,subprocess,shutil
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO,extract,convert_tpl

def build(iso=DEFAULT_ISO,output='romfs',work='local-assets/battle-class-icons'):
 out=Path(output);out.mkdir(parents=True,exist_ok=True);work=Path(work);work.mkdir(parents=True,exist_ok=True)
 directory=work/'icon.bin';tpl=work/'icon.tpl';extract(iso,'icon.bin',directory);extract(iso,'icon.tpl',tpl);raw=directory.read_bytes()
 atlas=Image.new('RGBA',(256,256));icons=[];payload=bytearray()
 for index,identifier in enumerate(range(334,344)):
  offset=struct.unpack_from('>H',raw,2*(identifier+1))[0]
  loop,unused1,unused2,count=struct.unpack_from('>4H',raw,offset)
  image,delay=struct.unpack_from('>2H',raw,offset+8)
  if (loop,unused1,unused2,count,image,delay)!=(0,0,0,1,identifier+5,0):raise ValueError('Unexpected original class icon descriptor')
  path=work/f'{identifier}.png';convert_tpl(tpl,path,image);bitmap=Image.open(path).convert('RGBA')
  if bitmap.size!=(48,48):raise ValueError('Unexpected original class image size')
  x,y=(index%4)*52+2,(index//4)*52+2;atlas.paste(bitmap,(x,y));uv=(x/256,1-y/256,(x+48)/256,1-(y+48)/256)
  payload+=struct.pack('<3I',identifier,loop,1)+struct.pack('<3I4f',delay,48,48,*uv)
  icons.append(dict(id=identifier,descriptor=offset,tpl=image,width=48,height=48,delay=delay,loop=loop,uv=uv))
 atlas.save(work/'atlas.png');(out/'battle-class-icons.bin').write_bytes(struct.pack('<8sI',b'PMICON1',10)+payload)
 tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
 subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(out/'battle-class-icons.t3x'),str(work/'atlas.png')],check=True,stdout=subprocess.DEVNULL)
 (out/'battle-class-icons.json').write_text(json.dumps(dict(source=['icon.bin','icon.tpl'],icons=icons,dimensions=[256,256],texture_bytes=262144,
 conversion='Original RGBA pixels;52px cells with2px transparent padding;no resampling',license='Nintendo/Intelligent Systems resources;no redistribution rights'),indent=2)+'\n')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs');p.add_argument('--work',default='local-assets/battle-class-icons');a=p.parse_args();build(a.iso,a.output,a.work)
