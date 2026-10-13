#!/usr/bin/env python3
"""Compile the original east-pipe underground map into ignored native assets."""
import hashlib,json,os,shutil,struct,subprocess
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO,extract,convert_tpl
from map_convert import convert
from room_convert import relocations,road_hit
from audio_convert import stm

def build(iso=DEFAULT_ISO):
    work=Path('local-assets/underground');work.mkdir(parents=True,exist_ok=True)
    out=Path('romfs');out.mkdir(exist_ok=True)
    paths={}
    for name in ('d','t','c','s'):
        path=work/('tik_01.'+name);extract(iso,'m/tik_01/'+name,path);paths[name]=path
    fingerprint=hashlib.sha256(Path(__file__).read_bytes()+Path('tools/map_convert.py').read_bytes()+b''.join(p.read_bytes() for p in paths.values())).hexdigest()
    manifest=out/'underground-assets.json'
    required=['field-pipes.bin','underground-music.wav']+['tik_01.'+x for x in ('vertices','batches','walls','ground','head','returns','uvanim','meshanim','camera')]
    if manifest.exists() and json.loads(manifest.read_text()).get('fingerprint')==fingerprint and all((out/x).exists() for x in required):return
    info=convert(paths['d'],work/'tik_01',hidden=('bom_kabe_2','bom_kabe_4'))
    for ext in ('vertices','batches','walls','ground','head','uvanim','meshanim'):
        shutil.copyfile(work/'tik_01'/('map.'+ext),out/('tik_01.'+ext))
    shutil.copyfile(paths['c'],out/'tik_01.camera')
    # No source damage-return activation is admitted until the init is bound.
    (out/'tik_01.returns').write_bytes(struct.pack('<4sII',b'RPOS',1,0))
    tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
    for index,name in enumerate(info['textures']):
        path=work/f'texture-{index}.png';convert_tpl(paths['t'],path,index)
        image=Image.open(path).convert('RGBA');w,h=image.size
        image.resize((min(128,1<<(w-1).bit_length()),min(128,1<<(h-1).bit_length())),Image.Resampling.LANCZOS).save(path)
        subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(out/f'tik_01-{index}.t3x'),str(path)],check=True,stdout=subprocess.DEVNULL)
    gor=json.loads(Path('local-assets/gor_02/map-summary.json').read_text())
    endpoints=[road_hit(gor['collision_nodes'],'dokan'),road_hit(info['collision_nodes'],'dokan_2')]
    rels=[]
    for module in ('gor','tik'):
        path=work/(module+'.rel');extract(iso,'rel/'+module+'.rel',path);rels.append(relocations(path))
    records=[]
    for source,target,offset,case in ((2,5,0x3ee60,6),(5,2,0x13870,10)):
        raw,ptr,_=rels[0 if source==2 else 1]
        flags,_,direction,x,y,z,length=struct.unpack_from('>hh5i',raw,offset+4)
        name=raw[ptr[offset]:raw.index(b'\0',ptr[offset])].decode('ascii')
        destination=raw[ptr[offset+40]:raw.index(b'\0',ptr[offset+40])].decode('ascii')
        arrival=raw[ptr[offset+44]:raw.index(b'\0',ptr[offset+44])].decode('ascii')
        if flags!=2 or x!=100000 or length!=-1:raise ValueError('Unexpected original pipe record')
        a,b=endpoints if source==2 else endpoints[::-1]
        records.append(dict(source=source,target=target,node=a['source_node'],arrivalNode=b['source_node'],case=case,center=a['center'],arrival=b['center'],direction=direction,name=name,destination=destination,entrance=arrival,relOffset=offset))
    with (out/'field-pipes.bin').open('wb') as f:
        f.write(struct.pack('<8sI',b'PMPIPE1',len(records)))
        for r in records:f.write(struct.pack('<5I6fI',r['source'],r['target'],r['node'],r['arrivalNode'],r['case'],*r['center'],*r['arrival'],r['direction']))
    music=work/'stg_tik2_32k.stm';extract(iso,'sound/stream/stg_tik2_32k.stm',music)
    music_meta=stm(music,out/'underground-music.wav')
    manifest.write_text(json.dumps(dict(fingerprint=fingerprint,source='m/tik_01',map=info,pipes=records,music=music_meta,musicBinding='BGM_STG0_TIK1 -> sound/stream/stg_tik2_32k.stm',conversion='Original geometry/UV/materials; max128 power-of-two textures, Lanczos',license='Nintendo/Intelligent Systems resources; no redistribution rights'),indent=2)+'\n')
if __name__=='__main__':
    import argparse
    p=argparse.ArgumentParser();p.add_argument('--iso',default=os.environ.get('TTYD_ISO',DEFAULT_ISO));build(p.parse_args().iso)
