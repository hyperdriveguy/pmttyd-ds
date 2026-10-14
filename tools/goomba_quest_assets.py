#!/usr/bin/env python3
"""Original Goomba quest Mario poses, separate from the full field atlas."""
import argparse,hashlib,json,struct,subprocess
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO,extract
from character_mesh import compile_mesh

def build(iso=DEFAULT_ISO,output='romfs',work='local-assets/goomba-quest'):
    out=Path(output);cache=Path(work);out.mkdir(parents=True,exist_ok=True);cache.mkdir(parents=True,exist_ok=True)
    scripts=[Path(__file__),*[Path(__file__).parent/n for n in ['character_mesh.py','character_animation_pack.py','character_animation_data.py','character_bake.py','iso_assets.py']]]
    fingerprint=hashlib.sha256(b''.join(p.read_bytes() for p in scripts)+str(Path(iso).stat().st_mtime_ns).encode()).hexdigest()
    manifest=out/'goomba-quest-mario.json';required=['goomba-quest-mario.pmanim','goomba-quest-mario.t3x','goomba-quest-mario.bin']
    if manifest.exists() and all((out/p).exists() for p in required) and json.loads(manifest.read_text()).get('fingerprint')==fingerprint:return
    for source,name in [('a/e_mario','e_mario'),('a/a_mario','a_mario'),('a/a_mario-','a_mario.tpl')]:extract(iso,source,cache/name)
    if (cache/'e_mario').read_bytes()[0x44:0x84].split(b'\0')[0]!=b'a_mario':raise ValueError('Original texture binding changed')
    atlas=cache/'atlas.png'
    metadata=compile_mesh([(cache/'e_mario',cache/'a_mario.tpl',['M_N_5B','M_N_C']),
                           (cache/'a_mario',cache/'a_mario.tpl',['M_S_1'])],cache/'quest.pmmesh',atlas,out/'goomba-quest-mario.pmanim')
    height=128
    while height<((metadata['textures']+7)//8)*128:height*=2
    image=Image.open(atlas);image.crop((0,0,1024,height)).save(atlas)
    subprocess.run(['/opt/devkitpro/tools/bin/tex3ds','-f','rgba8','-z','auto','-o',str(out/'goomba-quest-mario.t3x'),str(atlas)],check=True)
    (out/'goomba-quest-mario.bin').write_bytes(struct.pack('<8sf',b'PMQ13UV1',1024/height))
    metadata.update(fingerprint=fingerprint,source='US G8ME01 a/e_mario + original a_mario texture, a/a_mario stay pose',
                    dimensions=[1024,height],uv_scale=1024/height,license='Nintendo/Intelligent Systems original assets, local use only; no redistribution rights')
    manifest.write_text(json.dumps(metadata,indent=2)+'\n')
    print('Original quest poses:',metadata['textures'],'texture cells,',1024,height)
if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);a=p.parse_args();build(a.iso)
