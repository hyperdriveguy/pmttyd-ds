#!/usr/bin/env python3
"""Private original common-bank enemy HP17/18 atlas; exact pixels, no resize."""
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

BLOCK_HASHES = (
    '1968aec5606a2dc326d272efa0602a98ebd08ab15a99b53014e8bedd26a73ddd',
    '21c140ae6dae0bda018f9468b364fb733789d5ba60deacb8879acc8e516bc202',
)


def verify_source(raw):
    if len(raw) != SOURCE_SIZE or hashlib.sha256(raw).hexdigest() != SOURCE_HASH:
        raise ValueError('Original US common battle texture changed')
    if struct.unpack_from('>III', raw) != (0x0020af30, 97, 12):
        raise ValueError('Original common TPL root changed')
    records = []
    for index in (17, 18):
        header, palette = struct.unpack_from('>II', raw, 12 + index * 8)
        height, width, fmt, offset = struct.unpack_from('>HHII', raw, header)
        if (header, palette, width, height, fmt, offset) != (
                0x578 + (index - 17) * 36, 0, 40, 16, 14,
                0xb100 + (index - 17) * 320):
            raise ValueError('Original HP descriptor changed')
        digest = hashlib.sha256(raw[offset:offset + 320]).hexdigest()
        if digest != BLOCK_HASHES[index - 17]:
            raise ValueError('Original HP CMPR bytes changed')
        records.append(dict(index=index, width=width, height=height, format=fmt,
                            header=header, data=offset, bytes=320,
                            compressed_sha256=digest))
    return records


def build(iso=DEFAULT_ISO, output='romfs', work='local-assets/battle-enemy-hp'):
    out, cache = Path(output), Path(work)
    out.mkdir(parents=True, exist_ok=True)
    cache.mkdir(parents=True, exist_ok=True)
    tpl = cache / 'battle_common.tpl'
    extract(iso, SOURCE, tpl)
    records = verify_source(tpl.read_bytes())
    atlas = Image.new('RGBA', (128, 32))
    payload = bytearray(struct.pack('<8sI', b'PMBEHP01', 2))
    for record in records:
        index = record['index']
        path = cache / f'common-{index}.png'
        convert_tpl(tpl, path, index)
        bitmap = Image.open(path).convert('RGBA')
        if bitmap.size != (40, 16):
            raise ValueError('Original HP decoded dimensions changed')
        x, y = 2 + 44 * (index - 17), 2
        atlas.paste(bitmap, (x, y))
        uv = (x / 128, 1 - y / 32, (x + 40) / 128, 1 - (y + 16) / 32)
        payload += struct.pack('<3I4f', index, 40, 16, *uv)
        record.update(x=x, y=y, uv=uv,
                      pixel_sha256=hashlib.sha256(bitmap.tobytes()).hexdigest())
    atlas.save(cache / 'atlas.png')
    tex3ds = shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
    subprocess.run([tex3ds, '-f', 'rgba8', '-z', 'auto', '-o',
                    str(out / 'battle-enemy-hp-surfaces.t3x'),
                    str(cache / 'atlas.png')], check=True)
    (out / 'battle-enemy-hp-surfaces.bin').write_bytes(payload)
    metadata = dict(source=SOURCE, source_sha256=SOURCE_HASH,
                    dimensions=[128, 32], texture_bytes=16384, images=records,
                    conversion='Original RGBA pixels;2px transparent padding;no resampling',
                    license='Nintendo/Intelligent Systems resources;no redistribution rights')
    (out / 'battle-enemy-hp-surfaces.json').write_text(json.dumps(metadata, indent=2) + '\n')
    return metadata


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--output', default='romfs')
    parser.add_argument('--work', default='local-assets/battle-enemy-hp')
    args = parser.parse_args()
    build(args.iso, args.output, args.work)
