#!/usr/bin/env python3
"""Retain original US demo page deformation and source material owners.

The original paper's captured texturekind1 is explicitly metadata, not silently
admitted through the ordinary PMANIM4 texture driver. Native rendering supplies
its captured image; the fallback TPL is not a replacement for that capture.
"""
import argparse,hashlib,json,struct,shutil,subprocess
from PIL import Image
from pathlib import Path
from iso_assets import DEFAULT_ISO,extract,convert_tpl
from character_animation_pack import compile_animation_pack
from character_animation_data import animation
from character_bake import geometry,u

def build(iso=DEFAULT_ISO,out=Path('romfs')):
    out=Path(out);out.mkdir(parents=True,exist_ok=True)
    work=Path('local-assets/demo-book');work.mkdir(parents=True,exist_ok=True)
    model=work/'OFF_d_meku_right';tpl=work/'OFF_d_meku_right.tpl'
    extract(iso,'a/OFF_d_meku_right',model);extract(iso,'a/OFF_d_meku_right-',tpl)
    raw=model.read_bytes();digest=hashlib.sha256(raw).hexdigest()
    if digest!='096736b5a0277b4334d8f2c32038562f9c8dcd2ed9d925cff0aa03f978fbbfd7':raise ValueError('Original page resource changed')
    anim=animation(raw,'A_1')
    if anim['loop'] or (anim['start'],anim['end'],len(anim['frames']))!=(0,180,181):raise ValueError('Original page animation changed')
    if len(anim['texture_indices'])!=2 or anim['textures']!=[[0,0,1,1,0]]*2 or any(k['texture'] or k['visibility'] for k in anim['frames']):raise ValueError('Additional page material service required')
    kinds=[]
    for binding,state in enumerate(anim['texture_indices']):
        index=u(raw,u(raw,0x194)+binding*8)+state
        kinds.append(u(raw,u(raw,0x198)+index*64+8))
    if kinds!=[0,1]:raise ValueError('Source paper capture owner changed')
    # Keep distinct cell0/1 ownership. The renderer undoes the ordinary cell UV
    # inset and binds source capture only for original kind1 triangles.
    compile_animation_pack([(model,tpl,['A_1'])],out/'demo-page.pmanim',lambda model,tpl,index:index)
    triangles=geometry(model,'A_1',bindings=True,surfaces=True)
    records=[]
    for binding,triangle in triangles:
        capture=kinds[binding]
        _,_,_,_,color,mode,cull=triangle[0]
        if mode!=2 or cull>3 or any(v[3]!=binding or v[4]!=(255,255,255,255) or v[5:7]!=(mode,cull) for v in triangle):raise ValueError('Source page surface owner changed')
        records.append((capture,mode,cull))
    with (out/'demo-page.bin').open('wb') as file:
        file.write(struct.pack('<8sI',b'PMDPG001',len(records)))
        for row in records:file.write(struct.pack('<3I',*row))
    png=work/'back.png';convert_tpl(tpl,png,0)
    image=Image.open(png).convert('RGBA');width,height=image.size
    if width>512 or height>512:image=image.resize((width//2,height//2),Image.Resampling.LANCZOS);width,height=image.size
    canvas=Image.new('RGBA',(1<<(width-1).bit_length(),1<<(height-1).bit_length()));canvas.paste(image,(0,0));canvas.save(png)
    tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
    subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(out/'demo-page-back.t3x'),str(png)],check=True,stdout=subprocess.DEVNULL)
    (out/'demo-page-back.size').write_bytes(struct.pack('<4I',width,height,canvas.width,canvas.height))
    metadata=dict(source='a/OFF_d_meku_right',sha256=digest,animation='A_1',span=[0,180],loop=False,keys=181,updates=anim['stats'],triangles=len(records),materials=[dict(kind=0,owner='original TPL index0'),dict(kind=1,owner='original framebuffer capture')],capture=[124,0,386,480],capture_images=[8,9,10,11],image_depths=[6,5,4,3],uv_inverse='u=(1024*u-cell*128-2)/124;v=((1-v)*1024-2)/124 for cells0/1,row0',limitation='Original sparse deformation retained; framebuffer capture, texture substitution, image placement/projection, completion publication and paper material services require native binding.',license='Original Nintendo/Intelligent Systems assets for local use; no redistribution rights')
    (out/'demo-page.json').write_text(json.dumps(metadata,indent=2)+'\n')
    return metadata

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs');a=p.parse_args();meta=build(a.iso,a.output);print(f"Original page A_1 retained: {meta['triangles']} triangles,181keys,span180")
if __name__=='__main__':main()
