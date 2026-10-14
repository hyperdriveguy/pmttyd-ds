#!/usr/bin/env python3
"""Export original US item-get pose/base effect masks, no generated artwork."""
import argparse,hashlib,json,struct,subprocess
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO,extract,convert_tpl
from character_mesh import compile_mesh
from audio_convert import stm

def build(iso=DEFAULT_ISO,output='romfs',work='local-assets/field-item-get'):
    out=Path(output);cache=Path(work);out.mkdir(parents=True,exist_ok=True);cache.mkdir(parents=True,exist_ok=True)
    dependencies=[Path(__file__),*[Path(__file__).parent/n for n in ['iso_assets.py','character_mesh.py','character_animation_pack.py','character_animation_data.py','character_bake.py','audio_convert.py']]]
    fingerprint=hashlib.sha256(b''.join(p.read_bytes() for p in dependencies)+str((Path(iso).stat().st_size,Path(iso).stat().st_mtime_ns)).encode()).hexdigest()
    required=['field-item-get-key-fanfare.wav','field-item-get-mario.pmanim','field-item-get-mario.t3x','field-item-get-mario.bin',*[f'field-item-get-effect-{i}.t3x' for i in [42,43,44]]]
    manifest=out/'field-item-get.json'
    if manifest.exists() and not (out/'field-item-get-key-fanfare.wav.loop').exists() and all((out/p).is_file() for p in required) and json.loads(manifest.read_text()).get('fingerprint')==fingerprint:return
    for source,name in [('a/a_mario','a_mario'),('a/a_mario-','a_mario.tpl'),('e/us/effect.tpl','effect.tpl')]:extract(iso,source,cache/name)
    raw=(cache/'effect.tpl').read_bytes()
    if hashlib.sha256(raw).hexdigest()!='3b62850fe75afb95eebb9fc240e20204d5ed320fba0fc956729f28405c2c789b':raise ValueError('Original US base effect bank changed')
    atlas=cache/'mario.png';metadata=compile_mesh([(cache/'a_mario',cache/'a_mario.tpl',['M_I_U'])],cache/'mario.pmmesh',atlas,out/'field-item-get-mario.pmanim')
    height=128
    while height<((metadata['textures']+7)//8)*128:height*=2
    Image.open(atlas).crop((0,0,1024,height)).save(atlas)
    tex3ds='/opt/devkitpro/tools/bin/tex3ds'
    subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(out/'field-item-get-mario.t3x'),str(atlas)],check=True,stdout=subprocess.DEVNULL)
    (out/'field-item-get-mario.bin').write_bytes(struct.pack('<8sf',b'PMIGUV01',1024/height))
    images=[]
    for index in [42,43,44]:
        png=cache/f'effect-{index}.png';convert_tpl(cache/'effect.tpl',png,index)
        with Image.open(png) as image:
            dimensions=list(image.size);pixels=image.tobytes()
        if index==42 and dimensions!=[128,128] or index==44 and dimensions!=[16,16]:raise ValueError('Original effect dimensions changed')
        subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(out/f'field-item-get-effect-{index}.t3x'),str(png)],check=True,stdout=subprocess.DEVNULL)
        images.append(dict(id=index,dimensions=dimensions,pixel_sha256=hashlib.sha256(pixels).hexdigest()))
    source='sound/stream/ff_get2_32k.stm';raw_audio=cache/'ff_get2_32k.stm'
    extract(iso,source,raw_audio)
    if hashlib.sha256(raw_audio.read_bytes()).hexdigest()!='847b529744401aa5590b821ff7502a65b1ac59045306dc8792d6d44b4e05f7dc':raise ValueError('Original key-item fanfare changed')
    fanfare=stm(raw_audio,out/'field-item-get-key-fanfare.wav')
    if fanfare['rate']!=32000 or fanfare['channels']!=2 or fanfare['samples']!=83888 or fanfare['loop_enabled']:raise ValueError('Original key-item fanfare PCM metadata changed')
    fanfare.update(provenance=json.loads(Path(str(raw_audio)+'.provenance.json').read_text()),
        output='field-item-get-key-fanfare.wav',pcm_sha256=hashlib.sha256((out/'field-item-get-key-fanfare.wav').read_bytes()).hexdigest(),
        original_bgm='BGM_FF_GET_KEYITEM1',original_row=217,volume=80,pan=64)
    metadata.update(fanfare=fanfare,fingerprint=fingerprint,dimensions=[1024,height],uv_scale=1024/height,effect_images=images,
        sources=[json.loads((cache/name).with_suffix('.tpl.provenance.json' if name.endswith('.tpl') else '.provenance.json').read_text()) for name in ['a_mario','a_mario.tpl','effect.tpl']],
        conversion='Original M_I_U mesh/sparse animation, existing124px+2px-border pose cells; unused atlas lowerhalf cropped. Effect masks original dimensions/RGBA conversion, no resampling/color tint bake.',
        license='Original Nintendo/Intelligent Systems assets for local use; no redistribution rights',limitations='Original type6 shader/camera/window/control providers remain separate; assets alone do not establish raster parity')
    manifest.write_text(json.dumps(metadata,indent=2)+'\n')
    print('Original item-get pose/effect masks:',metadata['textures'],'cells,',images)
if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);a=p.parse_args();build(a.iso)
