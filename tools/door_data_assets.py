#!/usr/bin/env python3
"""Original area-owned door data; no executable PPC or invented event words."""
import argparse,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,extract
from rel_data_pack import package
from dol_event_closure import linked_events,merge_segments

def build(iso=DEFAULT_ISO):
    work=Path('local-assets/door-data');work.mkdir(parents=True,exist_ok=True)
    source=work/'gor.rel';extract(iso,'rel/gor.rel',source);raw=source.read_bytes()
    digest=hashlib.sha256(raw).hexdigest()
    if digest!='d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a':raise ValueError('Unexpected original US REL')
    packed,meta=package(raw,'artifacts/reference/us_symbols.csv','gor',['door_data_05','door_data_05_npc_01',*[f'gor_02.o::{n}' for n in ['evt_door_02_entry','evt_door_03_entry','evt_door_03_roll_entry','door_data_02','door_data_03','door_data_03_roll','door_data_04','gor_02_init_evt']]])
    objects={o['name']:o for o in meta['objects']}
    # Linked immutable generic entry and actual adjacent dispatch-table prefetch.
    with Path(iso).open('rb') as disc:
        if disc.read(8)!=b'G8ME01\0\0':raise ValueError('Original US disc0 revision0 required')
        disc.seek(0x420);dol=struct.unpack('>I',disc.read(4))[0];disc.seek(dol);header=disc.read(256)
        spans=[]
        for n in range(18):
            off,va,size=(struct.unpack_from('>I',header,b+4*n)[0] for b in (0,0x48,0x90))
            if va<=0x8033156c and 0x803316a4<=va+size:spans.append(off+0x8033156c-va)
        if len(spans)!=1:raise ValueError('Invalid generic door entry DOL span')
        disc.seek(dol+spans[0]);entry=disc.read(0x138)
    if hashlib.sha256(entry[:0x134]).hexdigest()!='6269747e7ff14d9effbd84761663f25e880e3ed3c191c842e974db0559e78281' or entry[0x134:]!=struct.pack('>I',0x800e8288):raise ValueError('Unexpected original door entry/context')
    segments=[]
    for n in range(struct.unpack_from('>I',packed,8)[0]):
        address,size,offset=struct.unpack_from('>III',packed,12+12*n);segments.append((address,packed[offset:offset+size]))
    segments.append((0x8033156c,entry))
    linked,linked_meta=linked_events(iso,raw,meta['objects'],'artifacts/reference/us_symbols.csv')
    segments.extend(linked)
    from field_battle_setup_assets import gor_setup_zero_segments
    battle_segments,battle_meta=gor_setup_zero_segments(iso,raw,'artifacts/reference/us_symbols.csv')
    segments.extend(battle_segments)
    from field_battle_registry_assets import gor_registry_segments
    registry_segments,registration,registry_meta=gor_registry_segments(iso,raw,'artifacts/reference/us_symbols.csv')
    segments.extend(registry_segments)
    from field_npc_constructor_assets import constructor_segments
    npc_segments,npc_meta=constructor_segments(iso)
    segments.extend(npc_segments)
    packed=merge_segments(segments)
    programs=[(objects[a]['address'],objects[b]['address']) for a,b in [('evt_door_02_entry','door_data_02'),('evt_door_03_entry','door_data_03'),('evt_door_03_roll_entry','door_data_03_roll')]]+[(0x8033156c,objects['door_data_04']['address'])]

    # Exact gor_02_init_evt4d8e8..4d948: copy into GW11, then select the
    # original list using GSW1420==1. This is a finite native init descriptor.
    row=(2,objects['door_data_05']['address'],11,objects['door_data_05_npc']['address'],objects['door_data_05_npc_01']['address'],1)
    if objects['door_data_05']['size']!=72:raise ValueError('Unexpected source door record')
    out=Path('romfs');out.mkdir(exist_ok=True)
    (out/'door-data.pmevt').write_bytes(packed)
    (out/'field-area-battle.bin').write_bytes(struct.pack('<8sI6I',b'PMFABT01',1,*registration))
    (out/'field-area-init.bin').write_bytes(struct.pack('<8sIII',b'PMFAIN01',1,2,objects['gor_02_init_evt']['address']))
    (out/'field-area-events.bin').write_bytes(struct.pack('<8sI',b'PMFAEV02',len(programs))+b''.join(struct.pack('<III',2,*p) for p in programs))
    (out/'door-data.bin').write_bytes(struct.pack('<8sI6I',b'PMDRDT01',1,*row))
    meta.update(field_npc_constructor=npc_meta,field_battle_registry=registry_meta,field_battle_setup=battle_meta,area_initializers=[dict(room=2,address=objects['gor_02_init_evt']['address'])],linked_dol_events=linked_meta,door_entry=dict(address=0x8033156c,size=0x134,sha256=hashlib.sha256(entry[:0x134]).hexdigest(),prefetch_address=0x803316a0,prefetch_word=0x800e8288),entry_programs=[dict(script=a,record=b) for a,b in programs],source='rel/gor.rel',source_sha256=digest,records=[dict(room=row[0],source=row[1],work=row[2],normal=row[3],alternate=row[4],quest=row[5])],
        policy='Original full gor02 root/REL data closure and referenced linked DOL EVT objects with actual scanner context; no PPC or arbitrary mutable globals. Full world-service/data binding remains separate.',
        license='Original Nintendo/Intelligent Systems resources; local use only, no redistribution rights')
    (out/'door-data.json').write_text(json.dumps(meta,indent=2)+'\n')
    from animation_prefetch_assets import build as build_prefetch
    build_prefetch(iso)
    print('Original mutable door data:',len(packed),'bytes,',len(meta['objects']),'objects')
    return meta
if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);build(p.parse_args().iso)
