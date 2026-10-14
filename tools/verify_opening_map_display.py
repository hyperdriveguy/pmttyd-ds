#!/usr/bin/env python3
"""Pin the original US map event, used draw/item and ordinary music bodies read-only."""
import argparse,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
from rel_event_inventory import inventory_events

def verify(iso=DEFAULT_ISO,symbols='artifacts/reference/us_symbols.csv'):
    evidence=json.loads(Path(__file__).with_name('opening_map_source_evidence.json').read_text())
    files={f['path']:f for f in inventory(iso)['files']}
    with Path(iso).open('rb') as source:
        if source.read(8)!=b'G8ME01\0\0':raise ValueError('US revision0 disc0 required')
        item=files['rel/aaa.rel'];source.seek(item['offset']);raw=source.read(item['size'])
        for span in evidence['rel']:
            a,n=span['offset'],span['size']
            if hashlib.sha256(raw[a:a+n]).hexdigest()!=span['sha256']:raise ValueError('Original map callback changed')
        source.seek(0x420);dol=struct.unpack('>I',source.read(4))[0];source.seek(dol);h=source.read(256)
        sections=[tuple(struct.unpack_from('>I',h,o+4*i)[0]for o in [0,0x48,0x90])for i in range(18)]
        def read(a,n):
            o,b,l=next(s for s in sections if s[1]<=a and a+n<=s[1]+s[2]);source.seek(dol+o+a-b);return source.read(n)
        for span in evidence['dol']:
            if hashlib.sha256(read(span['address'],span['size'])).hexdigest()!=span['sha256']:raise ValueError('Original '+span['name']+' changed')
        for address,expected in [(0x80315ac8+203*16,'802ce02080000000802ce03850400001'),(0x80315ac8+8*16,'802cc80c80000000802cc81c56400001'),(0x80315ac8+170*16,'802cda1c80000000802cda304e400001'),(0x8041f890,'3f0000003f19999a')]:
            if read(address,len(expected)//2).hex()!=expected:raise ValueError('Original music/icon constants changed')
        item=files['icon.bin'];source.seek(item['offset']);icons=source.read(item['size'])
        descriptor=struct.unpack_from('>H',icons,2*(314+1))[0]
        if descriptor!=0x1346 or struct.unpack_from('>6H',icons,descriptor)!=(0,0,0,1,319,0):raise ValueError('Original map icon descriptor changed')
    event=inventory_events(raw,symbols,'aaa',['evt_prologue2'])['events'][0]
    if len(event['commands'])!=evidence['event_count'] or event['sha256']!=evidence['event_sha256']:raise ValueError('Original map event changed')
    commands={c['address']:c for c in event['commands']}
    for address,value in [(0x19b8,300),(0x1a44,1000),(0x1a74,1500),(0x1ac0,1000)]:
        c=commands[address]
        if c['opcode']!=10 or c['operands'][0]['raw']!=value:raise ValueError('Original map wait changed')
    if commands[0x1a7c]['operands'][0]['raw']!=4204967296 or commands[0x1aa0]['operands'][0]['raw']!=4204967296:raise ValueError('Original GF0 handoff changed')
    if [o['raw'] for o in commands[0x1ac8]['operands'][1:]]!=[512,2000]:raise ValueError('Original map BGM off changed')
    return len(evidence['rel'])+len(evidence['dol'])+12
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--symbols',default='artifacts/reference/us_symbols.csv');a=p.parse_args()
    print(f'{verify(a.iso,a.symbols)} original map event/draw/item/music anchors verified read-only')
