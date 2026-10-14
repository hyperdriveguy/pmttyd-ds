#!/usr/bin/env python3
"""Decode original type7 diary pixels without resampling; local outputs only."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO,extract,convert_tpl

def build(iso):
    work=Path('local-assets/message-diary');work.mkdir(parents=True,exist_ok=True)
    tpl=work/'msgWindow.tpl';extract(iso,'f/msgWindow.tpl',tpl)
    fingerprint=hashlib.sha256(tpl.read_bytes()+Path(__file__).read_bytes()).hexdigest()
    meta=Path('romfs/message-diary.json');output=Path('romfs/message-diary.t3x')
    source=work/'6.png';atlas_path=work/'atlas.png'
    if all(p.exists() for p in (meta,output,source,atlas_path)) and json.loads(meta.read_text()).get('fingerprint')==fingerprint:return
    convert_tpl(tpl,source,6)
    with Image.open(source) as image:
        if image.size!=(560,176):raise ValueError('Original diary dimensions changed')
        pixels=image.convert('RGBA');atlas=Image.new('RGBA',(1024,256));atlas.paste(pixels,(1,1))
        # One-texel gutters preserve source CLAMP under the atlas' linear filter.
        atlas.paste(pixels.crop((0,0,1,176)),(0,1));atlas.paste(pixels.crop((559,0,560,176)),(561,1))
        atlas.paste(pixels.crop((0,0,560,1)),(1,0));atlas.paste(pixels.crop((0,175,560,176)),(1,177))
        for x,y,sx,sy in [(0,0,0,0),(561,0,559,0),(0,177,0,175),(561,177,559,175)]:atlas.putpixel((x,y),pixels.getpixel((sx,sy)))
        atlas.save(atlas_path)
    tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
    subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(output),str(atlas_path)],check=True,stdout=subprocess.DEVNULL)
    meta.write_text(json.dumps(dict(fingerprint=fingerprint,source='f/msgWindow.tpl',source_sha256=hashlib.sha256(tpl.read_bytes()).hexdigest(),
        indices=[6],dimensions=[1024,256],content=[560,176],origin=[1,1],gutter=1,pixel_sha256=hashlib.sha256(atlas.tobytes()).hexdigest(),
        conversion='Original TPL6 CMPR decoded to RGBA8 at canonical560x176; no resampling; one-pixel CLAMP gutters then transparent atlas padding; original CLAMP/linear sampler',
        license='Nintendo/Intelligent Systems original resource; no redistribution rights'),indent=2)+'\n')
if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--iso',default=DEFAULT_ISO);build(parser.parse_args().iso)
