#!/usr/bin/env python3
"""Import original audience banks and pack their unchanged pixels for native use."""
import argparse
import hashlib
import json
import shutil
import struct
import subprocess
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO, extract, decode_gx

BANKS = (
    ('audience_normal.tpl', 'audience-normal.tpl', 428896, 205,
     '16f8cdda26bb3727aeb9c2d7b494d66c1822d2ac94705dbad07b09d319e5e4f2'),
    ('audience_luigi.tpl', 'audience-luigi.tpl', 39776, 19,
     '3f556a8d9bd7cbfbba33ce3ab1a31531f174273451fe620f54182523533c48ef'),
)


def build(iso=DEFAULT_ISO, output='romfs', work='local-assets/audience'):
    root = Path(work)
    root.mkdir(parents=True, exist_ok=True)
    out = Path(output)
    out.mkdir(parents=True, exist_ok=True)
    atlas = Image.new('RGBA', (1024, 1024))
    frames = []
    # Native bank handles 1/2 retain each original TPL index independently.
    payload = bytearray(struct.pack('<8sIII', b'PMAUD001', 1024, 1024, 224))
    banks = []
    for handle, (name, target, size, count, digest) in enumerate(BANKS, 1):
        source = 'battle/audience/' + name
        path = root / name
        extract(iso, source, path)
        data = path.read_bytes()
        if len(data) != size or hashlib.sha256(data).hexdigest() != digest:
            raise ValueError('Original US audience TPL changed')
        magic, n, table = struct.unpack_from('>III', data)
        if magic != 0x20af30 or n != count or table + n * 8 > len(data):
            raise ValueError('Invalid original audience TPL table')
        for index in range(n):
            image, palette = struct.unpack_from('>II', data, table + 8 * index)
            if image + 36 > len(data):
                raise ValueError('Invalid original audience header')
            h, w, fmt, pixels = struct.unpack_from('>HHII', data, image)
            if palette or (w, h, fmt) != (64, 64, 14) or pixels + 2048 > len(data):
                raise ValueError('Invalid original audience image')
            bitmap = Image.new('RGBA', (w, h))
            bitmap.putdata(decode_gx(data[pixels:pixels + 2048], w, h, fmt))
            slot = len(frames)
            x, y = (slot % 15) * 68 + 2, (slot // 15) * 68 + 2
            atlas.paste(bitmap, (x, y))
            # Original GX CLAMP/linear sampler: repeat exact edge texels in the
            # atlas gutter, including the nontransparent border of normal92.
            for dy in range(-2, h + 2):
                for dx in range(-2, w + 2):
                    if 0 <= dx < w and 0 <= dy < h:
                        continue
                    atlas.putpixel((x + dx, y + dy), bitmap.getpixel(
                        (min(max(dx, 0), w - 1), min(max(dy, 0), h - 1))))
            uv = (x / 1024, 1 - y / 1024,
                  (x + w) / 1024, 1 - (y + h) / 1024)
            payload += struct.pack('<4I4f', handle, index, w, h, *uv)
            frames.append(dict(bank=handle, index=index, width=w, height=h,
                               x=x, y=y, uv=uv,
                               pixel_sha256=hashlib.sha256(bitmap.tobytes()).hexdigest()))
        product = out / target
        if not product.exists() or product.read_bytes() != data:
            product.write_bytes(data)
        banks.append(dict(source=source, path=str(product), bytes=len(data),
                          images=n, sha256=digest, format='Original TPL/CMPR64x64'))
    atlas.save(root / 'atlas.png')
    tex3ds = shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
    subprocess.run([tex3ds, '-f', 'rgba8', '-z', 'auto', '-o',
                    str(out / 'audience-frames.t3x'), str(root / 'atlas.png')],
                   check=True, stdout=subprocess.DEVNULL)
    (out / 'audience-frames.bin').write_bytes(payload)
    metadata = dict(
        banks=banks, frames=frames, dimensions=[1024, 1024], texture_bytes=4194304,
        catalog=str(out / 'audience-frames.bin'), texture=str(out / 'audience-frames.t3x'),
        atlas=str(root / 'atlas.png'),
        conversion='Byte-for-byte TPL import; CMPR to RGBA8; 68px cells with 2px original CLAMP edge extrusion; no resampling; original bank/index retained',
        license='Nintendo/Intelligent Systems original assets, local use only; no redistribution rights',
        limitations='Resource preparation only; original population, animation selection, GX draw and throw events require native binding')
    (out / 'audience-resources.json').write_text(json.dumps(metadata, indent=2) + '\n')
    print('Original audience TPL banks:', len(frames), 'images; 4MiB native atlas')
    return metadata


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--output', default='romfs')
    parser.add_argument('--work', default='local-assets/audience')
    args = parser.parse_args()
    build(args.iso, args.output, args.work)
