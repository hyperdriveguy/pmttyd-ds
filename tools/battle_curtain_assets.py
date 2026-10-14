#!/usr/bin/env python3
"""Retain original US fade23 curtain A_2; local assets only, ISO read-only."""
import argparse,hashlib,json,shutil,struct,subprocess
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO,extract
from character_animation_data import animation
from character_surface_mesh import compile_surface_mesh

def build(iso=DEFAULT_ISO,out='romfs'):
    out=Path(out);out.mkdir(parents=True,exist_ok=True)
    work=Path('local-assets/harbor-curtain');work.mkdir(parents=True,exist_ok=True)
    model=work/'OFF_d_maku_left_right';tpl=model.with_suffix('.tpl')
    extract(iso,'a/OFF_d_maku_left_right',model);extract(iso,'a/OFF_d_maku_left_right-',tpl)
    helpers=['iso_assets.py','character_surface_mesh.py','character_animation_pack.py','character_animation_data.py','character_bake.py']
    fingerprint=hashlib.sha256(Path(__file__).read_bytes()+model.read_bytes()+tpl.read_bytes()+b''.join(Path('tools',n).read_bytes()for n in helpers)).hexdigest()
    meta=out/'harbor-curtain.json'
    if meta.exists() and json.loads(meta.read_text()).get('fingerprint')==fingerprint and all((out/('harbor-curtain'+e)).is_file()for e in ['.pmanim','.t3x','.bin']):return
    pose=animation(model.read_bytes(),'A_2')
    if pose['loop'] or pose['start']!=0 or pose['end']!=45 or len(pose['frames'])!=14:raise ValueError('Original US curtain A_2 changed')
    mesh=compile_surface_mesh([(model,tpl,['A_2'])],out/'harbor-curtain.pmanim',work/'atlas.png')
    if mesh['textures']!=1:raise ValueError('Curtain texture ownership changed')
    atlas=Image.open(work/'atlas.png');width,height=atlas.size
    atlas.resize((width//2,height//2),Image.Resampling.LANCZOS).save(work/'atlas.png')
    tool=shutil.which('tex3ds')or'/opt/devkitpro/tools/bin/tex3ds'
    subprocess.run([tool,'-f','rgba8','-z','auto','-o',str(out/'harbor-curtain.t3x'),str(work/'atlas.png')],check=True,stdout=subprocess.DEVNULL)
    (out/'harbor-curtain.bin').write_bytes(struct.pack('<8sfIff',b'PMCUR001',width/height,216,0,45))
    meta.write_text(json.dumps(dict(fingerprint=fingerprint,source='a/OFF_d_maku_left_right',texture='a/OFF_d_maku_left_right-',model_sha256=hashlib.sha256(model.read_bytes()).hexdigest(),animation='A_2',context=2,span=[0,45],keys=14,triangles=216,uvScale=width/height,conversion='Original sparse hierarchy/geometry/UV/pass/cull/RGBA; original wrapped atlas downscaled512x64. Source pass0 normalized to alpha-test pass2 by shared converter. Native image-plane projection remains an adapter.',license='Original Nintendo/Intelligent Systems; local use only, no redistribution rights'),indent=2)+'\n')

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs');a=p.parse_args();build(a.iso,a.output)
