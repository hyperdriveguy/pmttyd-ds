#!/usr/bin/env python3
"""Original area-owned door data; no executable PPC or invented event words."""
import argparse,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,extract
from rel_data_pack import package

def build(iso=DEFAULT_ISO):
    work=Path('local-assets/door-data');work.mkdir(parents=True,exist_ok=True)
    source=work/'gor.rel';extract(iso,'rel/gor.rel',source);raw=source.read_bytes()
    digest=hashlib.sha256(raw).hexdigest()
    if digest!='d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a':raise ValueError('Unexpected original US REL')
    packed,meta=package(raw,'artifacts/reference/us_symbols.csv','gor',['door_data_05','door_data_05_npc_01'])
    objects={o['name']:o for o in meta['objects']}
    # Exact gor_02_init_evt4d8e8..4d948: copy into GW11, then select the
    # original list using GSW1420==1. This is a finite native init descriptor.
    row=(2,objects['door_data_05']['address'],11,objects['door_data_05_npc']['address'],objects['door_data_05_npc_01']['address'],1)
    if objects['door_data_05']['size']!=72:raise ValueError('Unexpected source door record')
    out=Path('romfs');out.mkdir(exist_ok=True)
    (out/'door-data.pmevt').write_bytes(packed)
    (out/'door-data.bin').write_bytes(struct.pack('<8sI6I',b'PMDRDT01',1,*row))
    meta.update(source='rel/gor.rel',source_sha256=digest,records=[dict(room=row[0],source=row[1],work=row[2],normal=row[3],alternate=row[4],quest=row[5])],
        policy='Original data closure; finite copy/list-selection descriptor from gor_02_init_evt4d8e8..4d948; later case/geometry/material providers remain separate',
        license='Original Nintendo/Intelligent Systems resources; local use only, no redistribution rights')
    (out/'door-data.json').write_text(json.dumps(meta,indent=2)+'\n')
    print('Original mutable door data:',len(packed),'bytes,',len(meta['objects']),'objects')
    return meta
if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);build(p.parse_args().iso)
