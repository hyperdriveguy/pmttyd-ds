#!/usr/bin/env python3
"""Export unchanged original window0 with complete native clamp-edge padding."""
import argparse
import hashlib
import json
import shutil
import struct
import subprocess
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO, inventory, decode_gx

SOURCE_SHA256 = '2ef0cdc818d2402de5bfb95394f67d78017e74bc1b41c38b79780cb0b870dc67'
ATLAS_FNV = 0xcaadb76327b7d2be

def atlas(raw):
    if len(raw) != 321984 or hashlib.sha256(raw).hexdigest() != SOURCE_SHA256:
        raise ValueError('Original US msgWindow.tpl identity changed')
    u32 = lambda p: struct.unpack_from('>I', raw, p)[0]
    if u32(0) != 0x20af30 or u32(4) != 51:
        raise ValueError('Original palette header changed')
    header, clut = struct.unpack_from('>II', raw, u32(8))
    h, w = struct.unpack_from('>HH', raw, header)
    if clut or (w,h,u32(header+4)) != (72,72,14) or tuple(u32(header+i) for i in (12,16,20,24,28)) != (0,0,1,1,0):
        raise ValueError('Original descriptor0 layout/sampler changed')
    start=u32(header+8)
    source=Image.new('RGBA',(72,72));source.putdata(decode_gx(raw[start:start+2592],72,72,14))
    result=Image.new('RGBA',(128,128));result.putdata([source.getpixel((min(x,71),min(y,71))) for y in range(128) for x in range(128)])
    hsh=14695981039346656037
    for b in result.tobytes():hsh=((hsh^b)*1099511628211)&((1<<64)-1)
    if hsh != ATLAS_FNV:
        raise ValueError('Original atlas pixels changed')
    return source,result

def build(iso,output):
    output=Path(output)
    if output.exists() and any(output.iterdir()):
        raise ValueError('Export requires empty destination')
    with Path(iso).open('rb') as source:
        if source.read(8)!=b'G8ME01\0\0':raise ValueError('US revision-0 ISO required')
    entries=[e for e in inventory(Path(iso))['files'] if e['path']=='f/msgWindow.tpl']
    if len(entries)!=1:raise ValueError('Original file missing/ambiguous')
    entry=entries[0]
    with Path(iso).open('rb') as f:f.seek(entry['offset']);raw=f.read(entry['size'])
    source,image=atlas(raw);output.mkdir(parents=True,exist_ok=True)
    image.save(output/'battle-window-panel.png')
    tool=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
    subprocess.run([tool,'-f','rgba8','-z','auto','-o',str(output/'battle-window-panel.t3x'),str(output/'battle-window-panel.png')],check=True,stdout=subprocess.DEVNULL)
    data=dict(fingerprint=hashlib.sha256(raw+Path(__file__).read_bytes()+Path(__file__).with_name("iso_assets.py").read_bytes()).hexdigest(),texture_sha256=hashlib.sha256((output/"battle-window-panel.t3x").read_bytes()).hexdigest(),source_path=entry['path'],source_offset=entry['offset'],source_size=len(raw),source_sha256=SOURCE_SHA256,index=0,source_dimensions=[72,72],native_dimensions=[128,128],source_rgba_sha256=hashlib.sha256(source.tobytes()).hexdigest(),native_rgba_sha256=hashlib.sha256(image.tobytes()).hexdigest(),native_rgba_fnv64=f'{ATLAS_FNV:016x}',conversion='Unmodified original CMPR72x72 at0,0; complete right/bottom/corner clamp-edge replication; no resize; UV scale72/128; native RGBA8',license='Original Nintendo/Intelligent Systems rights retained; local original source, no redistribution rights')
    (output/'battle-window-panel.json').write_text(json.dumps(data,indent=2)+'\n')
    return data

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',required=True);a=p.parse_args();print(build(a.iso,a.output))
