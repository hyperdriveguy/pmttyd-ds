#!/usr/bin/env python3
"""Pack original setup2 Crump/helper init and phase requests. ISO stays read-only."""
import argparse,hashlib,json,struct,shutil,subprocess
from PIL import Image
from character_mesh import compile_mesh
from pathlib import Path
from iso_assets import DEFAULT_ISO,extract
from rel_event_inventory import read_bindings,inventory_events
EVENTS=['init_event@unit_boss_kanbu1.o','phase_event@unit_boss_kanbu1.o','wait_event@unit_boss_kanbu1.o','init_event@unit_npc_christine.o','wait_event@unit_npc_christine.o']
SERVICES=['btlevtcmd_SetEventWait','btlevtcmd_SetEventAttack','btlevtcmd_SetEventDamage','btlevtcmd_SetEventConfusion','btlevtcmd_SetEventPhase','btlevtcmd_onoff_battleflag','btlevtcmd_SpawnUnit','btlevtcmd_SetUnitWork','btlevtcmd_SetPos','btlevtcmd_SetHomePos','btlevtcmd_SetWalkSound','btlevtcmd_SetRunSound','btlevtcmd_StartWaitEvent','btlevtcmd_get_turn','btlevtcmd_CheckPhase','btlevtcmd_StopWaitEvent','btlevtcmd_AnimeChangePose','evt_btl_camera_set_mode','evt_btl_camera_set_moveto','evt_sub_intpl_init','evt_sub_intpl_get_value','btlevtcmd_SetRotate','btlevtcmd_WaitEventEnd','btlevtcmd_StatusWindowOnOff','btlevtcmd_GetUnitWork','btlevtcmd_SetTalkPose','btlevtcmd_SetStayPose','evt_msg_print','btlevtcmd_snd_se','btlevtcmd_AnimeWaitPlayComplete','btlevtcmd_AnimeChangePoseType','btlevtcmd_GetMaxHp','btlevtcmd_GetHp','_mario_ac_success_check','_mario_ac_failed_check','btlevtcmd_AnimeChangePoseFromTable','btlevtcmd_SetMaxMoveCount']
def build(iso):
 work=Path('local-assets/harbor-tutorial');work.mkdir(parents=True,exist_ok=True);out=Path('romfs');out.mkdir(exist_ok=True)
 p=work/'gor.rel';extract(iso,'rel/gor.rel',p);raw=p.read_bytes()
 if hashlib.sha256(raw).hexdigest()!='d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a':raise ValueError('Unexpected gor REL')
 events=inventory_events(raw,'artifacts/reference/us_symbols.csv','gor',EVENTS)['events'];local,imports,foreign=read_bindings(raw)
 u=lambda o:struct.unpack_from('>I',raw,o)[0]
 # Prove the numeric setup2 and separate spawned NPC identity, never lecture kind.
 for site,target in [(0x60ecc,0x60dd0),(0x60dd4,0x60c90),(0x60dd8,0x608f8),(0x60c94,0x60c60),(0x60c60,0x645c8),(0x647d8,0x66d78),(0x64684,0x64808),(0x66e34,0x66ed8)]:
  if local[site]['file_offset']!=target:raise ValueError('Unexpected setup2 pointer at '+hex(site))
 if u(0x60c90)!=1 or u(0x60dd0)!=50 or u(0x66d78)!=0xdd or struct.unpack_from('>h',raw,0x645d0)[0]!=5:raise ValueError('Unexpected setup2 kind')
 sections=[(u(u(16)+i*8)&~3,u(u(16)+i*8+4),0x95000000+i*0x100000) for i in range(u(12))]
 def address(o):return next(base+o-off for off,size,base in sections if off and off<=o<off+size)
 segments=[]
 for section_index,(off,size,base) in enumerate(sections):
  if not off or not size or u(u(16)+section_index*8)&1:continue
  data=bytearray(raw[off:off+size]);data.extend(b'\0'*(-len(data)%4))
  for site,b in local.items():
   if off<=site<off+size:struct.pack_into('>I',data,site-off,sections[b['section']][2]+b['addend'])
  for site,a in imports.items():
   if off<=site<off+size:struct.pack_into('>I',data,site-off,a)
  segments.append((base,data))
 callbacks={}
 for event in events:
  for c in event['commands']:
   if c['opcode']!=91:continue
   op=c['operands'][0];name=op.get('annotation')
   if name not in SERVICES:raise ValueError('Unadmitted tutorial service '+str(name))
   a=op.get('dol_address');a=address(op['rel_offset']) if a is None else a;callbacks[a]=SERVICES.index(name)
 # Original Mario pose table used by type43 after the introductory voice pose.
 with Path(iso).open('rb') as disc:
  disc.seek(0x420);dol=struct.unpack('>I',disc.read(4))[0];disc.seek(dol);header=disc.read(256)
  def read_dol(a,n):
   for i in range(18):
    off,va,size=(struct.unpack_from('>I',header,b+i*4)[0] for b in (0,0x48,0x90))
    if va<=a and a+n<=va+size:
     disc.seek(dol+off+a-va);data=disc.read(n)
     if len(data)!=n:raise ValueError('Truncated Mario data')
     return data
   raise ValueError('Unmapped Mario data')
  du=lambda a:struct.unpack('>I',read_dol(a,4))[0]
  if imports.get(0x60c8c)!=0x80417920 or read_dol(0x80417920,8)!=b'\0'*8:raise ValueError('Unexpected setup2 held-item table')
  mario_kind=du(0x80416a50);mario_parts=du(mario_kind+0xb8);mario_table=du(mario_parts+0x48)
  # Literal UnitEntry admits all3 original Mario parts before init. Parts2/3
  # retain the source shell name; display Entry does not SetAnim. This name
  # is not a b_mario clip and must not be fabricated by the converter.
  if read_dol(mario_kind+0xb4,1)!=b'\x03':raise ValueError('Unexpected Mario part count')
  for index in [1,2]:
   if du(mario_parts+index*0x4c+0x48)!=0x80417bc8:raise ValueError('Unexpected Mario shell pose table')
  if read_dol(0x80417bc8,24)!=struct.pack('>6I',28,0x802f3f90,39,0x802f3f90,69,0x802f3f90) or read_dol(0x802f3f90,8)!=b'KRN_H_1\0':raise ValueError('Unexpected Mario shell pose')
  if du(mario_kind)!=222:raise ValueError('Unexpected Mario kind')
  strings={};table_data=bytearray()
  for i in range(80):
   item=read_dol(mario_table+i*8,8);table_data+=item;tag,a=struct.unpack('>II',item)
   if not a:break
   data=read_dol(a,128).split(b'\0',1)[0]+b'\0';data+=b'\0'*(-len(data)%4);strings[a]=data
  else:raise ValueError('Unterminated Mario pose table')
 from harbor_tutorial_dol_assets import segments as dol_segments
 segments.extend(dol_segments(iso))
 segments.sort(key=lambda s:s[0])
 offset=12+12*len(segments);table=bytearray();payload=bytearray()
 for base,data in segments:table+=struct.pack('>III',base,len(data),offset);payload+=data;offset+=len(data)
 (out/'harbor-tutorial-events.pmevt').write_bytes(b'PMEVT001'+struct.pack('>I',len(segments))+table+payload)
 # Explicit source object addresses and source initial HP/type. Runtime IDs are allocated separately.
 words=[address(e['file_offset']) for e in events]+[address(o) for o in [0x647d8,0x645c8,0x66d78,0x60c60,0x60c90,0x608f8]]+[mario_table]
 meta=struct.pack('<8s13I',b'PMHTUT01',*words,len(callbacks))+b''.join(struct.pack('<II',a,t) for a,t in sorted(callbacks.items()))
 (out/'harbor-tutorial-program.bin').write_bytes(meta)
 (work/'manifest.json').write_text(json.dumps(dict(events=events,callbacks=callbacks,source='rel/gor.rel',foreign=foreign,license='Nintendo/Intelligent Systems resources; no redistribution rights'),indent=2)+'\n')
 # Scoped original actor poses required by init and the introductory phase.
 for actor,bank,texture,poses in [('mario','b_mario','a_mario',['M_S_1','M_N_2','M_D_1','M_C_3']),('goombella','c_pkr','c_pkr',['PKR_S_1','PKR_T_1']),('crump','c_kanbu','c_kanbu',['S_1','R_2','R_1','A_1A','A_1B','A_1C','D_1'])]:
  model=work/bank;tpl=work/(texture+'.tpl');extract(iso,'a/'+bank,model);extract(iso,'a/'+texture+'-',tpl)
  fingerprint=hashlib.sha256(Path(__file__).read_bytes()+model.read_bytes()+tpl.read_bytes()+b''.join(Path('tools',h).read_bytes() for h in ['character_mesh.py','character_animation_pack.py','character_animation_data.py','character_node.py'])).hexdigest()
  prefix=out/('harbor-tutorial-'+actor);manifest=prefix.with_suffix('.json');products=[prefix.with_suffix('.'+e) for e in ['pmmesh','pmanim','t3x','bin']]
  if manifest.exists() and all(p.exists() for p in products) and json.loads(manifest.read_text()).get('fingerprint')==fingerprint:continue
  meta=compile_mesh([(model,tpl,poses)],products[0],work/(actor+'-atlas.png'),products[1]);height=128
  while height<((meta['textures']+7)//8)*128:height*=2
  Image.open(work/(actor+'-atlas.png')).crop((0,0,1024,height)).resize((512,height//2),Image.Resampling.LANCZOS).save(work/(actor+'-runtime.png'))
  products[3].write_bytes(struct.pack('<8sf',b'PMHTUV01',1024/height))
  subprocess.run([shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds','-f','rgba8','-z','auto','-o',str(products[2]),str(work/(actor+'-runtime.png'))],check=True,stdout=subprocess.DEVNULL)
  meta.update(fingerprint=fingerprint,uvScale=1024/height,source=['a/'+bank,'a/'+texture+'-'],license='Original Nintendo/Intelligent Systems assets; no redistribution rights');manifest.write_text(json.dumps(meta,indent=2)+'\n')
 from harbor_tutorial_combat_assets import build as build_combat
 build_combat(iso)
 from harbor_tutorial_stage_assets import build as build_stage
 build_stage(iso)
 print('Original tutorial init/phase/wait commands:' ,[len(e['commands']) for e in events])
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);build(p.parse_args().iso)
