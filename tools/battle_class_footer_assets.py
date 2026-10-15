#!/usr/bin/env python3
"""Private original battle common-bank button/danger/critical atlas, no resize."""
import argparse
import hashlib
import json
import shutil
import struct
import subprocess
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO, extract, convert_tpl
from battle_class_surface_assets import SOURCE, SOURCE_HASH, SOURCE_SIZE
from verify_class_footer import verify


def build(iso=DEFAULT_ISO, output='romfs', work='local-assets/battle-class-footers'):
    evidence = verify(iso)
    out, cache = Path(output), Path(work)
    out.mkdir(parents=True, exist_ok=True)
    cache.mkdir(parents=True, exist_ok=True)
    tpl = cache/'battle_common.tpl'
    extract(iso, SOURCE, tpl)
    raw = tpl.read_bytes()
    if len(raw) != SOURCE_SIZE or hashlib.sha256(raw).hexdigest() != SOURCE_HASH:
        raise ValueError('Original US common battle texture changed')
    # Source Sub uses4/5; verified source-language danger/critical tables use6..16.
    # Deliberately retain all six source languages, including their index7 alias.
    danger, critical = [6, 7, 8, 7, 9, 10], [11, 12, 13, 14, 15, 16]
    atlas = Image.new('RGBA', (512, 256))
    payload = bytearray(struct.pack('<8sI', b'PMBCLF01', 13))
    rows = []
    for index in range(4, 17):
        path = cache/f'common-{index}.png'
        convert_tpl(tpl, path, index)
        bitmap = Image.open(path).convert('RGBA')
        width, height = bitmap.size
        expected = (80, 32) if index < 6 else (112, 48 if index < 11 else 56)
        if bitmap.size != expected:
            raise ValueError('Original footer surface dimensions changed')
        column, row = (index-4) % 4, (index-4) // 4
        x, y = 2+116*column, 2+60*row
        atlas.paste(bitmap, (x, y))
        uv = (x/512, 1-y/256, (x+width)/512, 1-(y+height)/256)
        payload += struct.pack('<3I4f', index, width, height, *uv)
        rows.append(dict(index=index, width=width, height=height, x=x, y=y,
                         uv=uv, pixel_sha256=hashlib.sha256(bitmap.tobytes()).hexdigest()))
    atlas.save(cache/'atlas.png')
    tex3ds = shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
    subprocess.run([tex3ds, '-f', 'rgba8', '-z', 'auto', '-o',
                    str(out/'battle-class-footers.t3x'), str(cache/'atlas.png')], check=True)
    (out/'battle-class-footers.bin').write_bytes(payload)
    metadata = dict(source=SOURCE, source_sha256=SOURCE_HASH, dimensions=[512, 256],
                    texture_bytes=524288, images=rows, danger=danger, critical=critical,
                    source_verifier=evidence, conversion='Original RGBA pixels;2px transparent padding;no resampling',
                    license='Nintendo/Intelligent Systems resources;no redistribution rights')
    (out/'battle-class-footers.json').write_text(json.dumps(metadata, indent=2)+'\n')
    return metadata

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--output', default='romfs')
    parser.add_argument('--work', default='local-assets/battle-class-footers')
    args = parser.parse_args()
    build(args.iso, args.output, args.work)
