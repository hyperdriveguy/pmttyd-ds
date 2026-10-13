#!/usr/bin/env python3
"""Compile original field map-object banks locally for the native renderer."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
from iso_assets import DEFAULT_ISO,extract
from character_surface_mesh import compile_surface_mesh


def compile_object(iso,asset,name,poses):
    work=Path('local-assets/map-objects');work.mkdir(parents=True,exist_ok=True)
    model=work/name;tpl=work/(name+'.tpl')
    extract(iso,'a/'+name,model)
    bank=model.read_bytes()[0x44:0x84].split(b'\0')[0].decode('ascii')
    extract(iso,'a/'+bank+'-',tpl)
    helpers=['character_surface_mesh.py','character_animation_pack.py','character_animation_data.py','character_bake.py','character_node.py','iso_assets.py']
    fingerprint=hashlib.sha256(Path(__file__).read_bytes()+b''.join(Path('tools',p).read_bytes() for p in helpers)+
                               model.read_bytes()+tpl.read_bytes()+json.dumps(poses).encode()).hexdigest()
    manifest=Path('romfs/'+asset+'.json');animation=Path('romfs/'+asset+'.pmanim');texture=Path('romfs/'+asset+'.t3x')
    if not(manifest.exists() and animation.exists() and texture.exists() and json.loads(manifest.read_text()).get('fingerprint')==fingerprint):
        result=compile_surface_mesh([(model,tpl,poses)],animation,work/(asset+'.png'))
        tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
        subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(texture),str(work/(asset+'.png'))],check=True,stdout=subprocess.DEVNULL)
        result.update(fingerprint=fingerprint,assets=[json.loads(p.read_text()) for p in [model.with_suffix('.provenance.json'),tpl.with_suffix('.tpl.provenance.json')]],
                      license='Original Nintendo/Intelligent Systems resources, local conversion only; no redistribution rights')
        manifest.write_text(json.dumps(result,indent=2)+'\n')
    return json.loads(manifest.read_text())['uv_scale']


def build(iso):return compile_object(iso,'field-saveblock','MOBJ_SaveBlock',['S_1','A_1'])

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--iso',default=DEFAULT_ISO)
    build(parser.parse_args().iso)
