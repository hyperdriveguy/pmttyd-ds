#!/usr/bin/env python3
"""Pack original US title/demo raster assets and music read-only, outside history."""
import argparse,hashlib,json,shutil,struct,subprocess
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO,extract,convert_tpl
from audio_convert import stm
from map_convert import convert
from character_mesh import compile_mesh
from character_surface_mesh import compile_surface_mesh

def cached_maps_complete(out):
    """A vertices file alone cannot admit an original map pack cache hit."""
    for variant in ['aaa_00','aaa_00-interior','muj_20','gor_10','gor_00']:
        prefix='opening-'+variant
        if not all((out/(prefix+'.'+ext)).is_file() for ext in ['vertices','batches','uvanim','meshanim']):return False
        descriptor=(out/(prefix+'.batches')).read_bytes()
        if len(descriptor)<8:return False
        batches,textures=struct.unpack_from('<II',descriptor)
        if len(descriptor)!=8+batches*24 or textures>1024:return False
        if not all((out/f'{prefix}-{i}.t3x').is_file() for i in range(textures)):return False
    return True

def main():
    p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs');a=p.parse_args()
    out=Path(a.output);work=Path('local-assets/opening');work.mkdir(parents=True,exist_ok=True);out.mkdir(parents=True,exist_ok=True)
    helpers=['opening_ship_assets.py','opening_background_assets.py','iso_assets.py','audio_convert.py','map_convert.py','character_mesh.py','character_surface_mesh.py','character_animation_pack.py','character_animation_data.py']
    iso=Path(a.iso);fingerprint=hashlib.sha256(Path(__file__).read_bytes()+b''.join(Path('tools',name).read_bytes() for name in helpers)+str((iso.stat().st_size,iso.stat().st_mtime_ns)).encode()).hexdigest()
    manifest=out/'opening-assets.json'
    images=[('title-background','mariost.tpl',5),('title-logo','mariost.tpl',21),('title-prompt','mariost.tpl',16),('title-copyright','mariost.tpl',9),('opening-letter','mariost.tpl',4),('opening-map','mariost.tpl',3)]
    images += [(f'demo-{i}','demo.tpl',i) for i in [0,1,2,8,9,10,11,12]]
    tracks=[('title','sys_tit1_32k'),('demo-book','sys_demo1_32k'),('demo-market','sys_demo2_32k'),('opening-house','evt_mri1_32k'),('opening-voyage','sys_opn1_32k'),('opening-sailing','evt_kou1_32k')]
    required=[f'opening-{r}.vertices' for r in ['aaa_00','aaa_00-interior','muj_20','gor_10']]+[n+ext for n,_,_ in images for ext in ['.t3x','.size']]+[n+'.wav' for n,_ in tracks]+['prologue.t3x','prologue.size']
    required += [f'opening-bg-{room}'+ext for room in ['aaa_00','gor_10','gor_00','dmo_00'] for ext in ['.t3x','.size']]
    required += ['opening-muj_20.shipobj','opening-gor_00.shipobj','opening-ship-paper.pmanim','opening-ship-paper.bin']+[f'opening-gor_00.{ext}' for ext in ['vertices','batches','uvanim','meshanim']]
    required += ['opening-aaa_00.houseobj','opening-gor_10.camera','demo-page.pmanim','demo-page.bin','demo-page.json','demo-page-back.t3x','demo-page-back.size']
    required += [n+ext for n in ['c_luigi','c_paretta','c_peach','c_nazo','demo-chest'] for ext in ['.pmanim','.t3x','.uvscale']]
    if manifest.exists() and json.loads(manifest.read_text()).get('fingerprint')==fingerprint and all((out/n).exists() for n in required) and cached_maps_complete(out):return
    from opening_background_assets import build as build_backgrounds
    records=build_backgrounds(a.iso,out)
    from opening_ship_assets import build as build_ship

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
        hidden=('S_naka',) if variant=='aaa_00' else ('S_ie_door',) if variant=='aaa_00-interior' else ('marukongou','densha','pukupuku') if variant=='gor_10' else ()
        info=convert(work/(room+'.d'),work/variant,hidden=hidden,scene_root='S_naka' if variant=='aaa_00-interior' else None,object_bindings=('S_ie_door','S_posuto','sai_ki2','sai_ki1') if variant=='aaa_00' else ())
        if variant=='aaa_00':shutil.copyfile(work/variant/'map.houseobj',out/(prefix+'.houseobj'))
        for ext in ['vertices','batches','uvanim','meshanim']:shutil.copyfile(work/variant/('map.'+ext),out/(prefix+'.'+ext))
        for index,_ in enumerate(info['textures']):
            png=work/variant/f'texture-{index}.png';convert_tpl(work/(room+'.t'),png,index)
            im=Image.open(png).convert('RGBA');w,h=im.size;im.resize((min(128,1<<(w-1).bit_length()),min(128,1<<(h-1).bit_length())),Image.Resampling.LANCZOS).save(png)
            subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(out/f'{prefix}-{index}.t3x'),str(png)],check=True,stdout=subprocess.DEVNULL)
    extract(a.iso,'m/gor_10/c',work/'gor_10.c');shutil.copyfile(work/'gor_10.c',out/'opening-gor_10.camera')
    records.append(json.loads((work/'gor_10.c.provenance.json').read_text()))
    build_ship(a.iso,out)
    scene_poses={'c_luigi':['L_5','L_6','L_7','L_8','L_9','L_11'],
                 'c_paretta':['S_1'],
                 'c_peach':['P_Z_1','P_S_1','P_T_1','P_W_1','P_S_7','P_T_13','P_W_7','P_S_4','P_T_4','P_W_4','P_S_8','P_T_14','P_W_8','P_B_1','P_B_2','P_T_7','P_H_1','P_B_3','P_B_4'],
                 'c_nazo':['Z_1','S_1','T_1','A_1'],
                 'demo-chest':['S_1','A_1']}
    for bank,poses in scene_poses.items():
        original='MOBJ_GoldenTreasureBox' if bank=='demo-chest' else bank
        extract(a.iso,'a/'+original,work/bank);extract(a.iso,'a/'+original+'-',work/(bank+'.tpl'))
        records.extend(json.loads(p.read_text()) for p in [work/(bank+'.provenance.json'),work/(bank+'.tpl.provenance.json')])
        png=work/(bank+'.png');models=[(work/bank,work/(bank+'.tpl'),poses)]
        mesh=compile_surface_mesh(models,out/(bank+'.pmanim'),png) if bank=='demo-chest' else compile_mesh(models,out/(bank+'.pmmesh'),png,out/(bank+'.pmanim'))
        height=128
        while height<((mesh['textures']+7)//8)*128:height*=2
        atlas=Image.open(png).crop((0,0,1024,height))
        # The storybook and market coexist during the authored final zoom.
        # Peach's nineteen poses require a full-height atlas: RGBA8 at1024²
        # costs4MiB, exceeding the remaining linear arena during that overlap.
        # Keep every original cell/pose and normalized UV; halve this bank's
        # raster dimensions rather than dropping the book or an actor.
        if bank=='c_peach':atlas=atlas.resize((512,height//2),Image.Resampling.LANCZOS)
        atlas.save(png)
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
    from opening_book_assets import build as build_book
    build_book(a.iso,out)
    manifest.write_text(json.dumps(dict(fingerprint=fingerprint,sources=records,images=images,music=tracks,conversion='Large background raster half-size Lanczos; Peach animation atlas half-size Lanczos (512x512 RGBA8, all original cells/poses and normalized UV retained); small title masks unchanged; transparent power-of-two padding; original-rate PCM16 music and loop intervals',license='Original Nintendo/Intelligent Systems assets for local use; no redistribution rights'),indent=2)+'\n')
if __name__=='__main__':main()
