#!/usr/bin/env python3
"""Import original first-talk Mario poses and Frankuri1 stream locally."""
from pathlib import Path
import argparse,json,shutil,struct,subprocess,hashlib
from PIL import Image
from iso_assets import DEFAULT_ISO,extract,convert_tpl
from character_mesh import compile_mesh
from audio_convert import stm
def build(iso):
    work=Path('local-assets/professor-scene');work.mkdir(parents=True,exist_ok=True)
    tpl=work/'a_mario.tpl';extract(iso,'a/a_mario-',tpl);models=[]
    for bank,names in [('a_mario',['M_S_1','M_W_1','M_V_2','M_U_3']),('e_mario',['M_I_G','M_D_8','M_C_3','M_N_7'])]:
        model=work/bank;extract(iso,'a/'+bank,model)
        if model.read_bytes()[0x44:0x84].split(b'\0')[0]!=b'a_mario':raise ValueError('Unexpected original texture owner')
        models.append((model,tpl,names))
    music=work/'frankly.stm';extract(iso,'sound/stream/evt_fkh1_32k.stm',music)
    helpers=['character_mesh.py','character_bake.py','character_node.py','character_animation_pack.py','character_animation_data.py','audio_convert.py','iso_assets.py']
    fingerprint=hashlib.sha256(Path(__file__).read_bytes()+b''.join(Path('tools',h).read_bytes() for h in helpers)+b''.join(m.read_bytes() for m,_,_ in models)+tpl.read_bytes()+music.read_bytes()).hexdigest()
    manifest=Path('romfs/professor-scene.json')
    required=[Path('romfs/professor-mario.'+s) for s in ['pmmesh','pmanim','t3x','bin']]+[Path('romfs/professor-music.wav'),Path('romfs/professor-map.t3x'),Path('romfs/professor-map.bin')]
    if manifest.exists() and all(p.exists() for p in required) and json.loads(manifest.read_text()).get('fingerprint')==fingerprint:return
    meta=compile_mesh(models,'romfs/professor-mario.pmmesh',work/'atlas.png','romfs/professor-mario.pmanim')
    height=128
    while height<((meta['textures']+7)//8)*128:height*=2
    Image.open(work/'atlas.png').crop((0,0,1024,height)).resize((512,height//2),Image.Resampling.LANCZOS).save(work/'runtime.png')
    Path('romfs/professor-mario.bin').write_bytes(struct.pack('<8sf',b'PMSUV1',1024/height))
    tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
    subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o','romfs/professor-mario.t3x',str(work/'runtime.png')],check=True,stdout=subprocess.DEVNULL)
    icons=work/'icon.tpl';extract(iso,'icon.tpl',icons);convert_tpl(icons,work/'map.png',319)
    image=Image.open(work/'map.png');w,h=image.size
    Path('romfs/professor-map.bin').write_bytes(struct.pack('<8s2f',b'PMSICON1',w,h))
    subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o','romfs/professor-map.t3x',str(work/'map.png')],check=True,stdout=subprocess.DEVNULL)
    meta.update(fingerprint=fingerprint,atlas_height=height,runtime_atlas_width=512,cell_resampling='62px with1px border to bound linear-memory use',uv_scale=1024/height,music=stm(music,Path('romfs/professor-music.wav')),
                item=dict(kind=96,icon=314,tpl_image=319,width=w,height=h),source='a/a_mario,a/e_mario,a/a_mario-,sound/stream/evt_fkh1_32k.stm,icon.tpl',license='Local original game data; no redistribution rights')
    Path('romfs/professor-scene.json').write_text(json.dumps(meta,indent=2)+'\n')
if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);build(p.parse_args().iso)
