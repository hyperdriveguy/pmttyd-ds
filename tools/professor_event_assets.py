#!/usr/bin/env python3
"""Relocate original gor REL data into an explicitly authored native address map.

No event commands or strings are rewritten except source ADDR32 relocations.
Only kurihakase_first_talk is admitted by the scoped native callback catalog.
"""
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO, extract
from rel_event_inventory import read_bindings, inventory_events

SERVICES = ['evt_cam3d_evt_off','evt_cam3d_evt_set','evt_cam_shake','evt_door_end_wait','evt_eff_fukidashi',
 'evt_item_entry','evt_item_flag_on','evt_item_set_position','evt_item_set_scale','evt_mapobj_flag_onoff',
 'evt_mario_dispflag_onoff','evt_mario_get_pos','evt_mario_key_onoff','evt_mario_mov_pos2','evt_mario_set_dir',
 'evt_mario_set_party_pos','evt_mario_set_pose','evt_msg_print','evt_msg_print_party','evt_npc_flag_onoff',
 'evt_npc_get_position','evt_npc_jump_position_nohit','evt_npc_move_position','evt_npc_set_position','evt_npc_set_ry',
 'evt_party_dispflg_onoff','evt_party_move_pos2','evt_party_run','evt_party_set_dir','evt_party_stop',
 'evt_snd_bgmoff','evt_snd_bgmon','evt_snd_envoff','evt_snd_envon','evt_snd_sfxon_3d']

def build(iso=DEFAULT_ISO):
    work=Path('local-assets/professor');work.mkdir(parents=True,exist_ok=True)
    source=work/'gor.rel';extract(iso,'rel/gor.rel',source);raw=source.read_bytes()
    if hashlib.sha256(raw).hexdigest()!='d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a':raise ValueError('Unexpected US gor REL')
    event=inventory_events(raw,'artifacts/reference/us_symbols.csv','gor',['kurihakase_first_talk'])['events'][0]
    if event['sha256']!='9e27be37f4d93218ea9204cf25d17eef07876c130d25f13c8b5470739f545511':raise ValueError('First-talk source changed')
    local,imports,foreign=read_bindings(raw)
    u=lambda o:struct.unpack_from('>I',raw,o)[0]
    sections=[]
    for i in range(u(12)):
        off,size=struct.unpack_from('>II',raw,u(16)+i*8);off&=~3
        sections.append((off,size,0x90000000+i*0x100000))
    def address(offset):
        found=[base+offset-off for off,size,base in sections if off and off<=offset<off+size]
        if len(found)!=1:raise ValueError('Ambiguous REL source address')
        return found[0]
    segments=[]
    for off,size,base in sections:
        if not off or not size:continue
        data=bytearray(raw[off:off+size]);data.extend(b'\0'*(-len(data)%4))
        for site,binding in local.items():
            if off<=site<off+size:
                value=sections[binding['section']][2]+binding['addend']
                struct.pack_into('>I',data,site-off,value)
        for site,value in imports.items():
            if off<=site<off+size:struct.pack_into('>I',data,site-off,value)
        segments.append((base,bytes(data)))
    called={}
    for command in event['commands']:
        if command['opcode']!=91:continue
        operand=command['operands'][0];name=operand.get('annotation')
        if name not in SERVICES or 'dol_address' not in operand:raise ValueError('Unadmitted first-talk callback')
        called[operand['dol_address']]=SERVICES.index(name)
    table=bytearray();payload=bytearray();offset=12+12*len(segments)
    for base,data in segments:
        table+=struct.pack('>III',base,len(data),offset);payload+=data;offset+=len(data)
    Path('romfs/professor-events.pmevt').write_bytes(b'PMEVT001'+struct.pack('>I',len(segments))+table+payload)
    metadata=struct.pack('<8sII',b'PMPROG1',address(event['file_offset']),len(called))
    metadata+=b''.join(struct.pack('<II',a,tag) for a,tag in sorted(called.items()))
    Path('romfs/professor-program.bin').write_bytes(metadata)
    manifest=dict(source='rel/gor.rel',sha256=hashlib.sha256(raw).hexdigest(),event=event,
                  sections=sections,callbacks=called,foreign_relocations=foreign,
                  policy='Authored native section bases; original module0 imports and local ADDR32 relocations; unsupported callbacks are not silently admitted',
                  license='Local original Nintendo/Intelligent Systems data; no redistribution rights')
    Path('local-assets/professor/event-program.json').write_text(json.dumps(manifest,indent=2)+'\n')
    print('Original first-talk event commands:',len(event['commands']))

if __name__=='__main__':
    import argparse
    p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);build(p.parse_args().iso)
