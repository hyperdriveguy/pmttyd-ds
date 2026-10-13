#!/usr/bin/env python3
"""Compile original Professor Frankly animation/texture resources locally."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import struct
import subprocess
from PIL import Image
from iso_assets import DEFAULT_ISO, extract
from character_mesh import compile_mesh
from character_surface_mesh import compile_surface_mesh

def build(iso):
    work=Path('local-assets/professor');work.mkdir(parents=True,exist_ok=True)
    model=work/'c_kuri_h';tpl=work/'c_kuri_h.tpl'
    extract(iso,'a/c_kuri_h',model);extract(iso,'a/c_kuri_h-',tpl)
    bank=model.read_bytes()[0x44:0x84].split(b'\0')[0].decode('ascii')
    if bank!='c_kuri_h':raise ValueError('Unexpected Frankly texture bank')
    helpers=['character_mesh.py','character_surface_mesh.py','character_bake.py','character_node.py','character_animation_pack.py','character_animation_data.py','iso_assets.py']
    fingerprint=hashlib.sha256(Path(__file__).read_bytes()+b''.join(Path('tools',p).read_bytes() for p in helpers)+model.read_bytes()+tpl.read_bytes()).hexdigest()
    manifest=Path('romfs/professor.json')
    required=[Path('romfs/professor.'+suffix) for suffix in ['pmmesh','pmanim','t3x','bin']]
    if manifest.exists() and all(p.exists() for p in required) and json.loads(manifest.read_text()).get('fingerprint')==fingerprint:return
    poses=['Z_1','S_1','T_1','W_1','R_1','D_1']
    directory=struct.unpack_from('>I',model.read_bytes(),0x1ac)[0]
    zero=model.read_bytes()[directory:directory+60].split(b'\0')[0].decode('ascii')
    if poses[0]!=zero:raise ValueError('Original descriptor0 must lead the native NPC bank')
    compile_mesh([(model,tpl,poses)],required[0],work/'legacy.png')
    result=compile_surface_mesh([(model,tpl,poses)],required[1],work/'mesh.png')
    height=128
    while height<((result['textures']+7)//8)*128:height*=2
    Image.open(work/'mesh.png').crop((0,0,1024,height)).save(work/'runtime.png')
    required[3].write_bytes(struct.pack('<8sf',b'PMPNPC1',1024/height))
    tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
    subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(required[2]),str(work/'runtime.png')],check=True,stdout=subprocess.DEVNULL)
    result.update(fingerprint=fingerprint,atlas_height=height,uv_scale=1024/height,assets=[json.loads(p.read_text()) for p in [model.with_suffix('.provenance.json'),tpl.with_suffix('.tpl.provenance.json')]],
                  original_index0=zero,
                  tribe_pose_fallback='Case-sensitive DOL tribe d_1 misses bank D_1 and selects source descriptor0 Z_1',
                  license='Local original Nintendo/Intelligent Systems assets; no redistribution rights')
    manifest.write_text(json.dumps(result,indent=2)+'\n')
    print('Original Frankly animation poses:',len(poses))

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO)
    build(p.parse_args().iso)
