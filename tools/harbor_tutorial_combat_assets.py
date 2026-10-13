#!/usr/bin/env python3
"""Inventory installed source Crump combat hooks, retaining original addresses.
The existing tutorial event pack already contains these unchanged gor data
streams. This mapping does not admit unimplemented default-DOL child services.
"""
import argparse,json,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,extract
from rel_event_inventory import inventory_events,read_bindings
EVENTS=['attack_event@unit_boss_kanbu1.o','normal_attack_event@unit_boss_kanbu1.o','damage_event@unit_boss_kanbu1.o','dead_event@unit_boss_kanbu1.o','attack_event@unit_npc_christine.o']
HASHES=['47e8564501181e8e228135b7c2faef4fa8c40507d3ded8432c11692994ea8416','1ac3f7e075a9f37d216d9d2504590e0fd2bf071533d55f059b88cb36d0adceba','f59a6bfb7c00767c28083d3812e89e8a081a3c9e96ec8e55de2e2537ee1a04ba','57dd705b76f1a416227bf466da2384e67a14f8bf088bd9f078f360027ee27136','4663084af36b2537184d3e8b9be45025369a6f56b49e7187ffbcff14b7ab145f']
def build(iso=DEFAULT_ISO):
 work=Path('local-assets/harbor-tutorial-combat');work.mkdir(parents=True,exist_ok=True);source=work/'gor.rel';extract(iso,'rel/gor.rel',source);raw=source.read_bytes()
 if hashlib.sha256(raw).hexdigest()!='d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a':raise ValueError('Unexpected gor REL')
 events=inventory_events(raw,'artifacts/reference/us_symbols.csv','gor',EVENTS)['events']
 if [e['sha256'] for e in events]!=HASHES or [len(e['commands']) for e in events]!=[11,95,5,75,3]:raise ValueError('Unexpected source combat scripts')
 local,imports,foreign=read_bindings(raw);u=lambda o:struct.unpack_from('>I',raw,o)[0]
 sections=[(u(u(16)+i*8)&~3,u(u(16)+i*8+4),0x95000000+i*0x100000) for i in range(u(12))]
 def address(o):return next(base+o-off for off,size,base in sections if off and off<=o<off+size)
 # Original Crump data49 is the death stream; key48 remains default Dummy.
 if u(0x646bc)!=49 or local[0x646c0]['file_offset']!=events[3]['file_offset']:raise ValueError('Unexpected Crump death hook')
 callbacks={}
 for e in events:
  for c in e['commands']:
   if c['opcode']!=91:continue
   op=c['operands'][0];a=op.get('dol_address');a=address(op['rel_offset']) if a is None else a
   callbacks[a]=op['annotation']
 # Small native mapping plus original callback names; no script/text duplication.
 meta=struct.pack('<8s7I',b'PMHTC001',*[address(e['file_offset']) for e in events],address(0x64718),len(callbacks))
 for a,name in sorted(callbacks.items()):meta+=struct.pack('<I64s',a,name.encode('ascii'))
 Path('romfs/harbor-tutorial-combat.bin').write_bytes(meta)
 (work/'manifest.json').write_text(json.dumps(dict(source='rel/gor.rel',events=events,callbacks=callbacks,foreign=foreign,weapon=address(0x64718),license='Original Nintendo/Intelligent Systems local data; no redistribution rights'),indent=2)+'\n')
 print('Original Crump combat hooks:',[len(e['commands']) for e in events],'; callbacks:',len(callbacks))
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);build(p.parse_args().iso)
