#!/usr/bin/env python3
"""Export original US title center/left-right curtains; local read-only ISO use."""
import argparse,hashlib,json,shutil,struct,subprocess
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO,extract,convert_tpl
from character_animation_data import animation
from character_surface_mesh import compile_surface_mesh

def build(iso=DEFAULT_ISO,out='romfs'):
    out=Path(out);out.mkdir(parents=True,exist_ok=True)
    work=Path('local-assets/title-curtains');work.mkdir(parents=True,exist_ok=True)
    tpl=work/'mariost.tpl';extract(iso,'mariost.tpl',tpl)
    raw=tpl.read_bytes();table=struct.unpack_from('>I',raw,8)[0];header=struct.unpack_from('>I',raw,table+6*8)[0]
    if struct.unpack_from('>HHI',raw,header)!=(4,64,6) or struct.unpack_from('>II',raw,header+12)!=(1,1):raise ValueError('Original prompt pattern codec/wrap changed')
    pattern=work/'title-prompt-pattern.png'
    patternFingerprint=hashlib.sha256(Path(__file__).read_bytes()+tpl.read_bytes()+Path('tools/iso_assets.py').read_bytes()).hexdigest()
    patternMeta=out/'title-prompt-pattern.json'
    if not(patternMeta.exists() and json.loads(patternMeta.read_text()).get('fingerprint')==patternFingerprint and all((out/('title-prompt-pattern'+e)).is_file()for e in ['.t3x','.size'])):
        convert_tpl(tpl,pattern,6);im=Image.open(pattern).convert('RGBA');width,height=im.size
        if width!=64 or height!=4:raise ValueError('Original title prompt pattern dimensions changed')
        padded=Image.new('RGBA',(64,8));padded.paste(im,(0,0));padded.paste(im,(0,4));padded.save(pattern)
        tool=shutil.which('tex3ds')or'/opt/devkitpro/tools/bin/tex3ds'
        subprocess.run([tool,'-f','rgba8','-z','auto','-o',str(out/'title-prompt-pattern.t3x'),str(pattern)],check=True,stdout=subprocess.DEVNULL)
        (out/'title-prompt-pattern.size').write_bytes(struct.pack('<4I',width,height,width,8))
        patternMeta.write_text(json.dumps(dict(fingerprint=patternFingerprint,source='mariost.tpl',index=6,dimensions=[64,4],native=[64,8],wrap=[1,1],conversion='OriginalRGBA8 pattern repeated vertically into8px native minimum; sampled inverseVscale. No generated image.',license='Original Nintendo/Intelligent Systems; local use only, no redistribution rights'),indent=2)+'\n')
    for suffix,bank,poses,spans,keys in [('center','OFF_d_maku_center',['Z_2','A_2'],[0,160],[1,161]),('sides','OFF_d_maku_left_right',['A_1','Z_1'],[80,60],[14,1])]:
        name='title-curtain-'+suffix;model=work/bank;tpl=work/(bank+'.tpl')
        extract(iso,'a/'+bank,model);extract(iso,'a/'+bank+'-',tpl)
        helpers=['iso_assets.py','character_surface_mesh.py','character_animation_pack.py','character_animation_data.py','character_bake.py']
        fingerprint=hashlib.sha256(Path(__file__).read_bytes()+model.read_bytes()+tpl.read_bytes()+b''.join(Path('tools',n).read_bytes()for n in helpers)).hexdigest()
        meta=out/(name+'.json')
        if meta.exists() and json.loads(meta.read_text()).get('fingerprint')==fingerprint and all((out/(name+e)).is_file()for e in ['.pmanim','.t3x','.bin']):continue
        for pose,span,count in zip(poses,spans,keys):
            a=animation(model.read_bytes(),pose)
            if a['loop'] or a['start']!=0 or a['end']!=span or len(a['frames'])!=count:raise ValueError('Original title curtain changed')
        mesh=compile_surface_mesh([(model,tpl,poses)],out/(name+'.pmanim'),work/(name+'.png'))
        im=Image.open(work/(name+'.png'));width,height=im.size;im.resize((width//2,height//2),Image.Resampling.LANCZOS).save(work/(name+'.png'))
        tool=shutil.which('tex3ds')or'/opt/devkitpro/tools/bin/tex3ds'
        subprocess.run([tool,'-f','rgba8','-z','auto','-o',str(out/(name+'.t3x')),str(work/(name+'.png'))],check=True,stdout=subprocess.DEVNULL)
        (out/(name+'.bin')).write_bytes(struct.pack('<8sfII',b'PMTIT001',width/height,len(poses),mesh['textures']))
        meta.write_text(json.dumps(dict(fingerprint=fingerprint,source='a/'+bank,texture='a/'+bank+'-',poses=poses,spans=spans,keys=keys,context=2,uvScale=width/height,mesh=mesh,conversion='Original geometry/materials/UV/wrap/pass/cull/RGBA; sparse original hierarchy retained. GPU atlas half-size Lanczos,124px source cells. Native image-plane projection adapter.',license='Original Nintendo/Intelligent Systems; local use only, no redistribution rights'),indent=2)+'\n')
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs');a=p.parse_args();build(a.iso,a.output)
