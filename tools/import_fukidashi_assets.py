#!/usr/bin/env python3
"""Import original Crump fukidashi art into an empty editable destination."""
import argparse
import hashlib
import json
from pathlib import Path
import struct

from PIL import Image
from iso_assets import DEFAULT_ISO, extract, convert_tpl

SOURCE = 'e/us/effect.tpl'
SOURCE_HASH = '3b62850fe75afb95eebb9fc240e20204d5ed320fba0fc956729f28405c2c789b'
LICENSE = 'Nintendo/Intelligent Systems original game asset; no redistribution license granted'
TEXTURES = ((4, 'bubble', (2, 2)), (5, 'shadow', (38, 2)),
            (6, 'exclamation', (74, 2)), (7, 'question', (2, 70)))
ATLAS_SIZE = (256, 128)
PADDING = 2


def build(iso, destination, work, root):
    destination, work, root = (Path(p).resolve() for p in (destination, work, root))
    # A fresh import must never replace an authored edit or previous extraction.
    if destination == work or destination in work.parents or work in destination.parents:
        raise ValueError('Separate editable and temporary destinations required')
    relative = destination.relative_to(root)
    for path in (destination, work):
        if path.exists() and (not path.is_dir() or any(path.iterdir())):
            raise ValueError(f'Import destination must be empty: {path}')
    for path in (destination, work):
        path.mkdir(parents=True, exist_ok=True)
    source = work / 'effect.tpl'
    extract(iso, SOURCE, source)
    raw = source.read_bytes()
    if hashlib.sha256(raw).hexdigest() != SOURCE_HASH:
        raise ValueError('Original US base effect bank differs')
    table = struct.unpack_from('>I', raw, 8)[0]
    frames, records = [], []
    for index, name, position in TEXTURES:
        descriptor, palette = struct.unpack_from('>II', raw, table + index * 8)
        height, width, fmt = struct.unpack_from('>HHI', raw, descriptor)
        output = destination / (name + '.png')
        convert_tpl(source, output, index)
        with Image.open(output) as decoded:
            image = decoded.convert('RGBA')
            if image.size != (width, height):
                raise ValueError('Decoded source dimensions differ')
            pixel_hash = hashlib.sha256(image.tobytes()).hexdigest()
        frames.append(dict(name=name, source=str(relative / output.name), position=position))
        records.append(dict(name=name, source=SOURCE, source_sha256=SOURCE_HASH,
                            texture_index=index, size=[width, height], format=fmt,
                            palette_offset=palette, descriptor_hex=raw[descriptor:descriptor + 36].hex(),
                            rgba_sha256=pixel_hash, file_sha256=hashlib.sha256(output.read_bytes()).hexdigest(),
                            license=LICENSE))
    (destination / 'atlas.json').write_text(json.dumps(dict(size=ATLAS_SIZE, padding=PADDING, frames=frames), indent=2) + '\n')
    provenance = json.loads(source.with_suffix('.tpl.provenance.json').read_text())
    (destination / 'provenance.json').write_text(json.dumps(dict(source=provenance, assets=records,
        conversion='Unscaled original decoded RGBA; atlas adds two edge-extruded texels. Original descriptor/UV animation semantics remain separate.',
        license=LICENSE), indent=2) + '\n')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--destination', required=True)
    parser.add_argument('--work', required=True)
    parser.add_argument('--root', default='.')
    args = parser.parse_args()
    build(args.iso, args.destination, args.work, args.root)
