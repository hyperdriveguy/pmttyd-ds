#!/usr/bin/env python3
"""Package original underground trio/footstep event data and actor assets locally."""
import argparse,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,extract
from rel_event_inventory import inventory_events,read_bindings
from professor_event_assets import SERVICES as PROFESSOR_SERVICES
from field_actor_assets import compile_model
SERVICES=PROFESSOR_SERVICES+['evt_mario_set_dir_npc','evt_party_set_dir_npc','evt_mario_set_pos','evt_party_set_pos','evt_npc_set_run_anim','evt_npc_set_damage_anim','evt_snd_bgmon_f']
def build(iso):
    work=Path('local-assets/underground');work.mkdir(parents=True,exist_ok=True)
    path=work/'tik.rel';extract(iso,'rel/tik.rel',path);raw=path.read_bytes()
    if hashlib.sha256(raw).hexdigest()!='ca30b06f0b7176dbe364349f239ef3ddfe66650e50ea1246921bd2a368a9dac7':raise ValueError('Unexpected original tik REL')
    events=inventory_events(raw,'artifacts/reference/us_symbols.csv','tik',['gorotuki_sanninngumi','ashioto@tik_01.o'])['events']
    local,imports,foreign=read_bindings(raw);u=lambda o:struct.unpack_from('>I',raw,o)[0]
    sections=[(u(u(16)+8*i)&~3,u(u(16)+8*i+4),0x92000000+i*0x100000) for i in range(u(12))]
    def address(o):
        found=[base+o-off for off,size,base in sections if off and off<=o<off+size]
        if len(found)!=1:raise ValueError('Unbound original address')
        return found[0]
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
            if name not in SERVICES:raise ValueError('Unadmitted service '+name)
            callbacks[op['dol_address']]=SERVICES.index(name)
    table=bytearray();payload=bytearray();offset=12+12*len(segments)
    for base,data in segments:table+=struct.pack('>III',base,len(data),offset);payload+=data;offset+=len(data)
    Path('romfs/underground-events.pmevt').write_bytes(b'PMEVT001'+struct.pack('>I',len(segments))+table+payload)
    names=[next(o['rel_offset'] for c in events[0]['commands'] for o in c['operands'] if (o.get('annotation') or '').startswith('str_'+name+'_tik_')) for name in ('クリボー１','クリボー２','クリボー３')]
    metadata=struct.pack('<8s5I',b'PMTRIO1',address(events[0]['file_offset']),*[address(n) for n in names],len(callbacks))
    metadata+=b''.join(struct.pack('<II',a,t) for a,t in sorted(callbacks.items()))
    Path('romfs/underground-program.bin').write_bytes(metadata)
    poses=['KUR_Z_2']+[prefix+'_'+pose for prefix in ('KUR','PTK','TGK') for pose in ('S_1','T_1','W_1','R_1','D_1')]
    poses+=['KUR_R_2'] # Original grounded pose41; authored PTK_R_2 misses this bank and uses descriptor0.
    scale=compile_model(iso,'underground-goombas','c_kuribo',poses)
    Path('romfs/underground-goombas.bin').write_bytes(struct.pack('<f',scale))
    Path('romfs/underground-trio.json').write_text(json.dumps(dict(events=events,callbacks=callbacks,actor_names=names,uv_scale=scale,source='rel/tik.rel,a/c_kuribo,a/c_kuribo-',foreign=foreign,license='Nintendo/Intelligent Systems resources; no redistribution rights'),indent=2)+'\n')
if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);build(p.parse_args().iso)
