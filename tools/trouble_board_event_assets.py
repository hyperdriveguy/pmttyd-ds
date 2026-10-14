#!/usr/bin/env python3
"""Original Trouble Center board/counter event data and finite dependencies."""
import argparse,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,extract
from rel_data_pack import package
from rel_event_inventory import inventory_events
SERVICES=['evt_mario_key_onoff','evt_msg_toge','evt_msg_print','evt_msg_select','evt_msg_print_add','evt_msg_continue',
 'irai_init_func','irai_main_func','irai_end_func','evt_mario_set_dir','evt_party_set_dir','evt_run_case_evt',
 'irai_cancel_func','evt_pouch_get_coin','evt_pouch_add_coin','evt_npc_set_position','evt_door_set_param',
 'evt_item_delete','evt_item_delete_check','irai_check_func','irai_start_func']
ROOTS=['gor_irai_init','gor_irai_main','gor_irai_lecture','gor_irai_tenshu_main',
       'irai_msg_client','irai_msg_contents','irai_msg_kakunin','irai_item_table']

def build(iso=DEFAULT_ISO):
    work=Path('local-assets/trouble-board');work.mkdir(parents=True,exist_ok=True)
    source=work/'gor.rel';extract(iso,'rel/gor.rel',source);raw=source.read_bytes()
    expected='d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a'
    if hashlib.sha256(raw).hexdigest()!=expected:raise ValueError('Unexpected original US REL')
    packed,meta=package(raw,'artifacts/reference/us_symbols.csv','gor',ROOTS)
    out=Path('romfs');out.mkdir(exist_ok=True);(out/'trouble-board-events.pmevt').write_bytes(packed)
    objects={o['name']:o for o in meta['objects']}
    def address(offset):
        matches=[s['address']+offset-s['file_offset'] for s in meta['sections'] if s['file_offset'] and s['file_offset']<=offset<s['file_offset']+s['size']]
        if len(matches)!=1:raise ValueError('Ambiguous source callback address')
        return matches[0]
    events=inventory_events(raw,'artifacts/reference/us_symbols.csv','gor',[o['name'] for o in meta['objects'] if o['type']=='evt'])['events']
    callbacks={}
    for event in events:
        for command in event['commands']:
            if command['opcode']!=91:continue
            operand=command['operands'][0];name=operand.get('annotation')
            if name not in SERVICES:raise ValueError('Unadmitted Trouble callback')
            target=operand.get('dol_address')
            if target is None:target=address(operand['rel_offset'])
            callbacks[target]=SERVICES.index(name)
    if set(callbacks.values())!=set(range(len(SERVICES))):raise ValueError('Incomplete original callback catalog')
    header=[objects[n]['address'] for n in ['gor_irai_main','gor_irai_tenshu_main','gor_irai_init','irai_msg_kakunin']]
    (out/'trouble-board-program.bin').write_bytes(struct.pack('<8s5I',b'PMIRAI01',*header,len(callbacks))+
        b''.join(struct.pack('<II',a,tag) for a,tag in sorted(callbacks.items())))
    meta['callbacks']=[dict(address=a,tag=tag,name=SERVICES[tag]) for a,tag in sorted(callbacks.items())]
    meta.update(source='rel/gor.rel',source_sha256=expected,roots=ROOTS,pack_sha256=hashlib.sha256(packed).hexdigest(),
        policy='Selected data objects and transitive ADDR32 data dependencies only; native section addresses; no executable sections or invented commands',
        license='Nintendo/Intelligent Systems original resources; local use only, no redistribution rights')
    (out/'trouble-board-events.json').write_text(json.dumps(meta,indent=2)+'\n')
    print('Original Trouble Center event closure:',len(meta['objects']),'objects,',len(packed),'bytes')
    return meta
if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);build(p.parse_args().iso)
