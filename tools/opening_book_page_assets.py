#!/usr/bin/env python3
"""Export original US demo.tpl page backgrounds3..7 read-only for local use."""
import argparse
import hashlib
import json
import shutil
import struct
import subprocess
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO, extract, convert_tpl

def build(iso, output):
    out=Path(output);work=Path('local-assets/opening-book-pages')
    out.mkdir(parents=True,exist_ok=True);work.mkdir(parents=True,exist_ok=True)
    source=Path(iso)
    fingerprint=hashlib.sha256(Path(__file__).read_bytes()+Path('tools/iso_assets.py').read_bytes()+str((source.stat().st_size,source.stat().st_mtime_ns)).encode()).hexdigest()
    manifest=out/'demo-book-pages.json'
    required=[out/f'demo-{i}{ext}' for i in range(3,8) for ext in ['.size','.t3x']]
    if manifest.exists() and json.loads(manifest.read_text()).get('fingerprint')==fingerprint and all(p.exists() for p in required):return
    tpl=work/'demo.tpl';extract(iso,'demo.tpl',tpl)
    tex3ds=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
    for i in range(3,8):
        png=work/f'demo-{i}.png';convert_tpl(tpl,png,i)
        im=Image.open(png).convert('RGBA')
        if im.size!=(480,608):raise ValueError('Original US book page dimensions changed')
        im=im.resize((240,304),Image.Resampling.LANCZOS)
        canvas=Image.new('RGBA',(256,512));canvas.paste(im,(0,0));canvas.save(png)
        subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(out/f'demo-{i}.t3x'),str(png)],check=True,stdout=subprocess.DEVNULL)
        (out/f'demo-{i}.size').write_bytes(struct.pack('<4I',240,304,256,512))
    manifest.write_text(json.dumps(dict(fingerprint=fingerprint,source=json.loads(tpl.with_suffix('.tpl.provenance.json').read_text()),indices=list(range(3,8)),conversion='Original480x608 RGBA pages half-size Lanczos, transparent256x512 padding',license='Original Nintendo/Intelligent Systems assets for local use only; no redistribution rights'),indent=2)+'\n')

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs');a=p.parse_args();build(a.iso,a.output)
