#!/usr/bin/env python3
"""Import the original US N64-bank shadow texture, retaining its sampler."""
import argparse
import hashlib
import json
import shutil
import struct
import subprocess
from pathlib import Path

from PIL import Image
from iso_assets import DEFAULT_ISO, convert_tpl, extract

RESOURCE = 'e/us/effect_n64.tpl'
RESOURCE_SHA = '3202ac8f437a2a7664d791ec5d9ee1988504f51b912c1211429708fac0a268ef'
PIXELS_SHA = '77ffbe3002dcd46b807f7a2513393b9aa8e75c06012fa6eca259458109e8e2bb'


def descriptor(data):
    """Reject a different bank/revision or a changed source sampler."""
    if hashlib.sha256(data).hexdigest() != RESOURCE_SHA:
        raise ValueError('Original US effect_n64.tpl changed')
    if struct.unpack_from('>3I', data) != (0x20af30, 151, 12):
        raise ValueError('Original N64 TPL descriptor table changed')
    header, palette = struct.unpack_from('>2I', data, 12 + 44 * 8)
    fields = struct.unpack_from('>HH6If4B', data, header)
    if palette != 0 or fields != (16, 16, 0, 66496, 1, 1, 1, 1, 0., 0, 0, 0, 0):
        raise ValueError('Original shadow texture44 dimensions/format/sampler changed')
    return dict(index=44, dimensions=[16, 16], format='GX_I4',
                wrap_s='REPEAT', wrap_t='REPEAT', min_filter='LINEAR',
                mag_filter='LINEAR', mipmaps=False, lod_bias=0,
                header_sha256=hashlib.sha256(data[header:header+36]).hexdigest(),
                tiled_sha256=hashlib.sha256(data[66496:66496+128]).hexdigest())


def build(iso=DEFAULT_ISO, output='romfs'):
    out = Path(output)
    out.mkdir(parents=True, exist_ok=True)
    work = Path('local-assets/shadow')
    work.mkdir(parents=True, exist_ok=True)
    fingerprint = hashlib.sha256(
        Path(__file__).read_bytes() + Path('tools/iso_assets.py').read_bytes() +
        str((Path(iso).stat().st_size, Path(iso).stat().st_mtime_ns)).encode()).hexdigest()
    meta = out / 'shadow-texture44.json'
    if meta.exists() and json.loads(meta.read_text()).get('fingerprint') == fingerprint and (out/'shadow-texture44.t3x').is_file():
        return
    tpl = work / 'effect_n64.tpl'
    extract(iso, RESOURCE, tpl)
    texture = descriptor(tpl.read_bytes())
    png = work / 'texture44.png'
    convert_tpl(tpl, png, 44)
    with Image.open(png) as source:
        image = source.convert('RGBA')
    if image.size != (16, 16) or hashlib.sha256(image.tobytes()).hexdigest() != PIXELS_SHA:
        raise ValueError('Original shadow texture44 pixels changed')
    # Already power-of-two and above PICA's minimum: no atlas, padding,
    # resampling or quantization. Keep I4 intensity in RGB and alpha.
    image.save(png)
    tool = shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds'
    subprocess.run([tool, '-f', 'rgba8', '-z', 'auto', '-o',
                    str(out/'shadow-texture44.t3x'), str(png)],
                   check=True, stdout=subprocess.DEVNULL)
    meta.write_text(json.dumps(dict(
        fingerprint=fingerprint, source=RESOURCE,
        provenance=json.loads(tpl.with_suffix('.tpl.provenance.json').read_text()),
        texture=texture, rgba_sha256=PIXELS_SHA,
        conversion='Exact 16x16 I4 decoded to RGBA8, native tiled texture; no resize/padding/mipmaps. Native binding must apply repeat S/T and linear min/mag.',
        license='Original Nintendo/Intelligent Systems assets for private local use; no redistribution rights'), indent=2)+'\n')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--output', default='romfs')
    args = parser.parse_args()
    build(args.iso, args.output)
