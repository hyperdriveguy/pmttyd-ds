#!/usr/bin/env python3
"""Verify complete original HitN64 Entry/Main and its separate draw dependency.

Read-only US ISO. Report/private decoded original texture grant no new rights
and acknowledge neither native display ownership nor normal effect activation.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO, extract, convert_tpl

BODIES = {
    'wrapper': (0x8013f1b4, 0x20, 'b3da6d977a1b38c8134696a5579c2e8dea750634e587c5ec83992f6bba273c78'),
    'entry': (0x801c6344, 0x270, 'f3032cf8a104c1f1adb6194f71735381179aa86a6f84c2472907e9ff92b18d27'),
    'main': (0x801c61bc, 0x188, '993d240615a64d5e8df4c31394396416d3659c7a6efefa1ced955646129cd95a'),
    'display': (0x801c5d6c, 0x450, '3c0f065f58e9ae41835d7b79ded28c6ea2b6d43879346df11a452fa786ca4c86'),
    'main_dl': (0x801c5b58, 0x214, '765affb2be6158fa08536cd3f8a77dfcab581389f722c59cf58834b151ce0316'),
}
TABLES = {
    0x80005418: bytes.fromhex('3c408042604274403da0804161adcf20'),
    0x80419888: bytes.fromhex('0d0d0d0d'),
    0x8041988c: bytes.fromhex('406890a4b8c0d0d8e0e8f0f8ff'),
    0x8041989c: bytes.fromhex('0a141c232a3035393c3e3f4041'),
    0x804198ac: bytes.fromhex('ffffc8823c0000000000000000'),
    0x802fb0b8: bytes.fromhex('43300000800000004330000000000000'),
    0x80425458: bytes.fromhex('40a00000408000003d0000003c0000000000000042c000003c8efa35438000003b8000003f800000'),
}

def verify(iso=DEFAULT_ISO, work=Path('local-assets/hit-n64')):
    source = Path(iso)
    with source.open('rb') as disc:
        if disc.read(8) != b'G8ME01\0\0':
            raise ValueError('Original US revision0 required')
        disc.seek(0x420)
        base = struct.unpack('>I', disc.read(4))[0]
        disc.seek(base)
        header = disc.read(256)
        def read(address, size):
            matches = []
            for i in range(18):
                off, at, length = (struct.unpack_from('>I', header, p + 4*i)[0] for p in (0, 0x48, 0x90))
                if at <= address and address + size <= at + length:
                    matches.append(base + off + address - at)
            if len(matches) != 1:
                raise ValueError('Source range not uniquely mapped')
            disc.seek(matches[0])
            raw = disc.read(size)
            if len(raw) != size:
                raise ValueError('Truncated DOL range')
            return raw
        bodies = {}
        for name, (address, size, expected) in BODIES.items():
            actual = hashlib.sha256(read(address, size)).hexdigest()
            if actual != expected:
                raise ValueError('Original HitN64 function changed: ' + name)
            bodies[name] = dict(address=address, bytes=size, sha256=actual)
        for address, expected in TABLES.items():
            if read(address, len(expected)) != expected:
                raise ValueError('Original source table changed: ' + hex(address))
        vertex = read(0x803a0458, 0x118)
        vertex_hash = hashlib.sha256(vertex).hexdigest()
    work = Path(work)
    work.mkdir(parents=True, exist_ok=True)
    texture = work / 'effect_n64.tpl'
    extract(iso, 'e/us/effect_n64.tpl', texture)
    raw = texture.read_bytes()
    if hashlib.sha256(raw).hexdigest() != '3202ac8f437a2a7664d791ec5d9ee1988504f51b912c1211429708fac0a268ef':
        raise ValueError('Original N64 TPL changed')
    magic, count, table = struct.unpack_from('>III', raw)
    image, palette = struct.unpack_from('>II', raw, table + 74*8)
    height, width, fmt, data, wrap_s, wrap_t, minimum, maximum = struct.unpack_from('>HH6I', raw, image)
    if (magic, count, image, palette, height, width, fmt, data, wrap_s, wrap_t, minimum, maximum) != (0x20af30,151,0xf2c,0,128,32,0,110720,1,1,1,1):
        raise ValueError('Original HitN64 texture74 metadata changed')
    png = work / 'texture74.png'
    convert_tpl(texture, png, 74)
    return dict(functions=bodies, tables={hex(a):v.hex() for a,v in TABLES.items()},
                vertex=dict(address=0x803a0458, bytes=len(vertex), sha256=vertex_hash),
                texture=dict(source=json.loads(texture.with_suffix('.tpl.provenance.json').read_text()),
                             index=74, format='I4', width=32, height=128, wrap_s=1, wrap_t=1,
                             source_image_header=image, source_data=data, source_image_bytes=2048,
                             decoded_png=str(png), decoded_png_sha256=hashlib.sha256(png.read_bytes()).hexdigest(),
                             conversion='Original pixels decoded at32x128; no resizing or native texture import'),
                ownership='Entry/Main portable complete; actual global EffectPool/heap/lifetime/queue and original Display/main_dl GPU owners required.',
                license='Original Nintendo/Intelligent Systems rights retained; local extraction grants no redistribution rights.')

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--work', type=Path, default=Path('local-assets/hit-n64'))
    parser.add_argument('--report', type=Path, default=Path('artifacts/hit-n64-source/verified.json'))
    args = parser.parse_args()
    report = verify(args.iso, args.work)
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2)+'\n')
    print('Verified original HitN64 Entry/Main/display graph and texture74; native GPU owner remains separate')
