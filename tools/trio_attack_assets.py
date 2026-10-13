#!/usr/bin/env python3
"""Keep original Jump scripts and their DOL constant/data dependencies local."""
import argparse,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,extract
from character_mesh import compile_mesh
from PIL import Image
import shutil,subprocess
from pack_event_scripts import build_pack,DEFAULT_EVENTS

def build(iso=DEFAULT_ISO):
    _,manifest=build_pack(iso,'artifacts/reference/us_symbols.csv',DEFAULT_EVENTS+['partyChristineAttack_NormalAttack@unit_party_christine.o'])
    segments=[]
    with Path(iso).open('rb') as disc:
        disc.seek(0x420);base=struct.unpack('>I',disc.read(4))[0];disc.seek(base);header=disc.read(256)
        # Data/rodata only, never executable PowerPC text. The SDK/global mutable
        # data tail is excluded and belongs to native providers, not this pack.
        for i in range(7,13):
            off,address,size=(struct.unpack_from('>I',header,b+i*4)[0] for b in (0,0x48,0x90))
            if i==11:size=min(size,0x80418000-address)
            if not off or size<=0:continue
            disc.seek(base+off);data=disc.read(size)
            if len(data)!=size or size%4 or address%4:raise ValueError('Invalid DOL data extent')
            # Native managers expose these source address ranges through live
            # typed storage. Do not import original initialized copies over them.
            spans=[(address,data)]
            for lo,hi in ((0x803d8530,0x803d8e70),(0x804151e8,0x804151f0),(0x80415fd0,0x80415fd4)):
                split=[]
                for start,body in spans:
                    end=start+len(body)
                    if start<hi and lo<end:
                        if start<lo:split.append((start,body[:lo-start]))
                        if hi<end:split.append((hi,body[hi-start:]))
                    else:split.append((start,body))
                spans=split
            segments.extend(spans)
        # Immutable Goombella defense/status/default attack/wait data, beyond
        # the mutable-tail cutoff. Import only its exact admitted source span.
        off,va=(struct.unpack_from('>I',header,b+11*4)[0] for b in (0,0x48))
        disc.seek(base+off+0x80418450-va);segments.append((0x80418450,disc.read(0x4c)))
    # Import relocated tik data definitions/scripts; executable REL sections are
    # excluded. These share the existing source-address bank with the intro.
    from rel_event_inventory import read_bindings,inventory_events
    work=Path('local-assets/trio-battle');work.mkdir(parents=True,exist_ok=True)
    relpath=work/'tik.rel';extract(iso,'rel/tik.rel',relpath);raw=relpath.read_bytes()
    local,imports,_=read_bindings(raw);u=lambda o:struct.unpack_from('>I',raw,o)[0]
    sections=[(u(u(16)+i*8),u(u(16)+i*8+4),0x93000000+i*0x100000) for i in range(u(12))]
    def address(o):return next(base+o-(off&~3) for off,size,base in sections if off and (off&~3)<=o<(off&~3)+size)
    for flags,size,base in sections:
        off=flags&~3
        if not off or not size or flags&1:continue
        data=bytearray(raw[off:off+size]);data.extend(b'\0'*(-len(data)%4))
        for site,binding in local.items():
            if off<=site<off+size:struct.pack_into('>I',data,site-off,sections[binding['section']][2]+binding['addend'])
        for site,a in imports.items():
            if off<=site<off+size:struct.pack_into('>I',data,site-off,a)
        segments.append((base,data))
    units=json.loads(Path('romfs/trio-battle.json').read_text())['units'][:3]
    data=struct.pack('<8sI',b'PMTHIT3',3)
    for spec,namespace in zip(units,('unit_kuriboo.o','unit_togekuri.o','unit_patakuri.o')):
        events=inventory_events(raw,'artifacts/reference/us_symbols.csv','tik',['damage_event@'+namespace,'wait_event@'+namespace,('attack_event_patakuri' if namespace=='unit_patakuri.o' else 'attack_event')+'@'+namespace])['events']
        data+=struct.pack('<4I',address(spec['kind']),*[address(e['file_offset']) for e in events])
    data+=struct.pack('<2I',address(0x26460),address(0x3dc40)) # Original setup4 opponent group used by GetExp.
    Path('romfs/trio-attack-units.bin').write_bytes(data)
    segments.sort()
    offset=12+12*len(segments);table=[];payload=[];spans=[]
    for address,data in segments:
        table.append(struct.pack('>III',address,len(data),offset));payload.append(data)
        spans.append(dict(address=address,size=len(data),sha256=hashlib.sha256(data).hexdigest()));offset+=len(data)
    packed=b'PMEVT001'+struct.pack('>I',len(segments))+b''.join(table+payload)
    Path('romfs/trio-attack-events.pmevt').write_bytes(packed)
    manifest.update(format='PMEVT001',segments=spans,size=len(packed),sha256=hashlib.sha256(packed).hexdigest(),conversion='Unchanged original DOL data sections7..12; section11 trimmed at80418000; live manager/context/global ranges excluded; relocated tik non-executable data sections included; no PPC text',limitation='Original source data and dependencies only; native callback coverage and owned runtime fields remain required')
    Path('romfs/trio-attack.json').write_text(json.dumps(manifest,indent=2)+'\n')
    work=Path('local-assets/trio-battle');work.mkdir(parents=True,exist_ok=True)
    model=work/'b_mario';tpl=work/'a_mario.tpl';extract(iso,'a/b_mario',model);extract(iso,'a/a_mario-',tpl)
    poses=['M_S_1','M_R_1','M_W_1','M_J_1B','M_J_1D','M_B_1','M_D_1','M_D_2','M_A_5','M_H_1A','M_H_1B','M_H_1C','M_V_1','M_V_2']
    helpers=['character_mesh.py','character_bake.py','character_node.py','character_animation_pack.py','character_animation_data.py']
    fingerprint=hashlib.sha256(Path(__file__).read_bytes()+model.read_bytes()+tpl.read_bytes()+b''.join(Path('tools',h).read_bytes() for h in helpers)).hexdigest()
    metrics=Path('romfs/trio-attack-mario.json')
    products=[Path('romfs/trio-attack-mario.'+ext) for ext in ('pmmesh','pmanim','t3x','bin')]
    if not(metrics.exists() and all(p.exists() for p in products) and json.loads(metrics.read_text()).get('fingerprint')==fingerprint):
        meta=compile_mesh([(model,tpl,poses)],products[0],work/'battle-mario-atlas.png',products[1])
        height=128
        while height<((meta['textures']+7)//8)*128:height*=2
        Image.open(work/'battle-mario-atlas.png').crop((0,0,1024,height)).resize((512,height//2),Image.Resampling.LANCZOS).save(work/'battle-mario-runtime.png')
        products[3].write_bytes(struct.pack('<8sf',b'PMSUV1',1024/height))
        subprocess.run([shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds','-f','rgba8','-z','auto','-o',str(products[2]),str(work/'battle-mario-runtime.png')],check=True,stdout=subprocess.DEVNULL)
        meta.update(fingerprint=fingerprint,atlasHeight=height,uvScale=1024/height,source='a/b_mario,a/a_mario-',license='Nintendo/Intelligent Systems original assets; no redistribution rights')
        metrics.write_text(json.dumps(meta,indent=2)+'\n')

    model=work/'c_pkr';tpl=work/'c_pkr.tpl';extract(iso,'a/c_pkr',model);extract(iso,'a/c_pkr-',tpl)
    poses=['PKR_S_1','PKR_R_1','PKR_W_1','PKR_J_1A','PKR_A_2B','PKR_D_1','PKR_G_1','PKR_T_1','PKR_Y_1']
    fingerprint=hashlib.sha256(Path(__file__).read_bytes()+model.read_bytes()+tpl.read_bytes()+b''.join(Path('tools',h).read_bytes() for h in helpers)).hexdigest()
    metrics=Path('romfs/trio-attack-goombella.json');products=[Path('romfs/trio-attack-goombella.'+ext) for ext in ('pmmesh','pmanim','t3x','bin')]
    if not(metrics.exists() and all(p.exists() for p in products) and json.loads(metrics.read_text()).get('fingerprint')==fingerprint):
        meta=compile_mesh([(model,tpl,poses)],products[0],work/'headbonk-atlas.png',products[1]);height=128
        while height<((meta['textures']+7)//8)*128:height*=2
        Image.open(work/'headbonk-atlas.png').crop((0,0,1024,height)).resize((512,height//2),Image.Resampling.LANCZOS).save(work/'headbonk-runtime.png')
        products[3].write_bytes(struct.pack('<8sf',b'PMSUV1',1024/height))
        subprocess.run([shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds','-f','rgba8','-z','auto','-o',str(products[2]),str(work/'headbonk-runtime.png')],check=True,stdout=subprocess.DEVNULL)
        meta.update(fingerprint=fingerprint,atlasHeight=height,uvScale=1024/height,source='a/c_pkr,a/c_pkr-',license='Nintendo/Intelligent Systems original assets; no redistribution rights');metrics.write_text(json.dumps(meta,indent=2)+'\n')

if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);build(p.parse_args().iso)
