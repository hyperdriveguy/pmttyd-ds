#!/usr/bin/env python3
"""Pack the original US harbor before-battle event with native section addresses.
Only source ADDR32 relocations change. The ISO and source scripts stay read-only.
"""
import argparse,hashlib,json,struct,shutil,subprocess
from PIL import Image
from character_mesh import compile_mesh
from pathlib import Path
from iso_assets import DEFAULT_ISO,extract
from rel_event_inventory import read_bindings,inventory_events
SERVICES=['evt_mario_key_onoff','evt_mario_cont_onoff','evt_mario_set_pos','evt_mario_set_dir',
 'evt_npc_set_position','evt_npc_set_ry','evt_npc_flag_onoff','evt_mario_get_pos','evt_snd_envoff',
 'evt_mario_mov_pos2','evt_eff_fukidashi','evt_mario_set_pose','evt_npc_move_position','evt_msg_print',
 'evt_cam3d_evt_set','evt_mario_dispflag_onoff','evt_snd_sfxon_3d','evt_mario_jump_pos',
 'L_evt_pouch_get_hp','L_evt_pouch_set_hp','evt_npc_set_battle_info','evt_npc_battle_start',
 'evt_npc_wait_battle_end','evt_npc_get_reglid']
def build(iso=DEFAULT_ISO):
 work=Path('local-assets/harbor-event');work.mkdir(parents=True,exist_ok=True);out=Path('romfs');out.mkdir(exist_ok=True)
 source=work/'gor.rel';extract(iso,'rel/gor.rel',source);raw=source.read_bytes()
 if hashlib.sha256(raw).hexdigest()!='d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a':raise ValueError('Unexpected US gor REL')
 event=inventory_events(raw,'artifacts/reference/us_symbols.csv','gor',['gundan1000tai_beforebattle'])['events'][0]
 if event['sha256']!='4cc7e8fba986105d05f73b8c21bea7c79e4b69e1fae556f88f4920f77fd97bef':raise ValueError('Before-battle source changed')
 local,imports,foreign=read_bindings(raw);u=lambda o:struct.unpack_from('>I',raw,o)[0]
 sections=[]
 for i in range(u(12)):
  off,size=struct.unpack_from('>II',raw,u(16)+i*8);sections.append((off&~3,size,0x90000000+i*0x100000))
 def address(off):
  matches=[base+off-start for start,size,base in sections if start and start<=off<start+size]
  if len(matches)!=1:raise ValueError('Ambiguous source offset')
  return matches[0]
 segments=[]
 for off,size,base in sections:
  if not off or not size:continue
  data=bytearray(raw[off:off+size]);data.extend(b'\0'*(-len(data)%4))
  for site,b in local.items():
   if off<=site<off+size:struct.pack_into('>I',data,site-off,sections[b['section']][2]+b['addend'])
  for site,target in imports.items():
   if off<=site<off+size:struct.pack_into('>I',data,site-off,target)
  segments.append((base,bytes(data)))
 called={}
 for c in event['commands']:
  if c['opcode']!=91:continue
  op=c['operands'][0];name=op.get('annotation')
  if name not in SERVICES:raise ValueError('Unadmitted harbor callback: '+str(name))
  target=op.get('dol_address')
  if target is None:target=address(op['rel_offset'])
  called[target]=SERVICES.index(name)
 table=bytearray();payload=bytearray();offset=12+12*len(segments)
 for base,data in segments:table+=struct.pack('>III',base,len(data),offset);payload+=data;offset+=len(data)
 (out/'harbor-events.pmevt').write_bytes(b'PMEVT001'+struct.pack('>I',len(segments))+table+payload)
 actors=[address(o) for o in [0x14b28,0x14b40,0x14b5c,0x14b6c]]
 meta=struct.pack('<8s6I',b'PMHBR001',address(event['file_offset']),*actors,len(called))
 meta+=b''.join(struct.pack('<II',target,tag) for target,tag in sorted(called.items()))
 (out/'harbor-program.bin').write_bytes(meta)
 (work/'manifest.json').write_text(json.dumps(dict(event=event,callbacks=called,sections=sections,actors=actors,foreign=foreign,source='rel/gor.rel',license='Original Nintendo/Intelligent Systems local data; no redistribution rights'),indent=2)+'\n')
 # Retain the original pose IDs requested by this event, including effect-bank poses.
 tpl=work/'a_mario.tpl';extract(iso,'a/a_mario-',tpl);models=[]
 for bank,poses in [('a_mario',['M_S_1','M_W_1','M_J_1B','M_J_1C']),('e_mario',['M_I_N','M_N_H'])]:
  model=work/bank;extract(iso,'a/'+bank,model)
  if model.read_bytes()[0x44:0x84].split(b'\0')[0]!=b'a_mario':raise ValueError('Unexpected Mario texture owner')
  models.append((model,tpl,poses))
 fingerprint=hashlib.sha256(Path(__file__).read_bytes()+b''.join(m.read_bytes() for m,_,_ in models)+tpl.read_bytes()+Path('tools/character_mesh.py').read_bytes()+Path('tools/character_animation_pack.py').read_bytes()).hexdigest()
 manifest=out/'harbor-mario.json';required=[out/('harbor-mario.'+ext) for ext in ['pmanim','t3x','bin']]
 if not(manifest.exists() and json.loads(manifest.read_text()).get('fingerprint')==fingerprint and all(p.exists() for p in required)):
  meta=compile_mesh(models,out/'harbor-mario.pmmesh',work/'atlas.png',required[0]);height=128
  while height<((meta['textures']+7)//8)*128:height*=2
  Image.open(work/'atlas.png').crop((0,0,1024,height)).resize((512,height//2),Image.Resampling.LANCZOS).save(work/'runtime.png')
  required[2].write_bytes(struct.pack('<8sf',b'PMHUV001',1024/height))
  tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
  subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(required[1]),str(work/'runtime.png')],check=True,stdout=subprocess.DEVNULL)
  meta.update(fingerprint=fingerprint,uv_scale=1024/height,source='a/a_mario,a/e_mario,a/a_mario-',license='Original local assets; no redistribution rights')
  manifest.write_text(json.dumps(meta,indent=2)+'\n')
 print('Original harbor before-battle commands:',len(event['commands']))
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);build(p.parse_args().iso)
