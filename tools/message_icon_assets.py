#!/usr/bin/env python3
"""Import used original message/selection icons without replacing their pixels."""
import argparse,json,struct,subprocess,shutil
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO,extract,convert_tpl
def build(iso):
    work=Path('local-assets/message-icons');work.mkdir(parents=True,exist_ok=True)
    directory=work/'icon.bin';tpl=work/'icon.tpl';extract(iso,'icon.bin',directory);extract(iso,'icon.tpl',tpl)
    raw=directory.read_bytes();atlas=Image.new('RGBA',(512,256));x=y=row=0;icons=[];payload=bytearray()
    for identifier in (129,504,115):
        offset=struct.unpack_from('>H',raw,2*(identifier+1))[0];loop,_,_,count=struct.unpack_from('>4H',raw,offset)
        payload+=struct.pack('<3I',identifier,loop,count);frames=[]
        for i in range(count):
            image,delay=struct.unpack_from('>2H',raw,offset+8+4*i);path=work/f'{image}.png';convert_tpl(tpl,path,image)
            bitmap=Image.open(path).convert('RGBA');w,h=bitmap.size
            if x+w+4>512:x=0;y+=row;row=0
            if y+h+4>256:raise ValueError('Used icon atlas capacity exceeded')
            atlas.paste(bitmap,(x+2,y+2));uv=((x+2)/512,1-(y+2)/256,(x+2+w)/512,1-(y+2+h)/256)
            payload+=struct.pack('<3I4f',delay,w,h,*uv);frames.append(dict(tpl=image,delay=delay,width=w,height=h,uv=uv))
            x+=w+4;row=max(row,h+4)
        icons.append(dict(id=identifier,loop=loop,descriptor=offset,frames=frames))
    atlas.save(work/'atlas.png');Path('romfs/message-icons.bin').write_bytes(struct.pack('<8sI',b'PMICON1',len(icons))+payload)
    tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
    subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o','romfs/message-icons.t3x',str(work/'atlas.png')],check=True,stdout=subprocess.DEVNULL)
    Path('romfs/message-icons.json').write_text(json.dumps(dict(source=['icon.bin','icon.tpl'],icons=icons,
        conversion='Original pixels; shelf atlas with2px transparent padding; no resampling',license='Nintendo/Intelligent Systems resources; no redistribution rights'),indent=2)+'\n')
if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);build(p.parse_args().iso)
