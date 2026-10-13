#!/usr/bin/env python3
"""Original US battle-init resource descriptors and native surface packs.

Full original banks retain unsupported dormant animation streams. Converted
paper atlases contain original fallback art; admitting them is not a live-field
capture acknowledgement or a claim that every paper pose can execute natively.
"""
import argparse,hashlib,json,struct,shutil,subprocess
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO,extract
from character_bake import u
from character_animation_data import animation
from character_surface_mesh import compile_surface_mesh
RESOURCES=[
 ('p_b_st',0x802efbe4,'191158a8cfaf8eaf26a32d49eaf9fa23adef71144cdb7e3916b354891f152228'),
 ('p_jyabara',0x802efbec,'51e2de4c7dbb72270b3fa4c5c98a2476afba682971b88cf1af994d55710e3e28'),
 ('p_kaiten_h',0x802efbf8,'56c6598869d9aa83e7c7b43a3905838c8e23029c24f6d9c59c9b8f79b519d328'),
 ('EFF_nice_us',0x802f3d00,'a677a8c9fca3ceacf99a652ecbecd368d708bf2007bee0b682c635b1f9d6ae35'),
 ('EFF_good_us',0x802f3d0c,'0ca45b4467c1fe8c84a22bd6ece51dedee5d7120efe9fcdeffecb89ebbabd6d7'),
 ('EFF_great_us',0x802f3d18,'3eecceb5c3e14d40f22f41e6e2130657ba540128b180d7b474944a66e27ace50'),
 ('EFF_wonderful_us',0x802f3d28,'ed72ebe863746012145d0b5917099b8d01e33a3d4695d0392909d11821115c0b'),
 ('EFF_excellent_us',0x802f3d3c,'60b84bb0c5bb312ea923e72fe2065e1dd5f6796c936785ced091d3ddb3ae012e'),
 ('EFF_lucky_us',0x802f3d50,'f35ea6d870c8561a7c6d5b953c763e09c1c72f47cf9a4b30d89acfc7c2bab91a'),
]
def fnv(raw):
    value=2166136261
    for b in raw:value=((value^b)*16777619)&0xffffffff
    return value

def descriptors(raw):
    result=[]
    for index in range(u(raw,0x148)):
        offset=u(raw,0x1ac)+index*64;name=raw[offset:offset+60].split(b'\0',1)[0].decode('ascii');header=u(raw,offset+60)
        if u(raw,header+4)!=1:raise ValueError('Source loop descriptor domain changed')
        loop,start,end=struct.unpack_from('>Iff',raw,header+u(raw,header+0x24));keys=u(raw,header+8)
        try:
            animation(raw,name);supported=True;limitation=None
        except ValueError as error:
            if str(error)!='Partial tangent IN outside initialized source table':raise
            supported=False;limitation=str(error)
        result.append(dict(name=name,loop=loop,start=start,end=end,keys=keys,supported=supported,limitation=limitation))
    return result

def build(iso=DEFAULT_ISO,output='romfs'):
    work=Path('local-assets/battle-ready');work.mkdir(parents=True,exist_ok=True);out=Path(output);out.mkdir(parents=True,exist_ok=True)
    tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds';records=[];payload=bytearray(struct.pack('<8s3I',b'PMBRDY01',9,1,1))
    for index,(name,key,digest) in enumerate(RESOURCES):
        model=work/name;extract(iso,'a/'+name,model);raw=model.read_bytes()
        if hashlib.sha256(raw).hexdigest()!=digest:raise ValueError('Original startup resource changed: '+name)
        owner=raw[0x44:0x84].split(b'\0',1)[0].decode('ascii');tpl=work/(name+'-');extract(iso,'a/'+owner+'-',tpl)
        kinds={u(raw,u(raw,0x198)+i*64+8) for i in range(u(raw,0x134))}
        if kinds!=({1} if index<3 else {0}):raise ValueError('Source capture/ordinary material ownership changed')
        poses=descriptors(raw);supported=[p['name'] for p in poses if p['supported']]
        atlas=work/(name+'.png');prefix='battle-ready-'+name
        surface=compile_surface_mesh([(model,tpl,supported)],out/(prefix+'.pmanim'),atlas,captured_textures=index<3)
        image=Image.open(atlas);image.resize((512,image.height//2),Image.Resampling.LANCZOS).save(atlas)
        subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(out/(prefix+'.t3x')),str(atlas)],check=True,stdout=subprocess.DEVNULL)
        payload+=struct.pack('<64s64s7If',name.encode(),owner.encode(),key,len(raw),fnv(raw),len(poses),len(supported),surface['textures'],int(index<3),surface['uv_scale'])
        for pose in poses:payload+=struct.pack('<32sIff2I',pose['name'].encode(),pose['loop'],pose['start'],pose['end'],pose['keys'],int(pose['supported']))
        payload+=raw
        records.append(dict(name=name,sourceKey=key,context=1,source='a/'+name,sha256=digest,textureOwner=owner,textureSource='a/'+owner+'-',textureSha256=hashlib.sha256(tpl.read_bytes()).hexdigest(),needsCapture=index<3,poses=poses,surface=surface))
    (out/'battle-startup-resources.bin').write_bytes(payload)
    metadata=dict(language=1,context=1,resources=records,limitation='Descriptor/supported surface/GPU fallback admission only. Captured paper live-field textures, playback, two dormant p_jyabara tangent domains and original async cache/lifetimes remain unbound.',license='Original Nintendo/Intelligent Systems assets for local use; no redistribution rights')
    (out/'battle-startup-resources.json').write_text(json.dumps(metadata,indent=2)+'\n');return metadata

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs');a=p.parse_args();meta=build(a.iso,a.output);print('Original US battle readiness:9 banks/25 descriptors/23 supported poses; live capture/playback unacknowledged')
if __name__=='__main__':main()
