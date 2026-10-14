#!/usr/bin/env python3
"""Private original harbor geometry bindings and captured ship paper model."""
import argparse,hashlib,json,shutil,struct,subprocess
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO,extract,convert_tpl
from map_convert import convert
from map_visibility import EVENT_GROUPS
from character_animation_pack import compile_animation_pack
from character_animation_data import animation
from character_bake import geometry,u
OBJECTS=('S_fune_a','S_a_hikinami','S_f_drin','S_f_wa_l','S_f_wa_r','S_f_en01','S_skr')
def build(iso,out):
 out=Path(out);out.mkdir(parents=True,exist_ok=True);work=Path('local-assets/opening-ship');work.mkdir(parents=True,exist_ok=True)
 for name,path in [('gor_00.d','m/gor_00/d'),('gor_00.t','m/gor_00/t'),('OFF_d_kaiten','a/OFF_d_kaiten'),('OFF_d_kaiten.tpl','a/OFF_d_kaiten-')]:extract(iso,path,work/name)
 info=convert(work/'gor_00.d',work/'map',ambient=('nami','nami_02','hiki_nami'),mesh_ambient=('nami',),event_groups={**EVENT_GROUPS['gor_00'],'S_a_hikinami':13,'S_c_hikinami':14},object_bindings=OBJECTS)
 for ext in ['vertices','batches','uvanim','meshanim']:shutil.copyfile(work/'map'/('map.'+ext),out/('opening-gor_00.'+ext))
 shutil.copyfile(work/'map/map.houseobj',out/'opening-gor_00.shipobj')
 tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
 for i in range(len(info['textures'])):
  png=work/f'texture-{i}.png';convert_tpl(work/'gor_00.t',png,i);im=Image.open(png).convert('RGBA');w,h=im.size
  im.resize((min(128,1<<(w-1).bit_length()),min(128,1<<(h-1).bit_length())),Image.Resampling.LANCZOS).save(png)
  subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(out/f'opening-gor_00-{i}.t3x'),str(png)],check=True,stdout=subprocess.DEVNULL)
 # The original voyage room owns its own sky geometry and miniature steamship.
 for ext in ['d','t']:extract(iso,'m/muj_20/'+ext,work/('muj_20.'+ext))
 hidden=tuple('S_hai_'+side+'_'+str(n).zfill(2) for side in ['l','r'] for n in range(1,6))+('S_a1','S_b')
 voyageObjects=('S_fune_m','S_umi_l_siro_b')+OBJECTS[2:]
 voyageInfo=convert(work/'muj_20.d',work/'voyage',hidden=hidden,ambient=('nami','kumo'),mesh_ambient=('nami','fune_01'),event_groups={'S_fune_m':1,'S_umi_l_siro_b':13},object_bindings=voyageObjects,initial_pose=('yoru_hiru',250),local_offsets={'S_hai03_sun':(0,-26,0)})
 for ext in ['vertices','batches','uvanim','meshanim']:shutil.copyfile(work/'voyage'/('map.'+ext),out/('opening-muj_20.'+ext))
 shutil.copyfile(work/'voyage/map.houseobj',out/'opening-muj_20.shipobj')
 for i in range(len(voyageInfo['textures'])):
  png=work/f'voyage-texture-{i}.png';convert_tpl(work/'muj_20.t',png,i);im=Image.open(png).convert('RGBA');w,h=im.size
  im.resize((min(128,1<<(w-1).bit_length()),min(128,1<<(h-1).bit_length())),Image.Resampling.LANCZOS).save(png)
  subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(out/f'opening-muj_20-{i}.t3x'),str(png)],check=True,stdout=subprocess.DEVNULL)
 model=work/'OFF_d_kaiten';tpl=work/'OFF_d_kaiten.tpl';raw=model.read_bytes()
 kinds=[]
 a=animation(raw,'A_1')
 if (a['start'],a['end'],a['loop'],len(a['frames']))!=(0,60,False,61):raise ValueError('Original ship turn animation changed')
 for binding,state in enumerate(a['texture_indices']):
  index=u(raw,u(raw,0x194)+binding*8)+state;kinds.append(u(raw,u(raw,0x198)+index*64+8))
 if kinds!=[2,1]:raise ValueError('Original two captured ship faces changed')
 compile_animation_pack([(model,tpl,['Z_1','A_1'])],out/'opening-ship-paper.pmanim',lambda model,tpl,index:index)
 surfaces=[]
 for binding,tri in geometry(model,'A_1',bindings=True,surfaces=True):
  if any(v[4]!=(255,255,255,255) or v[5:]!=(2,1) for v in tri):raise ValueError('Original ship paper surface changed')
  surfaces.append((kinds[binding],2,1))
 with (out/'opening-ship-paper.bin').open('wb') as f:
  f.write(struct.pack('<8sI',b'PMSHP001',len(surfaces)))
  for s in surfaces:f.write(struct.pack('<3I',*s))
 (out/'opening-ship-assets.json').write_text(json.dumps(dict(objects=OBJECTS,paper_sha256=hashlib.sha256(raw).hexdigest(),paper_pose='A_1',frames=61,source_capture=[0,0,608,480],virtual_point=[0,0,375],sources=[json.loads(Path(str(work/n)+'.provenance.json').read_text()) for n in ['gor_00.d','gor_00.t','muj_20.d','muj_20.t','OFF_d_kaiten','OFF_d_kaiten.tpl']],voyage_initial_pose=['yoru_hiru',250],voyage_sun_local_offset=[0,-26,0],conversion='Original mesh/local coordinates; map textures bounded128; original61 paper deformation keys and distinct capture-face ownership; original used yoru_hiru completed pose and local sun translation',license='Original Nintendo/Intelligent Systems assets for local use, no redistribution rights'),indent=2)+'\n')
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs');a=p.parse_args();build(a.iso,a.output)
