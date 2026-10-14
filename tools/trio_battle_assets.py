#!/usr/bin/env python3
"""Import original setup4 unit data, Frankly init/entry/phase and stage resources."""
import argparse,json,struct,hashlib,shutil,subprocess
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO,extract,convert_tpl
from rel_event_inventory import inventory_events,read_bindings
from map_convert import convert
from underground_event_assets import SERVICES as FIELD_SERVICES
SERVICES=FIELD_SERVICES+['lecture_init','btlevtcmd_SetEventWait','btlevtcmd_SetEventEntry','btlevtcmd_SetEventPhase','btlevtcmd_SetEventDamage','btlevtcmd_SetEventConfusion','btlevtcmd_GetUnitId','_battle_set_lecture_unit_id','btlevtcmd_SetUnitWork','btlevtcmd_SetPos','btlevtcmd_ChangeFaceDirection','btlevtcmd_SetMaxMoveCount','btlevtcmd_StartWaitEvent','btlevtcmd_AnimeChangePose','btlevtcmd_MovePosition','btlevtcmd_CheckPhase','btlevtcmd_get_turn','btlevtcmd_StopWaitEvent','btlevtcmd_SetTalkPose','btlevtcmd_SetStayPose','btlevtcmd_StatusWindowOnOff','evt_btl_camera_set_mode','evt_btl_camera_set_moveto','evt_msg_toge']
def build(iso):
    work=Path('local-assets/trio-battle');work.mkdir(parents=True,exist_ok=True)
    path=work/'tik.rel';extract(iso,'rel/tik.rel',path);raw=path.read_bytes()
    names=['init_event@unit_lecture_frankli_kuriboo.o','entry_event@unit_lecture_frankli_kuriboo.o','phase_event@unit_lecture_frankli_kuriboo.o']
    events=inventory_events(raw,'artifacts/reference/us_symbols.csv','tik',names)['events'];local,imports,foreign=read_bindings(raw)
    u=lambda o:struct.unpack_from('>I',raw,o)[0]
    sections=[(u(u(16)+i*8)&~3,u(u(16)+i*8+4),0x93000000+i*0x100000) for i in range(u(12))]
    def address(o):return next(base+o-off for off,size,base in sections if off and off<=o<off+size)
    segments=[]
    for off,size,base in sections:
        if not off or not size:continue
        data=bytearray(raw[off:off+size]);data.extend(b'\0'*(-len(data)%4))
        for site,b in local.items():
            if off<=site<off+size:struct.pack_into('>I',data,site-off,sections[b['section']][2]+b['addend'])
        for site,a in imports.items():
            if off<=site<off+size:struct.pack_into('>I',data,site-off,a)
        segments.append((base,data))
    callbacks={}
    for event in events:
        for c in event['commands']:
            if c['opcode']!=91:continue
            op=c['operands'][0];name=op['annotation']
            if name not in SERVICES:raise ValueError('Unadmitted battle service '+name)
            a=op.get('dol_address',address(op['rel_offset']) if 'rel_offset' in op else 0);callbacks[a]=SERVICES.index(name)
    offset=12+12*len(segments);table=bytearray();payload=bytearray()
    for base,data in segments:table+=struct.pack('>III',base,len(data),offset);payload+=data;offset+=len(data)
    Path('romfs/trio-battle-events.pmevt').write_bytes(b'PMEVT001'+struct.pack('>I',len(segments))+table+payload)
    meta=struct.pack('<8s4I',b'PMBTRIO1',*[address(e['file_offset']) for e in events],len(callbacks))
    meta+=b''.join(struct.pack('<II',a,t) for a,t in sorted(callbacks.items()));Path('romfs/trio-battle-program.bin').write_bytes(meta)
    units=[]
    for index in range(4):
        site=0x263a0+index*0x30;kind=local[site]['file_offset'];hp,fp=struct.unpack_from('>hh',raw,kind+8);position=struct.unpack_from('>3f',raw,site+0xc)
        part=local[kind+0xb8]['file_offset'];defense=local[part+0x38]['file_offset'];defattr=local[part+0x3c]['file_offset']
        units.append(dict(defense=list(struct.unpack_from('>5b',raw,defense)),defenseAttributes=list(raw[defattr:defattr+5]),partAttributes=u(part+0x40),counterAttributes=u(part+0x44),index=index,record=site,kind=kind,type=u(kind),hp=hp,fp=fp,position=position,attributes=u(kind+0xac),partPosition=struct.unpack_from('>3f',raw,part+0xc),hitOffset=struct.unpack_from('>3f',raw,part+0x18),samplingOffset=struct.unpack_from('>3f',raw,part+0x24),forwardBias=struct.unpack_from('>h',raw,site+0x1a)[0],nameKey=raw[local[kind+4]['file_offset']:].split(b'\0',1)[0].decode('ascii')))
    data=struct.pack('<8sI',b'PMBUNIT2',4)
    for r in units:data+=struct.pack('<Ihh3fI',r['type'],r['hp'],r['fp'],*r['position'],r['attributes'])+struct.pack('<5b5BII',*r['defense'],*r['defenseAttributes'],r['partAttributes'],r['counterAttributes'])+struct.pack('<9fh64s',*r['partPosition'],*r['hitOffset'],*r['samplingOffset'],r['forwardBias'],r['nameKey'].encode('ascii'))
    Path('romfs/trio-battle-units.bin').write_bytes(data)
    stage_descriptors=[address(0x25c08+i*0x18) for i in range(5)]+[0x80417ca0,0x80417cb8]
    Path('romfs/trio-stage-objects.bin').write_bytes(struct.pack('<8s8I',b'PMSTOBJ1',address(0x2619c),*stage_descriptors))
    # Retain the actual default weapon targeting words and name IDs from main.dol.
    commands=[]
    with Path(iso).open('rb') as disc:
        disc.seek(0x420);dol=struct.unpack('>I',disc.read(4))[0];disc.seek(dol);header=disc.read(256)
        def read_dol(address,size):
            for i in range(18):
                off,va,length=(struct.unpack_from('>I',header,b+i*4)[0] for b in (0,0x48,0x90))
                if va<=address and address+size<=va+length:
                    disc.seek(dol+off+address-va);data=disc.read(size)
                    if len(data)!=size:raise ValueError('Truncated DOL weapon')
                    return data
            raise ValueError('Invalid DOL weapon address')
        for address in (0x803538a8,0x80353de8,0x80393ea8):
            record=read_dol(address,0xc0);name=struct.unpack_from('>I',record)[0]
            key=read_dol(name,64).split(b'\0',1)[0].decode('ascii')
            admission,attributes=struct.unpack_from('>II',record,0x64)
            commands.append(dict(address=address,admission=admission,attributes=attributes,nameKey=key,icon=struct.unpack_from('>H',record,4)[0],sha256=hashlib.sha256(record).hexdigest()))
    Path('romfs/trio-battle-commands.bin').write_bytes(struct.pack('<8sI',b'PMTCMD2',3)+b''.join(struct.pack('<4I64s',r['address'],r['admission'],r['attributes'],r['icon'],r['nameKey'].encode('ascii')) for r in commands))
    tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
    for room in ('bti_01','stg_00_1'):
        d=work/(room+'.d');t=work/(room+'.tpl');extract(iso,'m/'+room+'/d',d);extract(iso,'m/'+room+'/t',t)
        manifest=Path('romfs/'+room+'-battle.json');fingerprint=hashlib.sha256(d.read_bytes()+t.read_bytes()+Path(__file__).read_bytes()+Path('tools/map_convert.py').read_bytes()).hexdigest()
        if manifest.exists() and json.loads(manifest.read_text()).get('fingerprint')==fingerprint:continue
        info=convert(d,work/room,hidden=('escape_wall','spotlight','if_body','if_wire'),object_bindings=('escape_wall','spotlight'),map_objects=True) if room=='bti_01' else convert(d,work/room,scene_root=('A1','isi_1','isi_2','B_mati','C'),scene_offsets={'A1':(0,0,-.5),'isi_1':(-2.02,0,-.17),'isi_2':(2,0,-.3)},object_bindings=('isi_1','isi_2','A1','B_mati','C'),map_objects=True)
        for ext in ('vertices','batches','uvanim','meshanim','houseobj','mapobj'):shutil.copyfile(work/room/('map.'+ext),Path('romfs')/(room+'.'+ext))
        for index,_ in enumerate(info['textures']):
            image=work/f'{room}-{index}.png';convert_tpl(t,image,index);bitmap=Image.open(image).convert('RGBA');w,h=bitmap.size
            bitmap.resize((min(128,1<<(w-1).bit_length()),min(128,1<<(h-1).bit_length())),Image.Resampling.LANCZOS).save(image)
            subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(Path('romfs')/(room+'-'+str(index)+'.t3x')),str(image)],check=True,stdout=subprocess.DEVNULL)
        manifest.write_text(json.dumps(dict(fingerprint=fingerprint,map=info,source='m/'+room,license='Nintendo/Intelligent Systems resources; no redistribution rights'),indent=2)+'\n')
    Path('romfs/trio-battle.json').write_text(json.dumps(dict(events=events,units=units,commands=commands,callbacks=callbacks,source='rel/tik.rel',sha256=hashlib.sha256(raw).hexdigest(),foreign=foreign,license='Nintendo/Intelligent Systems resources; no redistribution rights'),indent=2)+'\n')
if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);build(p.parse_args().iso)
