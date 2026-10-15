#!/usr/bin/env python3
"""Original common-bank surfaces2/3; private empty output, no resampling."""
import argparse
import hashlib
import json
import shutil
import struct
import subprocess
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO,extract,convert_tpl
from battle_class_surface_assets import SOURCE,SOURCE_HASH,SOURCE_SIZE

def build(iso=DEFAULT_ISO,output='local-assets/battle-submenu-output',work='local-assets/battle-submenu-work'):
    out=Path(output);cache=Path(work)
    if out.exists() and any(out.iterdir()):raise ValueError('New imports require an empty destination')
    if cache.exists() and any(cache.iterdir()):raise ValueError('New extraction requires an empty private cache')
    out.mkdir(parents=True,exist_ok=True);cache.mkdir(parents=True,exist_ok=True)
    tpl=cache/'battle_common.tpl';extract(iso,SOURCE,tpl);raw=tpl.read_bytes()
    if len(raw)!=SOURCE_SIZE or hashlib.sha256(raw).hexdigest()!=SOURCE_HASH:raise ValueError('Original common bank changed')
    atlas=Image.new('RGBA',(32,32));payload=bytearray(struct.pack('<8sI',b'PMBSUB01',2));rows=[]
    for index,width,x in [(2,16,2),(3,8,22)]:
        png=cache/f'common-{index}.png';convert_tpl(tpl,png,index);image=Image.open(png).convert('RGBA')
        if image.size!=(width,24):raise ValueError('Original submenu surface dimensions changed')
        # Source sampler clamps and filters linearly. Replicate edge texels
        # into padding so UV endpoints never blend against transparent gutters.
        for dx in range(-2,width+2):
            for dy in range(-2,26):
                atlas.putpixel((x+dx,2+dy),image.getpixel((min(width-1,max(0,dx)),min(23,max(0,dy)))))
        uv=(x/32,1-2/32,(x+width)/32,1-26/32)
        payload+=struct.pack('<3I4f',index,width,24,*uv)
        rows.append(dict(index=index,width=width,height=24,x=x,y=2,uv=uv,pixel_sha256=hashlib.sha256(image.tobytes()).hexdigest()))
    atlas.save(cache/'atlas.png')
    tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
    subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(out/'battle-submenu-surfaces.t3x'),str(cache/'atlas.png')],check=True,stdout=subprocess.DEVNULL)
    (out/'battle-submenu-surfaces.bin').write_bytes(payload)
    meta=dict(source=SOURCE,source_sha256=SOURCE_HASH,dimensions=[32,32],texture_bytes=4096,images=rows,
        conversion='Original CMPR decoded RGBA pixels; two-pixel clamp-edge extrusion; source pixels/catalog UV unchanged; no resizing or geometry replacement',
        license='Nintendo/Intelligent Systems original resources; no redistribution rights granted')
    (out/'battle-submenu-surfaces.json').write_text(json.dumps(meta,indent=2)+'\n');return meta
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',required=True);p.add_argument('--work',required=True);a=p.parse_args();build(a.iso,a.output,a.work)
