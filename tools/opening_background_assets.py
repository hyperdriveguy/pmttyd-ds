#!/usr/bin/env python3
"""Bounded read-only original opening backdrop conversion; no invented sky."""
import argparse,json,shutil,struct,subprocess
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO,extract,convert_tpl
ROOMS=('aaa_00','gor_10','gor_00','dmo_00')
def build(iso,out):
    out=Path(out);out.mkdir(parents=True,exist_ok=True)
    work=Path('local-assets/opening-backgrounds');work.mkdir(parents=True,exist_ok=True)
    records=[];tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
    for room in ROOMS:
        name='opening-bg-'+room;tpl=work/(room+'.tpl');png=work/(room+'.png')
        extract(iso,'b/'+room+'.tpl',tpl);convert_tpl(tpl,png,0)
        im=Image.open(png).convert('RGBA');w,h=im.size
        # Original backdrops are512x480; retain every original pixel. Padding
        # is transparent and excluded by the native source-sized subtexture.
        canvas=Image.new('RGBA',(1<<(w-1).bit_length(),1<<(h-1).bit_length()))
        canvas.paste(im,(0,0));canvas.save(png)
        subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(out/(name+'.t3x')),str(png)],check=True,stdout=subprocess.DEVNULL)
        (out/(name+'.size')).write_bytes(struct.pack('<4I',w,h,canvas.width,canvas.height))
        records.append(json.loads(Path(str(tpl)+'.provenance.json').read_text()))
    (out/'opening-backgrounds.json').write_text(json.dumps(dict(sources=records,conversion='Original palette0 RGBA, no rescaling; transparent power-of-two padding; native screen viewport adapter',license='Nintendo/Intelligent Systems original assets; local use only, no redistribution rights'),indent=2)+'\n')
    return records
if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs');a=p.parse_args();build(a.iso,a.output)
