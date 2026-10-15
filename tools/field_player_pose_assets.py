#!/usr/bin/env python3
"""Full original field-player MODEL/TPL/directory and prologue CPU clips."""
import argparse,hashlib,json,struct,shutil,subprocess
from PIL import Image
from pathlib import Path
from iso_assets import DEFAULT_ISO,extract
from battle_pose_directory import source_directory,write_directory
from character_surface_mesh import compile_surface_mesh
MODELS={
 'a_mario':['M_S_1','M_W_1','M_R_1','M_J_1B','M_J_1C','M_S_3','M_I_S','M_I_Y','M_D_2','M_U_3','M_V_2','M_U_2','M_I_U'],
 'a_mario_r':['M_S_1R','M_W_1R','M_R_1R','M_J_1BR','M_J_1CR'],
 'e_mario':['M_I_5','M_N_5A','M_F_1','M_L_3','M_I_N','M_N_H','M_I_G','M_D_8','M_C_3','M_N_7','M_N_5B','M_N_C'],
 # Goombella is shared by the party and story NPC owners. Preserve every
 # original descriptor so the first canonical bank cannot hide story clips.
 'c_pkr':None}
def build(iso=DEFAULT_ISO,output='romfs',work='local-assets/field-player-poses'):
 out=Path(output);cache=Path(work);out.mkdir(parents=True,exist_ok=True);cache.mkdir(parents=True,exist_ok=True);models={}
 for name,requested in MODELS.items():
  model=cache/name;texture=cache/(name+'.tpl');extract(iso,'a/'+name,model);identity,bank,source=source_directory(model.read_bytes());assert identity==name
  extract(iso,'a/'+bank+'-',texture);clips=list(source) if requested is None else list(dict.fromkeys([source[0]]+requested));assert all(p in source for p in clips)
  asset='field-player-'+name;prefix=out/asset;atlas=cache/(asset+'.png')
  result=compile_surface_mesh([(model,texture,clips)],str(prefix)+'.pmanim',atlas)
  write_directory(model,str(prefix)+'.pose-directory.bin',texture)
  image=Image.open(atlas).convert('RGBA');height=image.height
  runtime=cache/(asset+'-runtime.png');image.resize((512,height//2),Image.Resampling.LANCZOS).save(runtime)
  Path(str(prefix)+'.bin').write_bytes(struct.pack('<8sf',b'PMSUV1',1024/height))
  subprocess.run([shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds','-f','rgba8','-z','auto','-o',str(prefix)+'.t3x',str(runtime)],check=True,stdout=subprocess.DEVNULL)
  products={}
  for suffix in ['pmanim','pose-directory.bin','draw-model.bin','draw-textures.bin','bin','t3x']:
   p=Path(str(prefix)+'.'+suffix);products[p.name]=dict(bytes=p.stat().st_size,sha256=hashlib.sha256(p.read_bytes()).hexdigest())
  row=dict(asset=asset,original_index0=source[0],source_descriptor_order=source,converted_pose_names=clips,conversion=result,products=products,source=[json.loads(model.with_suffix('.provenance.json').read_text()),json.loads(texture.with_suffix('.tpl.provenance.json').read_text())],license='Nintendo/Intelligent Systems original resources; local use only, no redistribution rights')
  Path(str(prefix)+'.json').write_text(json.dumps(row,indent=2)+'\n');models[name]=row
 from field_npc_resource_assets import export_model
 npc_work=cache/'shared-npc';npc_work.mkdir(parents=True,exist_ok=True)
 npc=export_model(iso,'c_kuri_h',npc_work,out);image=Image.open(npc_work/'field-native-c_kuri_h.png').convert('RGBA');height=image.height
 runtime=npc_work/'shared-frankly-runtime.png';image.resize((512,height//2),Image.Resampling.LANCZOS).save(runtime)
 prefix=out/'shared-field-frankly';Path(str(prefix)+'.bin').write_bytes(struct.pack('<8sf',b'PMSUV1',1024/height))
 subprocess.run([shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds','-f','rgba8','-z','auto','-o',str(prefix)+'.t3x',str(runtime)],check=True,stdout=subprocess.DEVNULL)
 meta=dict(resources=models,policy='Full original MODEL/TPL/descriptor metadata; Full Goombella CPU directory; selected Mario prologue CPU clips plus actual descriptor0. Unconverted Mario clips remain unavailable, never remapped to a different pose.')
 (out/'field-player-poses.json').write_text(json.dumps(meta,indent=2)+'\n');return meta
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs');p.add_argument('--work',default='local-assets/field-player-poses');a=p.parse_args();build(a.iso,a.output,a.work)
