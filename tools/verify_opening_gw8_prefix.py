#!/usr/bin/env python3
"""Pin the scoped original fresh-opening GW8 event provenance read-only.

This is a literal local-REL event census, not callback/store/runtime closure.
"""
import argparse,csv,hashlib,json,struct
from collections import defaultdict
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
from rel_event_inventory import inventory_events

TOKEN=4244967304

def verify(iso=DEFAULT_ISO,symbols='artifacts/reference/us_symbols.csv'):
    manifest=json.loads(Path(__file__).with_name('opening_gw8_prefix_evidence.json').read_text())
    info=inventory(iso)
    if info['disc_id']!='G8ME01':raise ValueError('US disc required')
    files={f['path']:f for f in info['files']}
    groups=defaultdict(list)
    for e in manifest['events']:groups[e['module']].append(e)
    events=[]
    with Path(iso).open('rb') as source:
        source.seek(6)
        if source.read(2)!=bytes(2):raise ValueError('US revision0/disc0 required')
        for module,expected in groups.items():
            resource=files['rel/'+module+'.rel'];source.seek(resource['offset']);raw=source.read(resource['size'])
            got=inventory_events(raw,symbols,module,[e['name']+'@'+e['namespace'] for e in expected])['events']
            for e,pin in zip(got,expected):
                if any(e[k]!=pin[k] for k in ['name','namespace','file_offset','size','sha256']):raise ValueError('Original prefix event changed')
                if any(o['raw']==TOKEN for c in e['commands'] for o in c['operands']):raise ValueError('GW8 operand in selected prefix')
                events.append(dict(module=module,**e))
            if module=='gor':
                arrival=inventory_events(raw,symbols,module,[e['name']+'@'+e['namespace'] for e in manifest['arrival_events']])['events']
        source.seek(0x420);dol=struct.unpack('>I',source.read(4))[0];source.seek(dol);header=source.read(256)
        def read(address,size):
            for i in range(18):
                offset,va,length=[struct.unpack_from('>I',header,b+i*4)[0] for b in [0,0x48,0x90]]
                if va<=address and address+size<=va+length:
                    source.seek(dol+offset+address-va);return source.read(size)
            raise ValueError('DOL pin outside image')
        for address,size,digest in [(0x8003781c,0x118,'1d00ceae20e44915faf432c019ee680f648650085a6627042bc5e282230eaf32'),(0x800377b0,0x6c,'5a0e1a5af9008548501e81de999f0f6d12a44f4d9642941a46127a4205e29feb'),(0x80051868,0x364,'eb67fa0707263fcd321df7e977c9f034517a71bfcd835ca96454feacd088a003')]:
            if hashlib.sha256(read(address,size)).hexdigest()!=digest:raise ValueError('Original initializer/reset/jump body changed')
        if read(0x8041fdfc,4)!=bytes.fromhex('c0800000'):raise ValueError('Original jump height constant changed')
    if len(events)!=manifest['event_count'] or sum(len(e['commands']) for e in events)!=manifest['command_count']:raise ValueError('Prefix census extent changed')
    rows=list(csv.DictReader(Path(symbols).open()))
    evtrows={(r['area'],int(r['file_addr'],16)):r for r in rows if r['type']=='evt' and r['file_addr']}
    by_offset={(e['module'],e['file_offset']):e for e in events}
    reachable=set();todo=[]
    for module,selections in manifest['roots'].items():
        for selection in selections:
            name,namespace=selection.split('@');e=next(e for e in events if e['module']==module and e['name']==name and e['namespace']==namespace);todo.append((module,e['file_offset']))
    while todo:
        key=todo.pop()
        if key in reachable:continue
        if key not in by_offset:raise ValueError('Local event dependency missing from prefix census')
        reachable.add(key)
        for c in by_offset[key]['commands']:
            for o in c['operands']:
                target=(key[0],o.get('rel_offset'))
                if target in evtrows:todo.append(target)
    if reachable!=set(by_offset):raise ValueError('Prefix closure/root membership changed')
    for e,pin in zip(arrival,manifest['arrival_events']):
        if any(e[k]!=pin[k] for k in ['name','namespace','file_offset','size','sha256']):raise ValueError('Original arrival event changed')
    enter=next(e for e in arrival if e['name']=='enter_gorotsuki_town');init=next(e for e in arrival if e['name']=='gor_00_init_evt')
    if any(o['raw']==TOKEN for c in init['commands'] for o in c['operands']):raise ValueError('GW8 operand in gor initializer')
    if any(o['raw']==TOKEN for c in enter['commands'] if c['address']<0x25920 for o in c['operands']):raise ValueError('Earlier GW8 operand in arrival root')
    cmds={c['address']:c for c in enter['commands']}
    first=cmds[0x25920]['operands'];get=cmds[0x2595c]['operands'];jump=cmds[0x259d8]['operands']
    if first[0].get('annotation')!='evt_npc_set_position' or [o['raw'] for o in first[2:]]!=[0,TOKEN,0]:raise ValueError('Original incoming dummy position changed')
    if get[0].get('annotation')!='evt_npc_get_position' or [o['raw'] for o in get[2:]]!=[4264967296,4264967297,4264967298]:raise ValueError('Original dummy getter changed')
    if cmds[0x25974]['opcode']!=52 or [o['raw'] for o in cmds[0x25974]['operands']]!=[TOKEN,4264967297]:raise ValueError('Original GW8 SETF changed')
    if jump[0].get('annotation')!='evt_npc_jump_position_nohit' or [o['raw'] for o in jump[2:]]!=[0,TOKEN,0,1800,100]:raise ValueError('Original resolved nohit jump changed')
    return {'events':len(events),'commands':manifest['command_count'],'literal_gw8_before_arrival':0,'callback_store_closure':False}
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--symbols',default='artifacts/reference/us_symbols.csv');a=p.parse_args()
    print(json.dumps(verify(a.iso,a.symbols),sort_keys=True))
