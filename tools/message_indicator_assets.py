#!/usr/bin/env python3
"""Import the original hoshi_2/HOS_C_1 textbox indicator, local-only."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
from PIL import Image
from iso_assets import DEFAULT_ISO, extract
from character_mesh import compile_mesh


def build(iso):
    work=Path('local-assets/message-indicator');work.mkdir(parents=True,exist_ok=True)
    model=work/'hoshi_2';tpl=work/'hoshi.tpl'
    extract(iso,'a/hoshi_2',model);extract(iso,'a/hoshi_2-',tpl)
    if model.read_bytes()[0x44:0x84].split(b'\0')[0]!=b'hoshi_2':
        raise ValueError('Original indicator bank changed')
    helpers=['character_mesh.py','character_animation_pack.py','character_animation_data.py','character_bake.py','character_node.py','iso_assets.py']
    fingerprint=hashlib.sha256(Path(__file__).read_bytes()+model.read_bytes()+tpl.read_bytes()+b''.join(Path('tools/'+p).read_bytes() for p in helpers)).hexdigest()
    manifest=Path('romfs/message-indicator.json')
    outputs=[Path('romfs/message-indicator'+s) for s in ('.t3x','.pmanim')]
    if manifest.exists() and all(p.exists() for p in outputs) and json.loads(manifest.read_text()).get('fingerprint')==fingerprint:return
    atlas=work/'atlas.png'
    meta=compile_mesh([(model,tpl,['HOS_C_1'])],work/'indicator.pmmesh',atlas,outputs[1])
    image=Image.open(atlas).convert('RGBA')
    if image.crop((0,128,1024,1024)).getbbox():raise ValueError('Indicator atlas exceeds one row')
    image.crop((0,0,1024,128)).save(atlas)
    tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
    subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(outputs[0]),str(atlas)],check=True,stdout=subprocess.DEVNULL)
    records=[json.loads(model.with_suffix('.provenance.json').read_text()),json.loads(tpl.with_suffix('.tpl.provenance.json').read_text())]
    manifest.write_text(json.dumps(dict(fingerprint=fingerprint,sources=records,animation='HOS_C_1',mesh=meta,uv_scale=8,conversion='Original model/node/texture animation retained in PMANIM3; source textures resampled into existing124px cells with2px edge padding; unused atlas rows removed',license='Original Nintendo/Intelligent Systems local asset; no redistribution rights'),indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);build(p.parse_args().iso)
