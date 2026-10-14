#!/usr/bin/env python3
"""Original Trouble Center board/counter event data and finite dependencies."""
import argparse,hashlib,json
from pathlib import Path
from iso_assets import DEFAULT_ISO,extract
from rel_data_pack import package
ROOTS=['gor_irai_init','gor_irai_main','gor_irai_lecture','gor_irai_tenshu_main',
       'irai_msg_client','irai_msg_contents','irai_msg_kakunin','irai_item_table']

def build(iso=DEFAULT_ISO):
    work=Path('local-assets/trouble-board');work.mkdir(parents=True,exist_ok=True)
    source=work/'gor.rel';extract(iso,'rel/gor.rel',source);raw=source.read_bytes()
    expected='d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a'
    if hashlib.sha256(raw).hexdigest()!=expected:raise ValueError('Unexpected original US REL')
    packed,meta=package(raw,'artifacts/reference/us_symbols.csv','gor',ROOTS)
    out=Path('romfs');out.mkdir(exist_ok=True);(out/'trouble-board-events.pmevt').write_bytes(packed)
    meta.update(source='rel/gor.rel',source_sha256=expected,roots=ROOTS,pack_sha256=hashlib.sha256(packed).hexdigest(),
        policy='Selected data objects and transitive ADDR32 data dependencies only; native section addresses; no executable sections or invented commands',
        license='Nintendo/Intelligent Systems original resources; local use only, no redistribution rights')
    (out/'trouble-board-events.json').write_text(json.dumps(meta,indent=2)+'\n')
    print('Original Trouble Center event closure:',len(meta['objects']),'objects,',len(packed),'bytes')
    return meta
if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);build(p.parse_args().iso)
