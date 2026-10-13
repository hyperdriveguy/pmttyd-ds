#!/usr/bin/env python3
"""Build a bounded static-init NPC slice from read-only original resources.

Adding areas uses catalog records and model resources, not renderer branches.
Story gates below are native coverage envelopes, not a general event evaluator.
"""
import argparse
import hashlib
import json
import re
import shutil
import struct
import subprocess
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO, extract
from character_mesh import compile_mesh
from character_surface_mesh import compile_surface_mesh
from professor_assets import build as build_professor
from map_object_assets import build as build_objects
from save_block_event_assets import build as build_save_events


def tagged_npcs(text):
    result = {}
    for block in text.split('<Npc>')[1:]:
        values = dict(re.findall(r'<([a-z_]+)>\s*(.*?)\s*</>', block, re.S))
        name = values['name'].strip('"')
        if name in result:
            raise ValueError('Duplicate original NPC name')
        result[name] = dict(tribe=values['tribe'].strip('"'),
                            position=[float(v) for v in values['position'].split()],
                            yaw=float(values['dir']), aitype=values['aitype'].strip('"'))
    return result


def identifier(room, name):
    value = 2166136261
    for byte in (room + ':' + name).encode('utf-8'):
        value = ((value ^ byte) * 16777619) & 0xffffffff
    return value


def fixed(value):
    data = value.encode('ascii')
    if not data or len(data) >= 32:
        raise ValueError('Invalid catalog string')
    return data.ljust(32, b'\0')

def npc_source_data(iso):
    """Only original DOL data: tribe dimensions/pose strings and angle table."""
    with Path(iso).open('rb') as disc:
        if disc.read(8)!=b'G8ME01\0\0':raise ValueError('US revision0 required')
        disc.seek(0x420);dol=struct.unpack('>I',disc.read(4))[0];disc.seek(dol);header=disc.read(256)
        def read(address,size):
            spans=[]
            for i in range(7,18):
                off,base,n=(struct.unpack_from('>I',header,p+i*4)[0] for p in [0,0x48,0x90])
                if base and base<=address and address+size<=base+n:spans.append(dol+off+address-base)
            if len(spans)!=1:raise ValueError('NPC constant outside source data')
            disc.seek(spans[0]);data=disc.read(size)
            if len(data)!=size:raise ValueError('Truncated source constant')
            return data
        result={}
        for name,address in [('c_tyusan',0x80333554),('c_kuri_h',0x80333014)]:
            raw=read(address,0x54);width,height=struct.unpack_from('>2f',raw,0x28)
            ptr=struct.unpack_from('>I',raw,0x14)[0];talk=bytearray()
            for i in range(32):
                c=read(ptr+i,1)[0]
                if not c:break
                talk.append(c)
            else:raise ValueError('Unterminated original talk pose')
            result[name]=dict(address=address,width=width,height=height,talk_pose=talk.decode('ascii'),sha256=hashlib.sha256(raw).hexdigest())
        angles=read(0x803041e0,91*4)
        Path('romfs/npc-angle.bin').write_bytes(b'PMNANG01'+struct.pack('<91f',*struct.unpack('>91f',angles)))
        result['angle_table']=dict(address=0x803041e0,count=91,sha256=hashlib.sha256(angles).hexdigest())
        return result


def compile_model(iso,asset,name,poses):
    work = Path('local-assets/field-actors');work.mkdir(parents=True, exist_ok=True)
    model = work/name;tpl = work/(name+'.tpl')
    extract(iso, 'a/'+name, model)
    bank = model.read_bytes()[0x44:0x84].split(b'\0')[0].decode('ascii')
    extract(iso, 'a/'+bank+'-', tpl)
    helpers = ['character_mesh.py','character_surface_mesh.py','character_bake.py','character_node.py','character_animation_pack.py','character_animation_data.py','iso_assets.py']
    fingerprint = hashlib.sha256(Path(__file__).read_bytes() + b''.join(Path('tools',p).read_bytes() for p in helpers) +
                                 model.read_bytes()+tpl.read_bytes()+json.dumps(poses).encode()).hexdigest()
    manifest = Path('romfs/'+asset+'.json')
    required = [Path('romfs/'+asset+'.'+suffix) for suffix in ['pmmesh','pmanim','t3x']]
    if not (manifest.exists() and all(p.exists() for p in required) and json.loads(manifest.read_text()).get('fingerprint') == fingerprint):
        compile_mesh([(model,tpl,poses)],required[0],work/(asset+'-legacy.png'))
        result = compile_surface_mesh([(model,tpl,poses)],required[1],work/(asset+'.png'))
        height = 128
        while height < ((result['textures']+7)//8)*128:height *= 2
        Image.open(work/(asset+'.png')).crop((0,0,1024,height)).save(work/(asset+'-runtime.png'))
        tex3ds = shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
        subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(required[2]),str(work/(asset+'-runtime.png'))],check=True,stdout=subprocess.DEVNULL)
        result.update(fingerprint=fingerprint,uv_scale=1024/height,
                      assets=[json.loads(p.read_text()) for p in [model.with_suffix('.provenance.json'),tpl.with_suffix('.tpl.provenance.json')]],
                      license='Local original Nintendo/Intelligent Systems assets; no redistribution rights')
        manifest.write_text(json.dumps(result,indent=2)+'\n')
    return json.loads(manifest.read_text())['uv_scale']


def build(iso):
    build_save_events(iso)
    build_professor(iso)
    npc_constants=npc_source_data(iso)
    work = Path('local-assets/field-actors');work.mkdir(parents=True, exist_ok=True)
    setup = work/'gor_02.s';extract(iso, 'm/gor_02/s', setup)
    chu = tagged_npcs(setup.read_bytes().decode('shift_jis'))['チューさん３']
    if chu['tribe'] != 'チューさん' or chu['aitype'] != 'なし':
        raise ValueError('Unsupported original stationary actor setup')
    chu_scale = compile_model(iso,'field-chu','c_tyusan',['CHU_S_1',npc_constants['c_tyusan']['talk_pose']])
    professor_scale = json.loads(Path('romfs/professor.json').read_text())['uv_scale']
    object_scale=build_objects(iso)
    models = [('field-chu',chu_scale),('professor',professor_scale),('field-saveblock',object_scale)]
    # Source Frankly init: exact story8/10 branches; story9 hides his NPC.
    # Interior admission is the current same-map door visibility adapter.
    records = [dict(key=identifier('gor_02','チューさん３'),room=2,model=0,required=0,forbidden=0,low=0,high=55,
                    position=chu['position'],yaw=chu['yaw'],pose='CHU_S_1',source='m/gor_02/s:チューさん３'),
               dict(key=identifier('gor_02','クリハカセ'),room=2,model=1,required=2,forbidden=0,low=8,high=8,
                    position=[-135,0,-240],yaw=90,pose='S_1',source='gor.rel:kurihakase_init@0x410c8'),
               dict(key=identifier('gor_02','クリハカセ'),room=2,model=1,required=0,forbidden=2,low=10,high=10,
                    position=[-220,0,305],yaw=270,pose='S_1',source='gor.rel:kurihakase_init@0x410c8'),
               dict(key=identifier('gor_01','mobj_save'),room=0,model=2,required=0,forbidden=0,low=341,high=341,gate=2,kind=1,scale=10,system_forbidden=0x20,
                    position=[185,60,-55],yaw=0,pose='S_1',source='gor.rel:gor_01_init_evt@0x3c514')]
    # Original bbox fields consumed by mobjHitEntry, not rendered pose bounds.
    bounds=struct.unpack_from('>6f',Path('local-assets/map-objects/MOBJ_SaveBlock').read_bytes(),0xd0)
    data = bytearray(struct.pack('<8sII',b'PMNPC005',len(models),len(records)))
    collision=[]
    for index,(asset,scale) in enumerate(models):
        hit=index==2;box=bounds if hit else [0]*6
        data += struct.pack('<32sfI6f',fixed(asset),scale,int(hit),*box)
        original_name=['c_tyusan','c_kuri_h','MOBJ_SaveBlock'][index]
        data += fixed(original_name)
        path=Path('local-assets/map-objects/MOBJ_SaveBlock') if hit else Path('local-assets/professor/c_kuri_h') if index==1 else Path('local-assets/field-actors/c_tyusan')
        draw_mask=struct.unpack_from('>I',path.read_bytes(),0xc4)[0]
        data += struct.pack('<I',draw_mask)
        collision.append(dict(asset=asset,original_name=original_name,hit_mode=int(hit),bounds=box,draw_mask=draw_mask))
    for r in records:
        prefix='gor_02_031_' if r['model']==0 else ''
        r['talk_prefix']=prefix
        r['source_name']='mobj_save' if r.get('kind')==1 else ''
        data += struct.pack('<8I2i5f32s32s',r['key'],r['room'],r['model'],r['required'],r['forbidden'],r.get('gate',1),r.get('kind',0),r.get('system_forbidden',0),
                            r['low'],r['high'],*r['position'],r['yaw'],r.get('scale',2),fixed(r['pose']),fixed(prefix) if prefix else bytes(32))
        data += fixed(r['source_name']) if r['source_name'] else bytes(32)
        dims=npc_constants[['c_tyusan','c_kuri_h'][r['model']]] if r.get('kind',0)==0 else dict(width=0,height=0,talk_pose='')
        r['npc_dimensions']=dims
        data += struct.pack('<2f',dims['width'],dims['height'])
        data += fixed(dims['talk_pose']) if dims['talk_pose'] else bytes(32)
    Path('romfs/field-actors.pmnpc').write_bytes(data)
    Path('romfs/field-actors.json').write_text(json.dumps(dict(models=models,actors=records,collision=collision,npc_constants=npc_constants,
        setup=json.loads(setup.with_suffix('.s.provenance.json').read_text()),
        coverage='Static original init subset; Chu story0..55 is a native coverage envelope. Actor keys are native FNV IDs; positions/yaw/pose retain source units. Original display-direction driver, talk arbitration and scripted motion remain separate.',
        license='Local original Nintendo/Intelligent Systems resources; no redistribution rights'),ensure_ascii=False,indent=2)+'\n')


if __name__ == '__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--iso',default=DEFAULT_ISO)
    build(parser.parse_args().iso)
