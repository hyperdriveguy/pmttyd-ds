#!/usr/bin/env python3
"""Pack original US title/demo raster assets and music read-only, outside history."""
import argparse,hashlib,json,shutil,struct,subprocess
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO,extract,convert_tpl
from audio_convert import stm
from map_convert import convert
from character_mesh import compile_mesh

def main():
    p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs');a=p.parse_args()
    out=Path(a.output);work=Path('local-assets/opening');work.mkdir(parents=True,exist_ok=True);out.mkdir(parents=True,exist_ok=True)
    iso=Path(a.iso);fingerprint=hashlib.sha256(Path(__file__).read_bytes()+Path('tools/iso_assets.py').read_bytes()+Path('tools/audio_convert.py').read_bytes()+Path('tools/map_convert.py').read_bytes()+Path('tools/character_mesh.py').read_bytes()+str((iso.stat().st_size,iso.stat().st_mtime_ns)).encode()).hexdigest()
    manifest=out/'opening-assets.json'
    images=[('title-background','mariost.tpl',5),('title-logo','mariost.tpl',21),('title-prompt','mariost.tpl',16),('title-copyright','mariost.tpl',9),('opening-letter','mariost.tpl',4),('opening-map','mariost.tpl',3)]
    images += [(f'demo-{i}','demo.tpl',i) for i in [0,1,2,8,9,10,11,12]]
    tracks=[('title','sys_tit1_32k'),('demo-book','sys_demo1_32k'),('demo-market','sys_demo2_32k'),('opening-house','evt_mri1_32k'),('opening-voyage','sys_opn1_32k'),('opening-sailing','evt_kou1_32k')]
    required=[f'opening-{r}.vertices' for r in ['aaa_00','aaa_00-interior','muj_20','gor_10']]+[n+ext for n,_,_ in images for ext in ['.t3x','.size']]+[n+'.wav' for n,_ in tracks]+['prologue.t3x','prologue.size']
    required += [n+ext for n in ['c_luigi','c_paretta','c_peach','c_nazo'] for ext in ['.pmanim','.t3x','.uvscale']]
    if manifest.exists() and json.loads(manifest.read_text()).get('fingerprint')==fingerprint and all((out/n).exists() for n in required):return
    records=[]
    for path in ['mariost.tpl','demo.tpl']:
        extract(a.iso,path,work/path);records.append(json.loads((work/(path+'.provenance.json')).read_text()))
    tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
    for name,path,index in images:
        png=work/(name+'.png');convert_tpl(work/path,png,index)
        im=Image.open(png).convert('RGBA');w,h=im.size
        # Preserve aspect in the draw; pad to GX-supported power-of-two storage.
        # Large608x480 backgrounds become304x240; original logo/prompts stay exact.
        if w>512 or h>512:im=im.resize((w//2,h//2),Image.Resampling.LANCZOS);w,h=im.size
        canvas=Image.new('RGBA',(1<<(w-1).bit_length(),1<<(h-1).bit_length()));canvas.paste(im,(0,0));canvas.save(png)
        subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(out/(name+'.t3x')),str(png)],check=True,stdout=subprocess.DEVNULL)
        (out/(name+'.size')).write_bytes(struct.pack('<4I',w,h,canvas.width,canvas.height))
    for name,stream in tracks:
        path=work/(name+'.stm');extract(a.iso,'sound/stream/'+stream+'.stm',path);stm(path,out/(name+'.wav'))
        records.append(json.loads(path.with_suffix('.stm.provenance.json').read_text()))
    for variant in ['aaa_00','aaa_00-interior','muj_20','gor_10']:
        room=variant.removesuffix('-interior')
        for ext in ['d','t']:
            extract(a.iso,f'm/{room}/{ext}',work/(room+'.'+ext));records.append(json.loads((work/(room+'.'+ext+'.provenance.json')).read_text()))
        prefix='opening-'+variant
        hidden=('sai_ki1','sai_ki2','S_naka') if variant=='aaa_00' else ('S_ie_door',) if variant=='aaa_00-interior' else ()
        info=convert(work/(room+'.d'),work/variant,hidden=hidden,scene_root='S_naka' if variant=='aaa_00-interior' else None)
        for ext in ['vertices','batches','uvanim','meshanim']:shutil.copyfile(work/variant/('map.'+ext),out/(prefix+'.'+ext))
        for index,_ in enumerate(info['textures']):
            png=work/variant/f'texture-{index}.png';convert_tpl(work/(room+'.t'),png,index)
            im=Image.open(png).convert('RGBA');w,h=im.size;im.resize((min(128,1<<(w-1).bit_length()),min(128,1<<(h-1).bit_length())),Image.Resampling.LANCZOS).save(png)
            subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(out/f'{prefix}-{index}.t3x'),str(png)],check=True,stdout=subprocess.DEVNULL)
    for bank,pose in [('c_luigi','L_5'),('c_paretta','S_1'),('c_peach','P_S_1'),('c_nazo','S_1')]:
        extract(a.iso,'a/'+bank,work/bank);extract(a.iso,'a/'+bank+'-',work/(bank+'.tpl'))
        records.extend(json.loads(p.read_text()) for p in [work/(bank+'.provenance.json'),work/(bank+'.tpl.provenance.json')])
        png=work/(bank+'.png');mesh=compile_mesh([(work/bank,work/(bank+'.tpl'),['L_5','L_7','L_8','L_9'] if bank=='c_luigi' else [pose])],out/(bank+'.pmmesh'),png,out/(bank+'.pmanim'))
        height=128
        while height<((mesh['textures']+7)//8)*128:height*=2
        Image.open(png).crop((0,0,1024,height)).save(png)
        (out/(bank+'.uvscale')).write_bytes(struct.pack('<f',1024/height))
        subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(out/(bank+'.t3x')),str(png)],check=True,stdout=subprocess.DEVNULL)
        (out/(bank+'.size')).write_bytes(struct.pack('<4I',128,128,128,128))
    extract(a.iso,'e/us/effect_sys.tpl',work/'effect_sys.tpl')
    extract(a.iso,'rel/sys.rel',work/'sys.rel')
    order=list((work/'sys.rel').read_bytes()[0x2648:0x2651])
    if order!=[19,20,21,22,21,23,24,25,255]:raise ValueError('Original US prologue heading changed')
    letters=[]
    for i in order[:-1]:
        png=work/f'heading-{i}.png';convert_tpl(work/'effect_sys.tpl',png,i);letters.append(Image.open(png).convert('RGBA'))
    heading=Image.new('RGBA',(512,128));x=0
    for letter in letters:heading.alpha_composite(letter,(x,0));x+=letter.width
    heading.save(work/'prologue.png')
    subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(out/'prologue.t3x'),str(work/'prologue.png')],check=True,stdout=subprocess.DEVNULL)
    (out/'prologue.size').write_bytes(struct.pack('<4I',x,max(im.height for im in letters),512,128))
    manifest.write_text(json.dumps(dict(fingerprint=fingerprint,sources=records,images=images,music=tracks,conversion='Large background raster half-size Lanczos; small title masks unchanged; transparent power-of-two padding; original-rate PCM16 music and loop intervals',license='Original Nintendo/Intelligent Systems assets for local use; no redistribution rights'),indent=2)+'\n')
if __name__=='__main__':main()
