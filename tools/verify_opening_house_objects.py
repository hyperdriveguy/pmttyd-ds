#!/usr/bin/env python3
"""Read-only US revision0 door/postbox static request and matrix evidence."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
from rel_event_inventory import inventory_events

def verify(iso,symbols):
    info=inventory(iso)
    if info['disc_id']!='G8ME01':raise ValueError('US source required')
    entry=next(f for f in info['files'] if f['path']=='rel/aaa.rel')
    with Path(iso).open('rb') as source:
        source.seek(6)
        if source.read(2)!=bytes(2):raise ValueError('US disc0 revision0 required')
        source.seek(entry['offset']);raw=source.read(entry['size'])
        events=inventory_events(raw,symbols,'aaa',['evt_door','evt_prologue'])['events']
        door=next(e for e in events if e['name']=='evt_door')
        if door['sha256']!='fb757f201ef61a0482a7bc99a053b5c75daf0dcfb3b6ccb9daea3cee67b25b38':raise ValueError('Source door event changed')
        by_address={c['address']:c for e in events for c in e['commands']}
        for address,raw_value in [(0xb74,4064966728),(0xb80,4064966272)]:
            c=by_address[address]
            if c['opcode']!=60 or c['operands'][1]['raw']!=raw_value:raise ValueError('Source door multiplier changed')
        args=by_address[0xb8c]['operands']
        if args[0].get('annotation')!='evt_mapobj_rotate' or args[2].get('identifier')!='S_ie_door' or [o['raw'] for o in args[3:]]!=[0,4264967296,0]:raise ValueError('Source door axis/object changed')
        for address,x in [(0x1084,2),(0x10a8,0),(0x132c,2),(0x1350,0)]:
            args=by_address[address]['operands']
            if args[0].get('annotation')!='evt_mapobj_trans' or args[2].get('identifier')!='S_posuto' or [o['raw'] for o in args[3:]]!=[x,0,0]:raise ValueError('Source postbox request changed')
        source.seek(0x420);dol=struct.unpack('>I',source.read(4))[0]
        expected=[(0x1b390,0x180,'57e372ec60ffbbf1c19db9155a33e19257e14109babf0fbd6be1d57032afbbcb'),(0x1b254,0x13c,'8568e88b40ba87f4d1230cdc767c803b07230e6d9b8d4b62a2dfe1c1371b7a07'),(0x25130,0x420,'212353de648b9515c7c9618b7faa4210044a37855eb11ab28fa2d389388d75f1')]
        for offset,size,digest in expected:
            source.seek(dol+offset)
            if hashlib.sha256(source.read(size)).hexdigest()!=digest:raise ValueError('Source matrix service changed')
        source.seek(dol);header=source.read(0x100)
        for address,value in [(0x8041f944,0x41200000),(0x8041f948,0x3c8efa35)]:
            found=False
            for i in range(18):
                offset=struct.unpack_from('>I',header,4*i)[0];base=struct.unpack_from('>I',header,0x48+4*i)[0];size=struct.unpack_from('>I',header,0x90+4*i)[0]
                if base<=address<base+size:
                    source.seek(dol+offset+address-base)
                    if struct.unpack('>I',source.read(4))[0]!=value:raise ValueError('Source local-unit/radian constant changed')
                    found=True;break
            if not found:raise ValueError('Missing source constant')
    return 10

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--symbols',default='artifacts/reference/us_symbols.csv');a=p.parse_args()
    print(f'Original house object event/matrix hashes and {verify(a.iso,a.symbols)} scalar/object/axis/constant anchors verified read-only')
if __name__=='__main__':main()
