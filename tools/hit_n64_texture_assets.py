#!/usr/bin/env python3
"""Package original HitN64 image74 and source indexed vertex bank privately."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import struct
import subprocess
from PIL import Image
from hit_n64_source_evidence import verify
from iso_assets import DEFAULT_ISO

def build(iso=DEFAULT_ISO, output=Path('romfs'), work=Path('local-assets/hit-n64')):
    output, work = Path(output), Path(work)
    proof = verify(iso, work)
    tpl = (work/'effect_n64.tpl').read_bytes()
    image_header = tpl[0xf2c:0xf2c+36]
    # Original immutable vertex bytes are deliberately retained, not flattened.
    with Path(iso).open('rb') as disc:
        disc.seek(0x420); base=struct.unpack('>I',disc.read(4))[0];disc.seek(base);header=disc.read(256)
        matches=[]
        for i in range(18):
            off,at,length=(struct.unpack_from('>I',header,p+4*i)[0] for p in (0,0x48,0x90))
            if at<=0x803a0458 and 0x803a0458+280<=at+length:matches.append(base+off+0x803a0458-at)
        if len(matches)!=1:raise ValueError('Original vertex bank not mapped')
        disc.seek(matches[0]); vertices=disc.read(280)
        source_spans=[]
        for address,size in ((0x802fb0a0,48),(0x80419888,52),(0x80425458,40)):
            source_matches=[]
            for i in range(18):
                off,at,length=(struct.unpack_from('>I',header,p+4*i)[0] for p in (0,0x48,0x90))
                if at<=address and address+size<=at+length:source_matches.append(base+off+address-at)
            if len(source_matches)!=1:raise ValueError('Original immutable Hit span not mapped')
            disc.seek(source_matches[0]);source_spans.append(disc.read(size))
        helpers=[]
        for address,size,expected in ((0x801d78b0,0x60,'3532b321a207e74d818bdabd00a2a0415261ead29507030b0eac166bd6f5d08f'),(0x801d771c,0xc8,'e35872f0f5164edadfb6f11b4ade065b49e9f9c35160402cd47c720bac881768'),(0x801d76a0,0x50,'af63fdb3a5065d4602fed504c2983eb16046e124164fee375a0637f28463965d')):
            helper_matches=[]
            for i in range(18):
                off,at,length=(struct.unpack_from('>I',header,p+4*i)[0] for p in (0,0x48,0x90))
                if at<=address and address+size<=at+length:helper_matches.append(base+off+address-at)
            if len(helper_matches)!=1:raise ValueError('Original helper not uniquely mapped')
            disc.seek(helper_matches[0]);actual=hashlib.sha256(disc.read(size)).hexdigest()
            if actual!=expected:raise ValueError('Original N64 texture/vertex helper changed')
            helpers.append(dict(address=address,bytes=size,sha256=actual))
    if hashlib.sha256(vertices).hexdigest()!=proof['vertex']['sha256']:raise ValueError('Original vertex bank changed')
    image=Image.open(work/'texture74.png').convert('RGBA')
    if image.size!=(32,128):raise ValueError('Original image dimensions changed')
    output.mkdir(parents=True,exist_ok=True)
    metadata=output/'hit-n64-texture74.bin';texture=output/'hit-n64-texture74.t3x';bank=output/'hit-n64-bank.tpl'
    metadata.write_bytes(b'PMHITX01'+struct.pack('<3I',74,32,128)+image_header+vertices+b''.join(source_spans))
    bank.write_bytes(tpl)
    subprocess.run([shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds','-f','rgba8','-z','auto','-o',str(texture),str(work/'texture74.png')],check=True,stdout=subprocess.DEVNULL)
    proof.update(source_helpers=helpers,native_metadata_format='PMHITX01',immutable_spans=[dict(address=address,bytes=len(data),sha256=hashlib.sha256(data).hexdigest()) for address,data in zip((0x802fb0a0,0x80419888,0x80425458),source_spans)],rgba_sha256=hashlib.sha256(image.tobytes()).hexdigest(),
                 conversion='Exact original32x128 I4 intensity inRGBA channels; no resizing/padding/atlas/mips. Nativebind mustapplyREPEATboth/LINEARboth. RaworiginalTPL retained separately for actual source palette/file ownership.',
                 products={p.name:dict(bytes=p.stat().st_size,sha256=hashlib.sha256(p.read_bytes()).hexdigest()) for p in (metadata,texture,bank)},
                 readiness='Native texture import prerequisite only. No real source palette/state mapping, queued callback/GPU material, effect initialization or normal audience activation.')
    (output/'hit-n64-texture74.json').write_text(json.dumps(proof,indent=2)+'\n')
    return proof

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',type=Path,default=Path('romfs'));p.add_argument('--work',type=Path,default=Path('local-assets/hit-n64'));a=p.parse_args();build(a.iso,a.output,a.work)
