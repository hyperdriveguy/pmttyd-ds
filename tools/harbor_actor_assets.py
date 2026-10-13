#!/usr/bin/env python3
"""Bake the original harbor Crump/X-Naut setup resources, locally only."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import struct
import subprocess
from PIL import Image
from iso_assets import DEFAULT_ISO, extract
from character_bake import bake
from audio_convert import stm


def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--iso',default=DEFAULT_ISO)
    args=parser.parse_args();work=Path('local-assets/harbor-actors');work.mkdir(parents=True,exist_ok=True)
    models=[];records=[]
    for name,bank in [('c_kanbu_ibt','c_kanbu'),('c_zako_n','c_zako_n')]:
        model=work/name;tpl=work/(bank+'.tpl')
        extract(args.iso,'a/'+name,model);extract(args.iso,'a/'+bank+'-',tpl)
        # Bank name in the original model header owns the palette binding.
        binding=model.read_bytes()[0x44:0x84].split(b'\0')[0].decode('ascii')
        if binding!=bank:raise ValueError('Original actor texture bank changed: '+binding)
        models.append((model,tpl))
        records.extend([json.loads(model.with_suffix('.provenance.json').read_text()),json.loads(tpl.with_suffix('.tpl.provenance.json').read_text())])
    music=work/'danger.stm';extract(args.iso,'sound/stream/evt_kik4_32k.stm',music)
    records.append(json.loads(music.with_suffix('.stm.provenance.json').read_text()))
    helpers=b''.join(Path('tools/'+name).read_bytes() for name in ['character_bake.py','character_node.py','iso_assets.py','audio_convert.py'])
    fingerprint=hashlib.sha256(Path(__file__).read_bytes()+helpers+music.read_bytes()+b''.join(a.read_bytes()+b.read_bytes() for a,b in models)).hexdigest()
    manifest=Path('romfs/harbor-actors.json')
    if manifest.exists() and json.loads(manifest.read_text()).get('fingerprint')==fingerprint and Path('romfs/harbor-actors.t3x').exists() and Path('romfs/harbor-actors.bin').exists() and Path('romfs/harbor-danger.wav').exists():return
    atlas=Image.new('RGBA',(256,128));metrics=[];talk_offsets=[]
    for i,(model,tpl) in enumerate(models):
        target=work/(model.name+'.png');meta=bake(model,tpl,target,'Z_1');image=Image.open(target).convert('RGBA');atlas.alpha_composite(image,(i*128,0))
        low,high=meta['bounds'];factor=118/max(high[0]-low[0],high[1]-low[1]);center=[(a+b)/2 for a,b in zip(low,high)]
        metrics.append((-(64-center[0]*factor)/factor*.02,(64+center[1]*factor-128)/factor*.02,128/factor*.02))
        bounds=image.getchannel('A').point(lambda a:255 if a>8 else 0).getbbox()
        if bounds is None:raise ValueError('Empty original actor mask')
        x,y,size=metrics[-1]
        talk_offsets.append((x+(bounds[0]+bounds[2])*.5/128*size,y+(128-bounds[1])/128*size,0))
    atlas.save(work/'atlas.png');Path('romfs/harbor-actors.bin').write_bytes(struct.pack('<8sI12f',b'PMHNPC2\0',2,*(v for row in metrics+talk_offsets for v in row)))
    tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
    subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o','romfs/harbor-actors.t3x',str(work/'atlas.png')],check=True,stdout=subprocess.DEVNULL)
    converted_music=stm(music,Path('romfs/harbor-danger.wav'))
    manifest.write_text(json.dumps(dict(fingerprint=fingerprint,assets=records,poses=['Z_1','Z_1'],metrics=metrics,talk_offsets=talk_offsets,music=converted_music,conversion='Original geometry/textures baked at setup pose; root offset and source scalar2 retained; native talk anchor uses opaque image top/center; BGM_EVT_DANGER4 source stream decoded to PCM16 with source loops',license='Local original Nintendo/Intelligent Systems assets; no redistribution rights'),indent=2)+'\n')


if __name__=='__main__':main()
