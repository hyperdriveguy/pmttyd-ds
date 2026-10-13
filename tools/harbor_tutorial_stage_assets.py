#!/usr/bin/env python3
"""Convert the original setup2-selected harbor battle stage and music locally.
Source-selected object roots/extra transforms are decoded, not authored. Static
full-scale admission and texture bounds are explicit native adaptations.
"""
import argparse,hashlib,json,struct,shutil,subprocess
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO,extract,convert_tpl
from rel_event_inventory import read_bindings
from map_convert import convert
from audio_convert import stm
from audio_map import gamecube_tables
REL_HASH='d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a'
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def selected_stage(raw):
 if hashlib.sha256(raw).hexdigest()!=REL_HASH:raise ValueError('Unexpected US gor REL')
 local,_,_=read_bindings(raw)
 def target(site):
  if site not in local:raise ValueError('Missing source pointer at '+hex(site))
  return local[site]['file_offset']
 def string(o):return raw[o:raw.index(b'\0',o)].decode('ascii')
 u=lambda o:struct.unpack_from('>I',raw,o)[0]
 # Numeric record2, off-list, sole positive weight, selected stage and objects.
 if target(0x60ecc)!=0x60dd0 or u(0x60dd0)!=50 or target(0x60dd8)!=0x608f8 or u(0x60ddc)!=0:raise ValueError('Unexpected setup2 stage selection')
 stage=target(0x60dd8);base=string(target(stage));scenery=string(target(stage+4));count=u(stage+8);array=target(stage+12)
 if (base,scenery,count,array)!=('bti_01','stg_00_0',4,0x60868):raise ValueError('Unexpected source stage record')
 records=[]
 for i in range(count):
  site=array+24*i;kind,layer,*position,countdown,flags=struct.unpack_from('>hh3fHH',raw,site+4)
  records.append(dict(record=site,name=string(target(site)),kind=kind,layer=layer,position=position,countdown=countdown,flags=flags,nativePosition=[v*.01 for v in position]))
 expected=[('A1',2,0,[33.,0.,-48.]),('A2',2,1,[0.,0.,71.]),('B',2,2,[0.,0.,0.]),('C',2,3,[0.,0.,0.])]
 if [(r['name'],r['kind'],r['layer'],r['position']) for r in records]!=expected:raise ValueError('Unexpected selected object records')
 return dict(record=stage,base=base,scenery=scenery,objects=records,recordSha256=hashlib.sha256(raw[stage:stage+16]).hexdigest(),objectSha256=hashlib.sha256(raw[array:array+count*24]).hexdigest(),music=string(target(0x60ef8)))
def cache_valid(manifest,fingerprint):
 try:
  info=json.loads(manifest.read_text())
  return info['fingerprint']==fingerprint and bool(info['products']) and all(Path(p).exists() and sha(p)==digest for p,digest in info['products'].items())
 except (OSError,ValueError,KeyError):return False
def source_nodes(raw):
 # Original map joint graph: record authored SRT for selected roots and every
 # ancestor. The converter follows this exact hierarchy before extra offsets.
 u=lambda o:struct.unpack_from('>I',raw,o)[0];base=32;table=32+u(4)+u(8)*4;names=table+u(12)*8
 def text(o):return raw[o:raw.index(b'\0',o)].decode('ascii')
 chunks={text(names+u(table+n*8+4)):base+u(table+n*8) for n in range(u(12))}
 root=base+u(chunks['information']+4);result=[]
 def walk(o,parent=None):
  name=text(base+u(o));srt=struct.unpack_from('>9f',raw,o+24);result.append(dict(offset=o-base,name=name,parent=parent,scale=list(srt[:3]),rotation=list(srt[3:6]),translation=list(srt[6:])))
  child=u(o+12)
  if child:walk(base+child,name)
  sibling=u(o+16)
  if sibling:walk(base+sibling,parent)
 walk(root);return result

def build(iso=DEFAULT_ISO):
 work=Path('local-assets/harbor-tutorial-stage');work.mkdir(parents=True,exist_ok=True);out=Path('romfs');out.mkdir(exist_ok=True)
 rel=work/'gor.rel';extract(iso,'rel/gor.rel',rel);selected=selected_stage(rel.read_bytes())
 helpers=b''.join(Path('tools',p).read_bytes() for p in ['map_convert.py','map_animation.py','character_node.py','iso_assets.py'])
 tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
 stages=[]
 for room in [selected['base'],selected['scenery']]:
  d=work/(room+'.d');t=work/(room+'.tpl');extract(iso,'m/'+room+'/d',d);extract(iso,'m/'+room+'/t',t)
  prefix='harbor-tutorial-'+room;manifest=out/(prefix+'.json');fingerprint=hashlib.sha256(Path(__file__).read_bytes()+helpers+d.read_bytes()+t.read_bytes()+json.dumps(selected,sort_keys=True).encode()).hexdigest()
  if cache_valid(manifest,fingerprint):stages.append(dict(prefix=prefix,manifest=str(manifest)));continue
  if room==selected['base']:
   # Source hides escape_wall, and hides frame when no selected layer6.
   hidden=['escape_wall']+(['if_body','if_wire'] if not any(r['layer']==6 for r in selected['objects']) else [])
   info=convert(d,work/room,hidden=hidden)
  else:
   roots=tuple(r['name'] for r in selected['objects']);offsets={r['name']:r['nativePosition'] for r in selected['objects']}
   info=convert(d,work/room,scene_root=roots,scene_offsets=offsets)
   authored=source_nodes(d.read_bytes());names={r['name'] for r in authored}
   if not all(root in names for root in roots):raise ValueError('Source-selected root absent from map')
   # Source names are unique for this admitted map; parent transforms retained.
   if any(sum(n['name']==root for n in authored)!=1 for root in roots):raise ValueError('Ambiguous selected map root')
   info['authoredNodes']=authored
  products=[]
  for ext in ['vertices','batches','uvanim','meshanim']:
   path=out/(prefix+'.'+ext);shutil.copyfile(work/room/('map.'+ext),path);products.append(path)
  for index,_ in enumerate(info['textures']):
   image=work/f'{room}-{index}.png';convert_tpl(t,image,index);bitmap=Image.open(image).convert('RGBA');w,h=bitmap.size
   bitmap.resize((min(128,1<<(w-1).bit_length()),min(128,1<<(h-1).bit_length())),Image.Resampling.LANCZOS).save(image)
   path=out/f'{prefix}-{index}.t3x';subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(path),str(image)],check=True,stdout=subprocess.DEVNULL);products.append(path)
  manifest.write_text(json.dumps(dict(fingerprint=fingerprint,products={str(p):sha(p) for p in products},source=['m/'+room+'/d','m/'+room+'/t'],sourceSha256=[sha(d),sha(t)],selection=selected,map=info,adaptation='Static full-scale scene admission; retained authored SRT; added source stage-object XYZ*0.01 once; map coordinates*0.1; texture power-of-two dimensions capped128px with Lanczos; original vertex colors/material passes/cull retained; native scene/light adapter requires separate binding',license='Nintendo/Intelligent Systems original resources; no redistribution rights'),indent=2)+'\n')
  stages.append(dict(prefix=prefix,manifest=str(manifest)))
 rows,_,_=gamecube_tables(iso);matches=[r for r in rows if r['table']=='bgm' and r['name']==selected['music']]
 if len(matches)!=1:raise ValueError('Ambiguous selected tutorial BGM')
 music=matches[0]
 if (music['table_index'],music['stream_alias'],music['flags'])!=(126,'sound/stream/btl_cbs1_32k',0x80000000):raise ValueError('Unexpected tutorial BGM source binding')
 source=work/'btl_cbs1_32k.stm';extract(iso,music['stream_alias']+'.stm',source)
 wav=out/'harbor-tutorial-music.wav';loop=Path(str(wav)+'.loop');manifest=out/'harbor-tutorial-music.json'
 fingerprint=hashlib.sha256(Path(__file__).read_bytes()+Path('tools/audio_convert.py').read_bytes()+Path('tools/audio_map.py').read_bytes()+source.read_bytes()).hexdigest()
 if not cache_valid(manifest,fingerprint):
  converted=stm(source,wav);products=[wav]+([loop] if converted['loop_enabled'] else [])
  manifest.write_text(json.dumps(dict(fingerprint=fingerprint,products={str(p):sha(p) for p in products},source=music['stream_alias']+'.stm',sourceSha256=sha(source),selection=music,conversion=converted,license='Nintendo/Intelligent Systems original resources; no redistribution rights'),indent=2)+'\n')
 (out/'harbor-tutorial-stage.json').write_text(json.dumps(dict(selection=selected,stages=stages,music=str(wav),musicManifest=str(manifest),source='rel/gor.rel',sourceSha256=REL_HASH,license='Original local assets; no redistribution rights'),indent=2)+'\n')
 print('Original setup2 stage:',selected['base'],selected['scenery'],[(r['name'],r['position']) for r in selected['objects']],'; BGM index',music['table_index'])
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);build(p.parse_args().iso)
