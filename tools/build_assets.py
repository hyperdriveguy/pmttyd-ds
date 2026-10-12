#!/usr/bin/env python3
"""Compile locally acquired original TTYD assets into a 3DS RomFS pack.
The ISO is opened read-only; original and converted assets are ignored by Git.
"""
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import struct
from PIL import Image,ImageDraw
from iso_assets import extract,convert_tpl,DEFAULT_ISO
from character_bake import bake
from character_mesh import compile_mesh
from map_convert import convert
from audio_convert import stm,sample_directory
from room_convert import exits,damage_returns,resolve_damage_returns,road_hit
ROOT=Path(__file__).resolve().parents[1]
OUTPUT=ROOT/'romfs';WORK=ROOT/'local-assets';ISO=os.environ.get('TTYD_ISO',DEFAULT_ISO)

def main():
    tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
    source=Path(ISO)
    if not source.is_file():raise SystemExit('Original asset pack requires TTYD_ISO pointing to your GameCube ISO')
    scripts=[Path(__file__),ROOT/'tools/iso_assets.py',ROOT/'tools/character_bake.py',ROOT/'tools/character_node.py',ROOT/'tools/character_mesh.py',ROOT/'tools/map_convert.py',ROOT/'tools/map_animation.py',ROOT/'tools/audio_convert.py',ROOT/'tools/room_convert.py']
    fingerprint=hashlib.sha256(b''.join(p.read_bytes() for p in scripts)+str((source.stat().st_size,source.stat().st_mtime_ns)).encode()).hexdigest()
    manifest=OUTPUT/'manifest.json'
    required=['characters.pmmesh','characters-mesh.t3x','characters.t3x','sky.t3x','map.vertices','map.batches','rogueport.wav','battle.wav','victory.wav','stage.vertices','stage.batches','map.walls','map.ground','gor_00.vertices','gor_02.vertices','gor_03.vertices','gor_04.vertices','map.uvanim','stage.uvanim','gor_00.uvanim','gor_02.uvanim','gor_03.uvanim','gor_04.uvanim','rooms.bin','map.meshanim','stage.meshanim','gor_00.meshanim','gor_02.meshanim','gor_03.meshanim','gor_04.meshanim']
    required.extend(prefix+'.returns' for prefix in ['map','gor_00','gor_02','gor_03','gor_04'])
    required.extend(prefix+'.head' for prefix in ['map','gor_00','gor_02','gor_03','gor_04'])
    required.extend(prefix+'.camera' for prefix in ['map','gor_00','gor_02','gor_03','gor_04'])
    if manifest.exists() and all((OUTPUT/p).exists() for p in required):
        if json.loads(manifest.read_text()).get('fingerprint')==fingerprint:return
    WORK.mkdir(parents=True,exist_ok=True);OUTPUT.mkdir(exist_ok=True)
    records=[]
    def asset(name,local):
        output=WORK/local;extract(ISO,name,output)
        records.append(json.loads(output.with_suffix(output.suffix+'.provenance.json').read_text()));return output
    def texture(image,output,fmt='rgba8'):
        subprocess.run([tex3ds,'-f',fmt,'-z','auto','-o',str(output),str(image)],check=True,stdout=subprocess.DEVNULL)
    mario=asset('a/a_mario-','a_mario.tpl');convert_tpl(mario,WORK/'mario-original.png',0)
    mario_model=asset('a/a_mario','a_mario')
    rear_model=asset('a/a_mario_r','a_mario_r');rear_tpl=asset('a/a_mario_r-','a_mario_r.tpl')
    character_mesh=compile_mesh([(mario_model,mario,['M_S_1','M_W_1','M_R_1','M_J_1B','M_J_1C']),
                                (rear_model,rear_tpl,['M_S_1R','M_W_1R','M_R_1R','M_J_1BR','M_J_1CR'])],
                               OUTPUT/'characters.pmmesh',WORK/'characters-mesh.png')
    texture(WORK/'characters-mesh.png',OUTPUT/'characters-mesh.t3x')
    atlas=Image.new('RGBA',(512,256))
    poses=[]
    for index,name,anim in [(1,'c_pkr','PKR_Z_1'),(2,'c_kanbu','Z_1'),(3,'c_kinopio','KNP_Z_1')]:
        model=asset('a/'+name,name);tpl=asset('a/'+name+'-',name+'.tpl')
        target=WORK/f'{name}-idle.png';poses.append(bake(model,tpl,target,anim))
        atlas.alpha_composite(Image.open(target),(index*128,0))
    marioimage=Image.open(WORK/'mario-original.png').convert('RGBA')
    bbox=marioimage.getbbox();marioimage=marioimage.crop(bbox);marioimage.thumbnail((118,118),Image.Resampling.LANCZOS)
    atlas.alpha_composite(marioimage,((128-marioimage.width)//2,123-marioimage.height))
    for slot,source_index in [(1,2),(2,12)]:
        path=WORK/f'mario-pose-{source_index}.png';convert_tpl(mario,path,source_index)
        image=Image.open(path).convert('RGBA');image=image.crop(image.getbbox());image.thumbnail((118,118),Image.Resampling.LANCZOS)
        atlas.alpha_composite(image,(slot*128+(128-image.width)//2,251-image.height))
    draw=ImageDraw.Draw(atlas);draw.ellipse((5,138,123,182),fill=(35,29,33,65));draw.rectangle((480,224,511,255),fill='white')
    atlas.save(WORK/'characters.png');texture(WORK/'characters.png',OUTPUT/'characters.t3x')
    sky=asset('b/gor_01.tpl','sky.tpl');convert_tpl(sky,WORK/'sky.png')
    Image.open(WORK/'sky.png').convert('RGB').resize((256,256),Image.Resampling.LANCZOS).save(WORK/'sky-runtime.png');texture(WORK/'sky-runtime.png',OUTPUT/'sky.t3x','rgb565')
    camera=asset('m/gor_01/c','gor_01.c');shutil.copyfile(camera,OUTPUT/'map.camera')
    model=asset('m/gor_01/d','gor_01.d');tpl=asset('m/gor_01/t','gor_01.tpl');summary=convert(model,WORK/'map',mesh_ambient=('hae',))
    for filename in ['map.vertices','map.batches','map.walls','map.ground','map.head','map.uvanim','map.meshanim']:shutil.copyfile(WORK/'map'/filename,OUTPUT/filename)
    for index,name in enumerate(summary['textures']):
        path=WORK/'map'/f'texture-{index}.png';convert_tpl(tpl,path,index)
        image=Image.open(path).convert('RGBA');w,h=image.size
        # Power-of-two dimensions retain repeating GX texture coordinates.
        width=min(128,1<<(w-1).bit_length());height=min(128,1<<(h-1).bit_length())
        image.resize((width,height),Image.Resampling.LANCZOS).save(path);texture(path,OUTPUT/f'map-{index}.t3x')
    rooms={'gor_01':summary}
    for room in ['gor_00','gor_02','gor_03','gor_04']:
        camera=asset(f'm/{room}/c',room+'.c');shutil.copyfile(camera,OUTPUT/(room+'.camera'))
        room_model=asset(f'm/{room}/d',room+'.d');room_tpl=asset(f'm/{room}/t',room+'.tpl')
        # Original gor_04 init ELSE for entry != hikousen_return hides these groups.
        # Only ordinary road entries are currently implemented for this room.
        hidden=('gondora_2','hako_1') if room=='gor_04' else ()
        ambient=('mizu','noname','ie_01_light') if room=='gor_02' else ('nami','nami_02','hiki_nami') if room=='gor_00' else ()
        mesh_ambient=('nami',) if room=='gor_00' else ()
        info=convert(room_model,WORK/room,hidden=hidden,ambient=ambient,mesh_ambient=mesh_ambient);rooms[room]=info
        for ext in ['vertices','batches','walls','ground','head','uvanim','meshanim']:shutil.copyfile(WORK/room/f'map.{ext}',OUTPUT/f'{room}.{ext}')
        for index,name in enumerate(info['textures']):
            path=WORK/room/f'texture-{index}.png';convert_tpl(room_tpl,path,index)
            image=Image.open(path).convert('RGBA');w,h=image.size
            image.resize((min(128,1<<(w-1).bit_length()),min(128,1<<(h-1).bit_length())),Image.Resampling.LANCZOS).save(path)
            texture(path,OUTPUT/f'{room}-{index}.t3x')
    room_rel=asset('rel/gor.rel','gor.rel')
    original_exits=exits(room_rel);return_tables=damage_returns(room_rel)
    (WORK/'damage-return-tables.json').write_text(json.dumps(return_tables,indent=2)+'\n')
    resolved_returns={}
    for table in return_tables:
        room={0x2f364:'gor_00',0x4d6d0:'gor_02'}.get(table['call_offset'])
        if room is None:raise ValueError('Unreviewed damage-return activation call')
        resolved,bound=resolve_damage_returns(rooms[room]['collision_nodes'],table['entries'])
        resolved_returns[room]={'entries':resolved,'bindings':bound}
    for room,prefix in [('gor_01','map'),('gor_00','gor_00'),('gor_02','gor_02'),('gor_03','gor_03'),('gor_04','gor_04')]:
        bound=resolved_returns.get(room,{}).get('bindings',{})
        with (OUTPUT/(prefix+'.returns')).open('wb') as out:
            out.write(struct.pack('<4sII',b'RPOS',1,len(bound)))
            for node,point in sorted(bound.items()):out.write(struct.pack('<I3f',node,*point))
    # Group consecutive original BeroEntry records. Match their exit names to
    # the map's geometric road markers, with reciprocal destinations checked.
    groups=[]
    for entry in original_exits:
        if not groups or entry['offset']!=groups[-1][-1]['offset']+60:groups.append([])
        groups[-1].append(entry)
    order=['gor_01','gor_00','gor_02','gor_03','gor_04']
    # Solve all tables together: gor_04 has both north/south markers, so
    # marker subsets alone cannot distinguish its table from the harbor's.
    # Require distinct tables and original reciprocal links across the pack.
    import itertools
    choices=[]
    for name,info in rooms.items():
        names={m['name'] for m in info['exit_markers'] if m['name'] in ('n_bero','s_bero','e_bero','w_bero')}
        candidates=[g for g in groups if {e['name'] for e in g}<=names]
        if candidates:
            size=max(map(len,candidates));candidates=[g for g in candidates if len(g)==size]
        choices.append(candidates)
    solutions=[]
    for tables in itertools.product(*choices):
        if len({g[0]['offset'] for g in tables})!=len(tables):continue
        assignment=dict(zip(rooms,tables))
        valid=True
        for source,table in assignment.items():
            for entry in table:
                if entry['target'] not in assignment:continue
                reverse=next((e for e in assignment[entry['target']] if e['name']==entry['arrival']),None)
                if not reverse or reverse['target']!=source or reverse['arrival']!=entry['name']:valid=False
        if valid:solutions.append(assignment)
    if len(solutions)!=1:raise ValueError(f'Ambiguous original reciprocal room tables: {len(solutions)} solutions')
    matched=solutions[0]
    links=[]
    for source,group in matched.items():
        for entry in group:
            target=entry['target']
            if target not in matched:continue # Not yet packaged, never invent an endpoint.
            reciprocal=next((e for e in matched[target] if e['name']==entry['arrival']),None)
            if not reciprocal or reciprocal['target']!=source or reciprocal['arrival']!=entry['name']:raise ValueError('Nonreciprocal road link')
            marker=road_hit(rooms[source]['collision_nodes'],entry['name'])
            dest=road_hit(rooms[target]['collision_nodes'],entry['arrival'])
            c=marker['center'];d=dest['center']
            direction={'n_bero':(0,-1),'s_bero':(0,1),'e_bero':(1,0),'w_bero':(-1,0)}
            dx,dz=direction[entry['arrival']]
            headings={'n_bero':0,'e_bero':2,'s_bero':4,'w_bero':6}
            links.append({'source':source,'exit':entry['name'],'target':target,'arrival':entry['arrival'],
                'source_node':marker['source_node'],'arrival_node':dest['source_node'],
                'direction':headings[entry['name']],'arrival_direction':headings[entry['arrival']],
                'center':c,'spawn':[d[0]-dx*.3,d[1],d[2]-dz*.3],'rel_offset':entry['offset']})
    (WORK/'room-links.json').write_text(json.dumps(links,indent=2))
    with (OUTPUT/'rooms.bin').open('wb') as out:
        out.write(struct.pack('<4sII',b'ROAD',1,len(links)))
        for link in links:out.write(struct.pack('<II5f4If',order.index(link['source']),order.index(link['target']),link['center'][0],link['center'][2],link['spawn'][0],link['spawn'][2],link['spawn'][1],link['source_node'],link['arrival_node'],link['direction'],link['arrival_direction'],link['center'][1]))
    stage_model=asset('m/stg_01/d','stage.d');stage_tpl=asset('m/stg_01/t','stage.tpl')
    stage_summary=convert(stage_model,WORK/'stage',hidden=('stg__MASUKU','stg_kaaten_01_a','stg_kaaten_01_b','switch_dai','switch_mark','switch_osu'))
    for extension in ['vertices','batches','uvanim','meshanim']:shutil.copyfile(WORK/'stage'/f'map.{extension}',OUTPUT/f'stage.{extension}')
    for index,name in enumerate(stage_summary['textures']):
        path=WORK/'stage'/f'texture-{index}.png';convert_tpl(stage_tpl,path,index)
        image=Image.open(path).convert('RGBA');w,h=image.size
        image.resize((min(128,1<<(w-1).bit_length()),min(128,1<<(h-1).bit_length())),Image.Resampling.LANCZOS).save(path)
        texture(path,OUTPUT/f'stage-{index}.t3x')
    music=[]
    for name,disc in [('rogueport','stg_gor1_32k'),('battle','btl_zak1_32k'),('victory','btl_win1_32k')]:
        path=asset(f'sound/stream/{disc}.stm',name+'.stm');music.append(stm(path,OUTPUT/f'{name}.wav'))
    # Preserve original banks, with a directory index for subsequent macro mapping.
    for suffix in ['sdir','samp','pool','proj','etbl','slib']:
        asset('sound/proj/pmario.'+suffix,'pmario.'+suffix)
    directory=sample_directory(WORK/'pmario.sdir')
    (WORK/'sample-directory.json').write_text(json.dumps(directory,indent=2)+'\n')
    # Retire synthetic effects: unresolved original macros remain silent.
    for filename in ['confirm.wav','jump.wav','hit.wav','success.wav']:
        (OUTPUT/filename).unlink(missing_ok=True)
    manifest.write_text(json.dumps({'fingerprint':fingerprint,'source_disc':'G8ME01','assets':records,
        'characters':poses,'character_mesh':character_mesh,'map':summary,'overworld_rooms':rooms,'road_links':links,'camera_containers':{'map':'m/gor_01/c',**{room:'m/'+room+'/c' for room in ['gor_00','gor_02','gor_03','gor_04']}},'damage_return_tables':return_tables,'resolved_damage_returns':resolved_returns,'battle_stage':stage_summary,'music':music,'sample_bank_entries':len(directory),
        'sound_effect_status':'Original banks preserved; macro-to-event mapping not yet implemented. No synthetic replacements.',
        'conversion':'Original character idle poses and Mario jump/hammer textures baked to RGBA8 atlas; static Rogueport geometry; map textures capped at 128px; DSP music decoded losslessly at original rate'},indent=2)+'\n')
    print(f'Built original asset pack: {summary["triangles"]} map triangles, {len(directory)} preserved samples')
if __name__=='__main__':main()
