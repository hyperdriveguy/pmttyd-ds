#!/usr/bin/env python3
"""Verify original fresh-demo type9 entry and mapChange type17 exit ownership."""
import argparse,csv,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
HASHES = {'seq_gameMain': '52cd83b5a3acfdb559a7e0950b35c278de49bd97cd35d9a367dcebadd8e2606a', 'seq_mapChangeMain': '517ca2e7b145ff0a315ba6a3c11312e083faced219eeed728b21c54dbadd66eb', 'fadeEntry': '34375b105173c33a535a1d3057bc51f8f7ef1a0b677970e5576df7ff495878ff', 'fadeMain': '369db5633483e4b397234036dae4080d6e8849d68ea747daec990b41fd891437', 'fadeIsFinish': '62d8e88e6c6ebb205a2b428a54a443bb13161d1ee3a84adeb7ba1816f1593557', 'disp_texture': 'bf25b784e62b75e3bf09442927c1ec24341373a32ff3e3a52dc2edc1aa59e1ab', 'evt_fade_set_mapchange_type': 'd42e66213b0762afdabd0dd14443829ef7421ee92a72390a2753710dbe6077fb', 'evt_fade_end_wait': '6c02ca955d9cd43fde3a47cafe6cfe065b3b4dd035443e7213a6aa656f34b1a0'}
def verify(iso,symbols):
    rows=list(csv.DictReader(Path(symbols).open()))
    with Path(iso).open('rb') as f:
        if f.read(8)!=b'G8ME01\0\0':raise ValueError('US disc0 revision0 required')
        f.seek(0x420);dol=struct.unpack('>I',f.read(4))[0];f.seek(dol);header=f.read(256)
        sections=[tuple(struct.unpack_from('>I',header,o+4*i)[0]for o in [0,0x48,0x90])for i in range(18)]
        def read(address,size):
            offset,base,length=next(s for s in sections if s[1]<=address and address+size<=s[1]+s[2]);f.seek(dol+offset+address-base);return f.read(size)
        for name,digest in HASHES.items():
            r=next(r for r in rows if r['area']=='_main'and r['name']==name)
            if hashlib.sha256(read(int(r['ram_addr'],16),int(r['size'],16))).hexdigest()!=digest:raise ValueError('Original '+name+' changed')
        for address,expected in [(0x8041f4a0,'000000ff'),(0x8041f77c,'f0f0f0ff'),(0x803046b0,'800120b8800121c0')]:
            if read(address,len(expected)//2).hex()!=expected:raise ValueError('Original fade color/dispatch changed')
        entry=next(x for x in inventory(iso)['files'] if x['path']=='rel/dmo.rel');f.seek(entry['offset']);raw=f.read(entry['size'])
        if hashlib.sha256(raw[0x1c44:0x1d64]).hexdigest()!='15b1880815ce4852e0f281e1cb0252f2da4d3f69c6d8303cac74937bf78a0c74':raise ValueError('Original demo_init changed')
        # Event words remain original static evidence, never portable bytecode.
        from rel_event_inventory import inventory_events
        event=inventory_events(raw,symbols,'dmo',['buttonCheck'])['events'][0]
        calls=[c for c in event['commands'] if c['operands'] and c['operands'][0].get('annotation')=='evt_fade_set_mapchange_type']
        if len(calls)!=1 or [o['raw'] for o in calls[0]['operands'][1:]]!=[1,17,500,0xffffffff,0xffffffff]:raise ValueError('Original button exit fade changed')
    return len(HASHES)+5
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--symbols',default='artifacts/reference/us_symbols.csv');a=p.parse_args();print(f'{verify(a.iso,a.symbols)} original fresh-demo entry/exit anchors verified read-only')
