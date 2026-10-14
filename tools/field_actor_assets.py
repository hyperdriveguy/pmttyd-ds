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
import tempfile
import os
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

def npc_source_data(iso, tribes=None):
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
        for name,address in (tribes or [('c_tyusan',0x80333554),('c_kuri_h',0x80333014),('c_f_mura',0x80333110),('c_kuribo_n',0x8033335c)]):
            raw=read(address,0x54);width,height=struct.unpack_from('>2f',raw,0x28)
            ptr=struct.unpack_from('>I',raw,0x14)[0];talk=bytearray()
            for i in range(32):
                c=read(ptr+i,1)[0]
                if not c:break
                talk.append(c)
            else:raise ValueError('Unterminated original talk pose')
            result[name]=dict(address=address,width=width,height=height,talk_pose=talk.decode('ascii'),sha256=hashlib.sha256(raw).hexdigest())
            def source_string(ptr):
                if not ptr:return ''
                data=bytearray()
                for i in range(64):
                    c=read(ptr+i,1)[0]
                    if not c:return data.decode('ascii')
                    data.append(c)
                raise ValueError('Unterminated original NPC string')
            result[name].update(idle=source_string(struct.unpack_from('>I',raw,0x10)[0]),
                walk=source_string(struct.unpack_from('>I',raw,0x18)[0]),run=source_string(struct.unpack_from('>I',raw,0x1c)[0]),
                run_threshold=struct.unpack_from('>f',raw,0x3c)[0],step_period=struct.unpack_from('>H',raw,0x48)[0],
                sound_left=source_string(struct.unpack_from('>I',raw,0x40)[0]),sound_right=source_string(struct.unpack_from('>I',raw,0x44)[0]))
        angles=read(0x803041e0,91*4)
        angle_path=Path('romfs/npc-angle.bin')
        angle_bytes=b'PMNANG01'+struct.pack('<91f',*struct.unpack('>91f',angles))
        # Readers can run while another build refreshes this shared source bank.
        # Publish the complete unchanged table without a transient empty file.
        with tempfile.NamedTemporaryFile(dir=angle_path.parent,prefix='npc-angle-',suffix='.tmp',delete=False) as temp:
            temp.write(angle_bytes);angle_temp=Path(temp.name)
        try:os.replace(angle_temp,angle_path)
        finally:angle_temp.unlink(missing_ok=True)
        result['angle_table']=dict(address=0x803041e0,count=91,sha256=hashlib.sha256(angles).hexdigest())
        return result


def compile_model(iso,asset,name,poses):
    work = Path('local-assets/field-actors');work.mkdir(parents=True, exist_ok=True)
    model = work/name;tpl = work/(name+'.tpl')
    extract(iso, 'a/'+name, model)
    bank = model.read_bytes()[0x44:0x84].split(b'\0')[0].decode('ascii')
    raw=model.read_bytes();directory=struct.unpack_from('>I',raw,0x1ac)[0]
    zero=raw[directory:directory+60].split(b'\0')[0].decode('ascii')
    poses=[zero,*[pose for pose in poses if pose!=zero]]
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
                      original_index0=zero,
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
    plaza_setup=work/'gor_01.s';extract(iso,'m/gor_01/s',plaza_setup)
    goomba3=tagged_npcs(plaza_setup.read_bytes().decode('shift_jis'))['クリボー３']
    if goomba3['tribe']!='NPCクリボー' or goomba3['aitype']!='なし':
        raise ValueError('Unsupported original plaza Goomba setup')
    chu = tagged_npcs(setup.read_bytes().decode('shift_jis'))['チューさん３']
    if chu['tribe'] != 'チューさん' or chu['aitype'] != 'なし':
        raise ValueError('Unsupported original stationary actor setup')
    darkly = tagged_npcs(setup.read_bytes().decode('shift_jis'))['クラガリさん']
    if darkly['tribe']!='クラガリさん' or darkly['aitype']!='なし':
        raise ValueError('Unsupported original Darkly setup')
    darkly_scale=compile_model(iso,'field-darkly','c_f_mura',['S_1',npc_constants['c_f_mura']['talk_pose']])
    goomba_scale=compile_model(iso,'field-goomba','c_kuribo_n',['S_1','T_1','W_1','R_1'])
    chu_scale = compile_model(iso,'field-chu','c_tyusan',['CHU_S_1',npc_constants['c_tyusan']['talk_pose']])
    professor_scale = json.loads(Path('romfs/professor.json').read_text())['uv_scale']
    object_scale=build_objects(iso)
    model_specs = [
        dict(asset='field-chu',scale=chu_scale,original='c_tyusan',path=work/'c_tyusan'),
        dict(asset='professor',scale=professor_scale,original='c_kuri_h',path=Path('local-assets/professor/c_kuri_h')),
        dict(asset='field-saveblock',scale=object_scale,original='MOBJ_SaveBlock',path=Path('local-assets/map-objects/MOBJ_SaveBlock'),hit=True),
        dict(asset='field-darkly',scale=darkly_scale,original='c_f_mura',path=work/'c_f_mura'),
        dict(asset='field-goomba',scale=goomba_scale,original='c_kuribo_n',path=work/'c_kuribo_n'),
    ]
    models = [(m['asset'],m['scale']) for m in model_specs]
    # Source Frankly init: exact story8/10 branches; story9 hides his NPC.
    # Interior admission is the current same-map door visibility adapter.
    records = [dict(key=identifier('gor_01','クリボー３'),room=0,model=4,required=0,forbidden=0,low=0,high=55,
                    position=goomba3['position'],yaw=goomba3['yaw'],pose='S_1',npc_flags=0x00080603,
                    wander=dict(radius=100,speed=30),
                    talk_prefix='gor_01_028_',source_talk_present=1,
                    source='m/gor_01/s:クリボー３;gor.rel:kuribo3_init@0x32f64/regular@0x32f6c/talk@0x32f7c'),
               dict(key=identifier('gor_02','チューさん３'),room=2,model=0,required=0,forbidden=0,low=0,high=55,
                    position=chu['position'],yaw=chu['yaw'],pose='CHU_S_1',talk_prefix='gor_02_031_',source='m/gor_02/s:チューさん３'),
               dict(key=identifier('gor_02','クリハカセ'),room=2,model=1,required=2,forbidden=0,low=8,high=8,
                    position=[-135,0,-240],yaw=90,pose='S_1',source='gor.rel:kurihakase_init@0x410c8'),
               dict(key=identifier('gor_02','クリハカセ'),room=2,model=1,required=0,forbidden=2,low=10,high=10,
                    position=[-220,0,305],yaw=270,pose='S_1',source='gor.rel:kurihakase_init@0x410c8'),
               dict(key=identifier('gor_01','mobj_save'),room=0,model=2,required=0,forbidden=0,low=341,high=341,gate=2,kind=1,scale=10,system_forbidden=0x20,
                    position=[185,60,-55],yaw=0,pose='S_1',source='gor.rel:gor_01_init_evt@0x3c514'),
               dict(key=identifier('gor_02','クラガリさん'),room=2,model=3,required=0,forbidden=0,low=0,high=55,
                    position=darkly['position'],yaw=darkly['yaw'],pose='S_1',talk_prefix='gor_02_034_',npc_flags=0x40080603,
                    talk_rules=[(0,56,'gor_02_034_00')],source='m/gor_02/s:クラガリさん;gor.rel:init@0x446a8/talk@0x44d90'),
               dict(key=identifier('gor_02','クリボー４'),room=2,model=4,required=0,forbidden=0,low=0,high=55,
                    position=[-370,0,130],yaw=270,pose='S_1',npc_flags=0x40080603,source_talk_present=1,
                    wander=dict(radius=70,speed=30),talk_prefix='gor_02_030_',
                    talk_rules=[(0,limit,'gor_02_030_'+suffix) for limit,suffix in [(56,'00'),(112,'01'),(164,'02_01'),(175,'03'),(176,'03_01'),(215,'03_02'),(261,'04'),(339,'05'),(373,'06'),(401,'07')]]+[(3,0,'gor_02_030_08')],source='gor.rel:kuribo4_init@0x438f0/regular@0x43924')]
    tagged_order={name:i for i,name in enumerate(tagged_npcs(setup.read_bytes().decode('shift_jis')))}
    source_order={identifier('gor_02',name):i for name,i in tagged_order.items()}
    source_order.update({identifier('gor_01',name):i for i,name in enumerate(tagged_npcs(plaza_setup.read_bytes().decode('shift_jis')))})
    records.sort(key=lambda r:(r['room'],source_order.get(r['key'],0)))
    # Original bbox fields consumed by mobjHitEntry, not rendered pose bounds.
    bounds=struct.unpack_from('>6f',Path('local-assets/map-objects/MOBJ_SaveBlock').read_bytes(),0xd0)
    data = bytearray(struct.pack('<8sII',b'PMNPC008',len(models),len(records)))
    collision=[]
    for spec in model_specs:
        asset,scale=spec['asset'],spec['scale']
        hit=spec.get('hit',False);box=bounds if hit else [0]*6
        data += struct.pack('<32sfI6f',fixed(asset),scale,int(hit),*box)
        original_name=spec['original']
        data += fixed(original_name)
        path=spec['path']
        draw_mask=struct.unpack_from('>I',path.read_bytes(),0xc4)[0]
        data += struct.pack('<I',draw_mask)
        collision.append(dict(asset=asset,original_name=original_name,hit_mode=int(hit),bounds=box,draw_mask=draw_mask))
    for r in records:
        prefix=r.get('talk_prefix','')
        r['talk_prefix']=prefix
        r['source_name']='mobj_save' if r.get('kind')==1 else ''
        data += struct.pack('<8I2i5f32s32s',r['key'],r['room'],r['model'],r['required'],r['forbidden'],r.get('gate',1),r.get('kind',0),r.get('system_forbidden',0),
                            r['low'],r['high'],*r['position'],r['yaw'],r.get('scale',2),fixed(r['pose']),fixed(prefix) if prefix else bytes(32))
        data += fixed(r['source_name']) if r['source_name'] else bytes(32)
        dims=npc_constants[model_specs[r['model']]['original']] if r.get('kind',0)==0 else dict(width=0,height=0,talk_pose='')
        r['npc_dimensions']=dims
        data += struct.pack('<2f',dims['width'],dims['height'])
        data += fixed(dims['talk_pose']) if dims['talk_pose'] else bytes(32)
        # Source Entry3 + setup80600, with Frankly's constructor OR40000000.
        r['npc_flags']=r.get('npc_flags',0x80603 | (0x40000000 if model_specs[r['model']]['original']=='c_kuri_h' else 0)) if r.get('kind',0)==0 else 0
        data += struct.pack('<I',r['npc_flags'])
        rules=r.get('talk_rules',[(0,limit,prefix+f'{i:02d}') for i,limit in enumerate([56,112,164,215,261,339,373,401])]+[(3,0,prefix+'08')] if prefix else [])
        r['talk_rules']=rules
        data += struct.pack('<I',len(rules))
        for comparison,story,message in rules:data += struct.pack('<Ii32s',comparison,story,fixed(message))
        r['source_talk_present']=r.get('source_talk_present',int(bool(prefix)))
        w=r.get('wander');r['wander']=w
        data += struct.pack('<3I3fI',r['source_talk_present'],int(w is not None),w.get('flags',0) if w else 0,w['radius'] if w else 0,w['speed'] if w else 0,dims['run_threshold'] if w else 0,dims['step_period'] if w else 0)
        for field in ['idle','walk','run','sound_left','sound_right']:
            data += fixed(dims[field]) if w and dims[field] else bytes(32)
    Path('romfs/field-actors.pmnpc').write_bytes(data)
    Path('romfs/field-actors.json').write_text(json.dumps(dict(models=models,actors=records,collision=collision,npc_constants=npc_constants,
        setup=json.loads(setup.with_suffix('.s.provenance.json').read_text()),
        plaza_setup=json.loads(plaza_setup.with_suffix('.s.provenance.json').read_text()),
        coverage='Static original init subset; Chu/Darkly story0..55 is a native coverage envelope; Darkly binds only the original story<56 print arm, not later quest branches. Actor keys are native FNV IDs; positions/yaw/pose retain source units. Original display-direction driver, talk arbitration and scripted motion remain separate.',
        license='Local original Nintendo/Intelligent Systems resources; no redistribution rights'),ensure_ascii=False,indent=2)+'\n')


if __name__ == '__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--iso',default=DEFAULT_ISO)
    build(parser.parse_args().iso)
