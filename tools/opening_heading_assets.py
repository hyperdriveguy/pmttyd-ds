#!/usr/bin/env python3
"""Export exact US sys heading glyphs/backgrounds and its original center curtain."""
import argparse,hashlib,json,shutil,struct,subprocess
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO,extract,convert_tpl
from character_animation_data import animation
from character_surface_mesh import compile_surface_mesh

def build(iso=DEFAULT_ISO,output='romfs'):
    out=Path(output);out.mkdir(parents=True,exist_ok=True);work=Path('local-assets/heading');work.mkdir(parents=True,exist_ok=True)
    fingerprint=hashlib.sha256(Path(__file__).read_bytes()+b''.join(Path('tools',n).read_bytes() for n in ('iso_assets.py','character_surface_mesh.py','character_animation_pack.py','character_animation_data.py','character_bake.py'))+str((Path(iso).stat().st_size,Path(iso).stat().st_mtime_ns)).encode()).hexdigest()
    meta=out/'heading-resources.json';names=[f'heading-{i}' for i in [0,1,2,3,19,20,21,22,23,24,25]]
    required=[n+ext for n in names for ext in ('.t3x','.size')]+['heading-curtain.pmanim','heading-curtain.t3x','heading-curtain.bin']
    if meta.exists() and json.loads(meta.read_text()).get('fingerprint')==fingerprint and all((out/n).is_file() for n in required):return
    tpl=work/'effect_sys.tpl';extract(iso,'e/us/effect_sys.tpl',tpl)
    model=work/'OFF_d_maku_center';curtain=work/'OFF_d_maku_center.tpl'
    extract(iso,'a/OFF_d_maku_center',model);extract(iso,'a/OFF_d_maku_center-',curtain)
    tool=shutil.which('tex3ds')or'/opt/devkitpro/tools/bin/tex3ds'
    for index in [0,1,2,3,19,20,21,22,23,24,25]:
        name=f'heading-{index}';png=work/(name+'.png');convert_tpl(tpl,png,index)
        image=Image.open(png).convert('RGBA');w,h=image.size
        if (w,h)!=((160,160) if index<4 else (64,64)):raise ValueError('Original heading raster dimensions changed')
        padded=Image.new('RGBA',(1<<(w-1).bit_length(),1<<(h-1).bit_length()));padded.paste(image,(0,0));padded.save(png)
        subprocess.run([tool,'-f','rgba8','-z','auto','-o',str(out/(name+'.t3x')),str(png)],check=True,stdout=subprocess.DEVNULL)
        (out/(name+'.size')).write_bytes(struct.pack('<4I',w,h,padded.width,padded.height))
    for pose,span,count in [('Z_1',0,1),('A_1',60,121)]:
        original=animation(model.read_bytes(),pose)
        if original['loop'] or original['start']!=0 or original['end']!=span or len(original['frames'])!=count:raise ValueError('Original heading curtain pose changed')
    mesh=compile_surface_mesh([(model,curtain,['Z_1','A_1'])],out/'heading-curtain.pmanim',work/'heading-curtain.png')
    image=Image.open(work/'heading-curtain.png');width,height=image.size
    image.resize((width//2,height//2),Image.Resampling.LANCZOS).save(work/'heading-curtain.png')
    subprocess.run([tool,'-f','rgba8','-z','auto','-o',str(out/'heading-curtain.t3x'),str(work/'heading-curtain.png')],check=True,stdout=subprocess.DEVNULL)
    (out/'heading-curtain.bin').write_bytes(struct.pack('<8sfII',b'PMHEDC01',width/height,2,mesh['textures']))
    sources=[json.loads(p.with_suffix(p.suffix+'.provenance.json').read_text())for p in (tpl,model,curtain)]
    meta.write_text(json.dumps(dict(fingerprint=fingerprint,sources=sources,backgrounds=[0,1,2,3],glyph_order=[19,20,21,22,21,23,24,25],subtitle_id='msg_stg_seq_prolouge',curtain=mesh,conversion='Exact original heading rasters with transparent power-of-two padding; original Z_1/A_1 curtain hierarchy/materials/poses, half-size Lanczos atlas; no replacement art or text',license='Original Nintendo/Intelligent Systems assets for private local use; no redistribution rights'),indent=2)+'\n')
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs');a=p.parse_args();build(a.iso,a.output)
