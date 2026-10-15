#!/usr/bin/env python3
"""Export original map NPC resources, independent of finite presentation records.

The tagged map selects original tribe rows; model names share one resource bank
across areas. Full descriptor order/CPU clips and raw MODEL/TPL are retained.
Atlas PNGs are conversion evidence, not GPU publication or facing readiness.
"""
import argparse
import hashlib
import json
import re
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO, extract
from field_actor_assets import tagged_npcs
from battle_pose_directory import source_directory, write_directory
from character_surface_mesh import compile_surface_mesh

ROLES=('initial','pose_c','stay','talk','walk','run','pose_20','pose_24')

def source_tribes(iso):
    with Path(iso).open('rb') as disc:
        if disc.read(8)!=b'G8ME01\0\0':raise ValueError('US revision0 required')
        disc.seek(0x420);dol=struct.unpack('>I',disc.read(4))[0];disc.seek(dol);header=disc.read(256)
        def read(address,size):
            matches=[]
            for i in range(18):
                off,base,length=(struct.unpack_from('>I',header,p+4*i)[0] for p in (0,0x48,0x90))
                if base<=address and address+size<=base+length:matches.append(dol+off+address-base)
            if len(matches)!=1:raise ValueError('Tribe data outside original DOL')
            disc.seek(matches[0]);data=disc.read(size)
            if len(data)!=size:raise ValueError('Truncated original tribe data')
            return data
        def string(address,encoding):
            if not address:return ''
            data=bytearray()
            for i in range(256):
                c=read(address+i,1)[0]
                if not c:return data.decode(encoding)
                data.append(c)
            raise ValueError('Unterminated original tribe string')
        rows={}
        for i in range(1024):
            address=0x803316d0+i*0x54;raw=read(address,0x54);words=struct.unpack_from('>10I',raw)
            if not words[0]:return rows
            name=string(words[0],'shift_jis')
            # npcGetTribe returns the first exact row, preserving duplicate names.
            if name not in rows:
                rows[name]=dict(address=address,model=string(words[1],'ascii'),
                    poses={role:string(words[j+2],'ascii') for j,role in enumerate(ROLES)},
                    sha256=hashlib.sha256(raw).hexdigest())
        raise ValueError('Missing original tribe terminator')

def safe_name(name):
    if not re.fullmatch(r'[A-Za-z0-9_]+',name):raise ValueError('Unsafe original resource/map name')
    return name

def export_model(iso,model_name,work,output):
    safe_name(model_name);asset='field-native-'+model_name
    model=work/model_name;texture=work/(model_name+'.tpl')
    extract(iso,'a/'+model_name,model)
    original,bank,names=source_directory(model.read_bytes())
    if original!=model_name:raise ValueError('Original MODEL identity differs from request')
    safe_name(bank);extract(iso,'a/'+bank+'-',texture)
    helpers=['field_npc_resource_assets.py','battle_pose_directory.py','character_surface_mesh.py','character_mesh.py','character_bake.py','character_node.py','character_animation_pack.py','character_animation_data.py','iso_assets.py']
    fingerprint=hashlib.sha256(b''.join(Path('tools',p).read_bytes() for p in helpers)+model.read_bytes()+texture.read_bytes()).hexdigest()
    atlas=work/(asset+'.png')
    manifest=output/(asset+'.json');products=[output/(asset+'.'+s) for s in ('pmanim','pose-directory.bin','draw-model.bin','draw-textures.bin')]
    if manifest.is_file() and all(p.is_file() for p in products):
        cached=json.loads(manifest.read_text())
        if cached.get('fingerprint')==fingerprint and atlas.is_file() and hashlib.sha256(atlas.read_bytes()).hexdigest()==cached.get('atlas_evidence',{}).get('sha256') and all(hashlib.sha256(p.read_bytes()).hexdigest()==cached['products'][p.name]['sha256'] for p in products):return cached
    # Compile all original descriptors. Unsupported source services are errors,
    # never substituted idle clips or aliases. A manifest publishes only success.
    result=compile_surface_mesh([(model,texture,names)],products[0],atlas)
    write_directory(model,products[1],texture)
    result.update(fingerprint=fingerprint,model=model_name,texture=bank,asset=asset,
        original_index0=names[0],source_descriptor_order=names,metadata_format='PMPOSE02',
        atlas_evidence=dict(path=str(atlas),bytes=atlas.stat().st_size,sha256=hashlib.sha256(atlas.read_bytes()).hexdigest()),
        assets=[json.loads(p.read_text()) for p in (model.with_suffix('.provenance.json'),texture.with_suffix('.tpl.provenance.json'))],
        products={p.name:dict(bytes=p.stat().st_size,sha256=hashlib.sha256(p.read_bytes()).hexdigest()) for p in products},
        readiness='Real CPU bank and immutable raw MODEL/TPL. No GPU/facing/Main or Game actor registration acknowledgment.',
        license='Local original Nintendo/Intelligent Systems assets; no redistribution rights')
    manifest.write_text(json.dumps(result,indent=2)+'\n');print('Original field resource:',model_name,len(names),'poses',flush=True)
    return result

def build(iso,areas=('gor_02',)):
    work=Path('local-assets/field-native');work.mkdir(parents=True,exist_ok=True)
    output=Path('romfs');output.mkdir(parents=True,exist_ok=True);tribes=source_tribes(iso)
    maps=[];models={}
    for area in dict.fromkeys(areas):
        safe_name(area);path=work/(area+'.s');extract(iso,'m/'+area+'/s',path)
        actors=[]
        for name,actor in tagged_npcs(path.read_bytes().decode('shift_jis')).items():
            if actor['tribe'] not in tribes:raise ValueError('Original map tribe not found: '+actor['tribe'])
            row=tribes[actor['tribe']];model=row['model'];models.setdefault(model,None)
            actors.append(dict(name=name,tribe=actor['tribe'],**row))
        maps.append(dict(area=area,source=json.loads(path.with_suffix('.s.provenance.json').read_text()),actors=actors))
    models.setdefault('hoshi',None) # Actual animPoseEntry fallback, no fabricated failure.
    for model in models:models[model]=export_model(iso,model,work,output)
    result=dict(areas=maps,resources={name:dict(asset=m['asset'],descriptors=m['source_descriptor_order'],fingerprint=m['fingerprint']) for name,m in models.items()},
        policy='Tagged-map resource dependency census only; source setup/init/story visibility, registration, Main/GPU and room lifecycle remain separate.')
    (output/'field-npc-resources.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
    return result

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--iso',default=DEFAULT_ISO);parser.add_argument('--area',action='append')
    args=parser.parse_args();build(args.iso,args.area or ('gor_02',))
