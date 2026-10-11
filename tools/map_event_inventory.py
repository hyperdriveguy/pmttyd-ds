#!/usr/bin/env python3
"""Inventory original REL map event calls using an external US symbol table.
Read-only inspection, not event execution or a story-state evaluator.
Symbol reference: github.com/jdaster64/ttyd-utils/resources/us_symbols.csv.
"""
import csv,json,struct
from pathlib import Path
from room_convert import pointers

def inventory(rel,symbols):
    raw,local=pointers(rel)
    def u(o):return struct.unpack_from('>I',raw,o)[0]
    sections=[u(u(16)+n*8)&~3 for n in range(u(12))];imports={}
    for j in range(u(44)//8):
        module,stream=struct.unpack_from('>II',raw,u(40)+j*8);section=0;offset=0
        if module:continue
        while True:
            delta,kind,target,addend=struct.unpack_from('>HBBI',raw,stream);stream+=8
            if kind==203:break
            if kind==202:section=target;offset=0;continue
            offset+=delta
            if kind==1:imports[sections[section]+offset]=addend
    rows=list(csv.DictReader(Path(symbols).open()))
    names={int(r['ram_addr'],16):r['name'] for r in rows if r['area']=='_main' and r['ram_addr']}
    events=[]
    for row in rows:
        if row['area']!='gor' or row['type']!='evt' or not row['name'].startswith('gor_') or not row['name'].endswith('_init_evt'):continue
        start=int(row['file_addr'],16);end=start+int(row['size'],16)
        if end>len(raw):raise ValueError('Symbol event range exceeds REL')
        offset=start;commands=[]
        while offset<end:
            word=u(offset);count=word>>16;opcode=word&65535
            if count>100 or offset+4+count*4>end:raise ValueError(f'Invalid event command at {offset:#x}')
            args=[]
            for n in range(count):
                site=offset+4+n*4;arg={'raw':u(site)}
                if site in imports:arg['dol_address']=imports[site];arg['symbol']=names.get(imports[site])
                if site in local:
                    pointer=local[site];arg['rel_pointer']=pointer
                    try:
                        string=raw[pointer:raw.index(b'\0',pointer)].decode('ascii')
                        if string and all(32<=ord(c)<127 for c in string):arg['string']=string
                    except (ValueError,UnicodeError):pass
                args.append(arg)
            commands.append({'offset':offset,'opcode':opcode,'arguments':args})
            offset+=4+count*4
        events.append({'name':row['name'],'start':start,'size':end-start,'commands':commands})
    return events
if __name__=='__main__':
    import argparse
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--rel',default='local-assets/gor.rel');p.add_argument('--symbols',required=True);p.add_argument('--output',required=True);a=p.parse_args()
    events=inventory(a.rel,a.symbols);Path(a.output).write_text(json.dumps(events,indent=2)+'\n')
    for event in events:
        for command in event['commands']:
            args=command['arguments'];function=args[0].get('symbol','') if args else ''
            if command['opcode']==0x5b and function in ('evt_map_playanim','evt_mapobj_flag_onoff'):
                print(event['name'],hex(command['offset']),function,[x.get('string',hex(x['raw'])) for x in args[1:]])
