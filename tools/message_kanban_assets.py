#!/usr/bin/env python3
"""Original type4 sign, with source texture CLAMP baked at canonical geometry."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO,extract,convert_tpl

def build(iso):
    work=Path('local-assets/message-kanban');work.mkdir(parents=True,exist_ok=True)
    tpl=work/'msgWindow.tpl';extract(iso,'f/msgWindow.tpl',tpl)
    fingerprint=hashlib.sha256(tpl.read_bytes()+Path(__file__).read_bytes()).hexdigest()
    meta=Path('romfs/message-kanban.json');output=Path('romfs/message-kanban.t3x')
    if meta.exists() and output.exists() and json.loads(meta.read_text()).get('fingerprint')==fingerprint:return
    images={}
    for index in (29,30,31,32):
        path=work/f'{index}.png';convert_tpl(tpl,path,index);images[index]=Image.open(path).convert('RGBA')
        if images[index].size!=((72,176) if index in (29,30) else (416,40)):raise ValueError('Original sign texture dimensions changed')
    atlas=Image.new('RGBA',(1024,256))
    atlas.paste(images[29],(0,0));atlas.paste(images[31],(72,0));atlas.paste(images[30],(488,0));atlas.paste(images[32],(72,136))
    # Source middle quad uses texture coordinates0..3.4 with CLAMP, not a
    # stretch or repeated wood texture. Preserve40 rows then clamp its edge.
    edge=images[31].crop((0,39,416,40))
    for y in range(40,136):atlas.paste(edge,(72,y))
    atlas.save(work/'atlas.png')
    tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
    subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(output),str(work/'atlas.png')],check=True,stdout=subprocess.DEVNULL)
    meta.write_text(json.dumps(dict(fingerprint=fingerprint,source='f/msgWindow.tpl',source_sha256=hashlib.sha256(tpl.read_bytes()).hexdigest(),
        indices=[29,31,30,32],dimensions=[1024,256],content=[560,176],conversion='Original pixels at canonical560x176; upper-middle40 original rows then96 copies of CLAMP last row; no resampling; transparent outside content',
        license='Nintendo/Intelligent Systems original resource; no redistribution rights'),indent=2)+'\n')
if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--iso',default=DEFAULT_ISO);build(parser.parse_args().iso)
