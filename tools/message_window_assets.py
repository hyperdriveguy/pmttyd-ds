#!/usr/bin/env python3
"""Locally pack original ordinary message border/tail masks; never write the ISO."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import struct
import subprocess
from PIL import Image
from iso_assets import DEFAULT_ISO, inventory, convert_tpl


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--output', default='romfs')
    args = parser.parse_args()
    iso, out = Path(args.iso), Path(args.output)
    info = inventory(iso)
    if info['disc_id'] != 'G8ME01':
        raise ValueError('Original US ISO required')
    entry = next(e for e in info['files'] if e['path'] == 'f/msgWindow.tpl')
    with iso.open('rb') as disc:
        disc.seek(entry['offset'])
        raw = disc.read(entry['size'])
    if len(raw) != entry['size'] or struct.unpack_from('>I', raw, 4)[0] != 51:
        raise ValueError('Original window palette changed')
    fingerprint = hashlib.sha256(raw + Path(__file__).read_bytes() + Path('tools/iso_assets.py').read_bytes()).hexdigest()
    manifest = out / 'message-window.json'
    if manifest.exists() and (out / 'message-window.t3x').exists() and json.loads(manifest.read_text()).get('fingerprint') == fingerprint:
        return
    work = Path('local-assets/fonts'); work.mkdir(parents=True, exist_ok=True)
    tpl = work / 'msgWindow.tpl'; tpl.write_bytes(raw)
    atlas = Image.new('RGBA', (1024, 512))
    for row, first in enumerate((9, 13)):
        parts = []
        for i in range(first, first + 4):
            target = work / f'window-{i}.png'
            convert_tpl(tpl, target, i)
            parts.append(Image.open(target).convert('RGBA'))
        if [p.size for p in parts] != [(72,176),(72,176),(416,40),(416,40)]:
            raise ValueError('Original ordinary border dimensions changed')
        # Source quads: caps at0/488, top at72/0, bottom at72/136.
        # Interior uses the matching flat edge color, preserving source masks.
        bubble = Image.new('RGBA', (560,176))
        fill = Image.new('RGBA', (416,96), parts[0].getpixel((71,88)))
        bubble.paste(fill, (72,40))
        for part, xy in zip(parts, ((0,0),(488,0),(72,0),(72,136))):
            bubble.alpha_composite(part, xy)
        atlas.alpha_composite(bubble, (0,row*176))
    for i in (1,2):
        target = work / f'window-{i}.png'; convert_tpl(tpl,target,i)
        atlas.alpha_composite(Image.open(target).convert('RGBA'), (600,(i-1)*64))
    for i in (3,4):
        target = work / f'window-{i}.png'; convert_tpl(tpl,target,i)
        atlas.alpha_composite(Image.open(target).convert('RGBA'), (600,128+(i-3)*64))
    png = work / 'message-window.png'; atlas.save(png)
    out.mkdir(parents=True,exist_ok=True)
    tex3ds = shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
    subprocess.run([tex3ds,'-f','rgba8','-z','auto','-o',str(out/'message-window.t3x'),str(png)],check=True,stdout=subprocess.DEVNULL)
    manifest.write_text(json.dumps(dict(fingerprint=fingerprint,source_path=entry['path'],source_offset=entry['offset'],sha256=hashlib.sha256(raw).hexdigest(),indices=[1,2,3,4,9,10,11,12,13,14,15,16],conversion='GX masks to RGBA8, lossless border composition at original560x176; no source resize; native draw adapts to400x240',license='Original Nintendo/Intelligent Systems local asset; no redistribution rights'),indent=2)+'\n')


if __name__ == '__main__':
    main()
