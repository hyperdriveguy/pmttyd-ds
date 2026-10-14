#!/usr/bin/env python3
"""Export exact US sys heading glyphs/backgrounds and its original center curtain."""
import argparse,hashlib,json,shutil,struct,subprocess
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO,extract,convert_tpl
from character_animation_data import animation
from character_surface_mesh import compile_surface_mesh

def stardust_data(iso):
    # Original signed ADDI addresses, not the unadjusted LIS upper half.
    with Path(iso).open('rb') as source:
        source.seek(0x420);dol=struct.unpack('>I',source.read(4))[0];source.seek(dol);header=source.read(256)
        def read(address,size):
            for i in range(18):
                off,va,length=(struct.unpack_from('>I',header,b+i*4)[0] for b in (0,0x48,0x90))
                if va<=address and address+size<=va+length:
                    source.seek(dol+off+address-va);return source.read(size)
            raise ValueError('Stardust data outside original DOL')
        jump=struct.unpack('>8I',read(0x803a9388,32))
        if jump!=(0x801ed56c,0x801ed574,0x801ed57c,0x801ed584,0x801ed58c,0x801ed594,0x801ed5a4,0x801ed5ac):raise ValueError('Original Stardust display frame jump table changed')
        data=b'PMHSTAR1'
        for index in (3,4,5,6,7,0,1,2):
            for row in struct.iter_unpack('>5h4B',read(0x803a91c8+index*56,56)):
                x,y,z,u,v,*rgba=row;data+=struct.pack('<5f4B',x,y,z,u/32,v/32,*rgba)
        data+=read(0x80419c90,36)+read(0x80419cbc,7)
        for address in (0x8042629c,0x804262a0,0x80426288,0x804262a4,0x80426294,0x804262ac,0x804262b0):data+=struct.pack('<f',struct.unpack('>f',read(address,4))[0])
        spans=[(0x801eda2c,0x69c),(0x801ed630,0x3fc),(0x801ed208,0x428),(0x8026c324,0x20),(0x803a91c8,448),(0x80419c90,36),(0x80419cbc,7)]
        evidence=[dict(address=a,size=n,sha256=hashlib.sha256(read(a,n)).hexdigest())for a,n in spans]
        expected=['40dbd6490093344b761ac1fafe4ea231e749413fd8eda8626565f281c6fd84a0','ddd4c347deb2e7e3e57195fa872004b6ba644798d4eac47cb95a3774155ab858','18275958e602299ce5d978f1c9df064457177e5494275ba591211b07f1cb2e7f','6b19fd52fb70db058f43afa9f5b9f1922cdbd69d70b461055e6ba104576326b5','34c468776e6f9480b27916e6b6d3d22877ab2d0986e77d083eca6d1e7f28efb3','5ad6edf2f2b9ef6b5f6261092bb09e45511f3bc74727dc33db6bbd7288415133','c4ebccdcbbebe03cb39abcb40f701c888f850f8345bc2aa9b491ccbe922f8f5a']
        if [row['sha256']for row in evidence]!=expected:raise ValueError('Original Stardust source bodies/data changed')
        return data,evidence

def build(iso=DEFAULT_ISO,output='romfs'):
    out=Path(output);out.mkdir(parents=True,exist_ok=True);work=Path('local-assets/heading');work.mkdir(parents=True,exist_ok=True)
    fingerprint=hashlib.sha256(Path(__file__).read_bytes()+b''.join(Path('tools',n).read_bytes() for n in ('iso_assets.py','character_surface_mesh.py','character_animation_pack.py','character_animation_data.py','character_bake.py'))+str((Path(iso).stat().st_size,Path(iso).stat().st_mtime_ns)).encode()).hexdigest()
    meta=out/'heading-resources.json';names=[f'heading-{i}' for i in [0,1,2,3,19,20,21,22,23,24,25]]
    required=[n+ext for n in names for ext in ('.t3x','.size')]+['heading-curtain.pmanim','heading-curtain.t3x','heading-curtain.bin','heading-stardust.bin','heading-stardust.t3x','heading-stardust.size']
    if meta.exists() and json.loads(meta.read_text()).get('fingerprint')==fingerprint and all((out/n).is_file() for n in required):return
    tpl=work/'effect_sys.tpl';extract(iso,'e/us/effect_sys.tpl',tpl)
    model=work/'OFF_d_maku_center';curtain=work/'OFF_d_maku_center.tpl'
    extract(iso,'a/OFF_d_maku_center',model);extract(iso,'a/OFF_d_maku_center-',curtain)
    tool=shutil.which('tex3ds')or'/opt/devkitpro/tools/bin/tex3ds'
    for index in [0,1,2,3,19,20,21,22,23,24,25]:
        name=f'heading-{index}';png=work/(name+'.png');convert_tpl(tpl,png,index)
        image=Image.open(png).convert('RGBA');w,h=image.size
        if (w,h)!=((160,160) if index<4 else (64,64)):raise ValueError('Original heading raster dimensions changed')
        padded=Image.new('RGBA',(1<<(w-1).bit_length(),1<<(h-1).bit_length()));padded.paste(image,(0,0));padded.save(png)
        subprocess.run([tool,'-f','rgba8','-z','auto','-o',str(out/(name+'.t3x')),str(png)],check=True,stdout=subprocess.DEVNULL)
        (out/(name+'.size')).write_bytes(struct.pack('<4I',w,h,padded.width,padded.height))
    for pose,span,count in [('Z_1',0,1),('A_1',60,121)]:
        original=animation(model.read_bytes(),pose)
        if original['loop'] or original['start']!=0 or original['end']!=span or len(original['frames'])!=count:raise ValueError('Original heading curtain pose changed')
    mesh=compile_surface_mesh([(model,curtain,['Z_1','A_1'])],out/'heading-curtain.pmanim',work/'heading-curtain.png')
    image=Image.open(work/'heading-curtain.png');width,height=image.size
    image.resize((width//2,height//2),Image.Resampling.LANCZOS).save(work/'heading-curtain.png')
    subprocess.run([tool,'-f','rgba8','-z','auto','-o',str(out/'heading-curtain.t3x'),str(work/'heading-curtain.png')],check=True,stdout=subprocess.DEVNULL)
    (out/'heading-curtain.bin').write_bytes(struct.pack('<8sfII',b'PMHEDC01',width/height,2,mesh['textures']))
    data,evidence=stardust_data(iso);(out/'heading-stardust.bin').write_bytes(data)
    n64=work/'effect_n64.tpl';extract(iso,'e/us/effect_n64.tpl',n64)
    png=work/'stardust.png';convert_tpl(n64,png,32);image=Image.open(png).convert('RGBA')
    if image.size!=(176,22):raise ValueError('Original Stardust32 raster dimensions changed')
    padded=Image.new('RGBA',(256,32));padded.paste(image,(0,0));padded.save(png)
    subprocess.run([tool,'-f','rgba8','-z','auto','-o',str(out/'heading-stardust.t3x'),str(png)],check=True,stdout=subprocess.DEVNULL)
    (out/'heading-stardust.size').write_bytes(struct.pack('<4I',176,22,256,32))
    sources=[json.loads(p.with_suffix(p.suffix+'.provenance.json').read_text())for p in (tpl,model,curtain,n64)]
    meta.write_text(json.dumps(dict(fingerprint=fingerprint,sources=sources,stardust=evidence,backgrounds=[0,1,2,3],glyph_order=[19,20,21,22,21,23,24,25],subtitle_id='msg_stg_seq_prolouge',curtain=mesh,conversion='Exact original heading rasters with transparent power-of-two padding; original Z_1/A_1 curtain hierarchy/materials/poses, half-size Lanczos atlas; no replacement art or text',license='Original Nintendo/Intelligent Systems assets for private local use; no redistribution rights'),indent=2)+'\n')
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs');a=p.parse_args();build(a.iso,a.output)
