#!/usr/bin/env python3
"""Export four original house PCM samples and source object sound-position data."""
import argparse,hashlib,json,struct
from pathlib import Path
import numpy as np
from iso_assets import DEFAULT_ISO,extract
from audio_map import gamecube_tables,project_effects,macro_samples
from audio_convert import sample
from character_bake import trans,rotate

KEYS=[('flight','SFX_EVT_OPN_PARETTA_MOVE1',972,1278,1435,1176),('postbox','SFX_EVT_POST_SHAKE1',973,527,620,479),('door-open','SFX_DOOR_OPEN1',568,931,1062,840),('door-shut','SFX_DOOR_SHUT1',589,942,1073,851)]

def object_positions(path):
    raw=Path(path).read_bytes();base=32
    u=lambda o:struct.unpack_from('>I',raw,o)[0]
    def text(o):return raw[o:raw.index(b'\0',o)].decode('ascii')
    table=base+u(4)+u(8)*4;names=table+u(12)*8
    chunks={text(names+u(table+n*8+4)):base+u(table+n*8) for n in range(u(12))}
    nodes={}
    def walk(o,parent=None):
        srt=struct.unpack_from('>9f',raw,o+24);local=trans(srt[6:])@rotate(srt[3:6])@np.diag([*srt[:3],1])
        world=nodes[parent]['world']@local if parent is not None else local
        bbox=np.array(struct.unpack_from('>6f',raw,o+60));center=(bbox[:3]+bbox[3:])*.5 if u(o+92) else np.array(srt[6:])
        nodes[o]=dict(name=text(base+u(o)),parent=parent,world=world,center=center,child=u(o+12))
        if u(o+12):walk(base+u(o+12),o)
        if u(o+16):walk(base+u(o+16),parent)
    walk(base+u(chunks['information']+4))
    result=[]
    for name in ['S_ie_door','S_posuto']:
        matches=[o for o,n in nodes.items() if n['name']==name]
        if len(matches)!=1:raise ValueError('Unique original sound object required')
        target=matches[0];root=nodes[target]['world'];inverse=np.linalg.inv(root);points=[]
        for o,n in nodes.items():
            ancestor=o
            while ancestor is not None and ancestor!=target:ancestor=nodes[ancestor]['parent']
            if ancestor!=target or n['child']:continue
            p=n['world']@np.array([*n['center'],1]);points.append((inverse@p)[:3])
        if not points:raise ValueError('Original sound-position leaf average empty')
        result.append(dict(name=name,root=target-base,world=root[:3,:].flatten().tolist(),localMean=np.mean(points,axis=0).tolist(),leaves=len(points)))
    return result

def build(iso=DEFAULT_ISO):
    iso=Path(iso);out=Path('romfs');out.mkdir(exist_ok=True);work=Path('local-assets/opening-sounds');work.mkdir(parents=True,exist_ok=True)
    helpers=['audio_map.py','audio_convert.py','character_bake.py','iso_assets.py']
    fingerprint=hashlib.sha256(Path(__file__).read_bytes()+b''.join(Path('tools',p).read_bytes() for p in helpers)+str((iso.stat().st_size,iso.stat().st_mtime_ns)).encode()).hexdigest()
    manifest=out/'opening-house-sounds.json';products=['opening-house-sounds.bin']+[f'opening-house-{clip}.wav' for clip,*_ in KEYS]
    if manifest.exists():
        old=json.loads(manifest.read_text())
        if old.get('fingerprint')==fingerprint and all((out/p).is_file() and hashlib.sha256((out/p).read_bytes()).hexdigest()==old.get('products',{}).get(p) for p in products):return
    for ext in ['proj','pool','sdir','samp']:extract(iso,'sound/proj/pmario.'+ext,work/('pmario.'+ext))
    effects=project_effects((work/'pmario.proj').read_bytes());macros=macro_samples((work/'pmario.pool').read_bytes());rows,spans,_=gamecube_tables(iso);byname={r['name']:r for r in rows if r['table']=='sfx'}
    payload=bytearray(b'PMHSND01'+struct.pack('<I',4));cues=[]
    for clip,name,index,fx,obj,sid in KEYS:
        row=byname[name];objects=effects.get(row['fx_id'],[])
        if row['table_index']!=index or row['fx_id']!=fx or len(objects)!=1 or objects[0]['object_id']!=obj or macros[obj]['sample_ids']!=[sid]:raise ValueError('Original house sample identity changed: '+name)
        pcm=sample(work/'pmario.sdir',work/'pmario.samp',sid,out/f'opening-house-{clip}.wav')
        if pcm['loop_enabled']:raise ValueError('Used original house sample unexpectedly loops')
        payload+=struct.pack('<7I8B32s',index,row['flags'],fx,obj,sid,pcm['rate'],pcm['samples'],*row['parameters'],name.encode())
        cues.append(dict(clip=clip,table=row,object=objects[0],macro=macros[obj],pcm=pcm,playback='Original primary-sample PCM presentation; complete MusyX macro pitch/envelopes/delay/RNG not rendered'))
    d=work/'aaa_00.d';extract(iso,'m/aaa_00/d',d);positions=object_positions(d)
    for position in positions:payload+=struct.pack('<15f',*position['world'],*position['localMean'])
    with iso.open('rb') as source:
        if source.read(8)!=b'G8ME01\0\0':raise ValueError('US disc0/revision0 required')
        source.seek(0x420);dol=struct.unpack('>I',source.read(4))[0];source.seek(dol);h=source.read(256)
        def read(a,n):
            for j in range(18):
                off,va,size=(struct.unpack_from('>I',h,b+4*j)[0] for b in [0,0x48,0x90])
                if va<=a and a+n<=va+size:source.seek(dol+off+a-va);return source.read(n)
            raise ValueError('Unmapped original sound evidence')
        environment=[dict(address=0x8032b690+28*i,bytes=read(0x8032b690+28*i,28).hex()) for i in range(5)]
        if struct.unpack('>I',read(0x8032da20,4))[0]!=0x802e349c or read(0x802e349c,13)!=b'ENV_OPN_AAA1\0':raise ValueError('Original house ENV name changed')
    (out/'opening-house-sounds.bin').write_bytes(payload)
    manifest.write_text(json.dumps(dict(fingerprint=fingerprint,products={p:hashlib.sha256((out/p).read_bytes()).hexdigest() for p in products},cues=cues,objects=positions,environment=dict(index=1,name='ENV_OPN_AAA1',rows=environment,consumer='Unbound original RNG/ENV scheduling; requests retained, no fabricated ambience'),sourceTables=spans,conversion='Original rate PCM16 mono and source leaf-average object-position metadata; native PS matrix/spatial/macro adapters documented separately',license='Nintendo/Intelligent Systems original resources; local use without redistribution rights'),indent=2)+'\n')
    print('Original house sounds4 PCM + object positions2; ENV rows5 retained; pack',len(payload))

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);build(p.parse_args().iso)
