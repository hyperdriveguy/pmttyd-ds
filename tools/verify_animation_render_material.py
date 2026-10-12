#!/usr/bin/env python3
"""Verify US animation shape renderer and material request bodies.

Static evidence only;no original game instruction is executed.
"""
import argparse
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO, inventory
from verify_field_lecture_callbacks import verify
from verify_animation_main import verify_resource_example

MANIFEST = Path(__file__).with_name('animation_render_material_evidence.json')


def verify_census(iso, expected):
    census = expected['direct_reference_census']
    if census['text_sections'] != 7:
        raise ValueError('Expected all seven DOL text sections')
    found = {target['address']: [] for target in census['targets']}
    with Path(iso).open('rb') as disc:
        disc.seek(0x420)
        base = struct.unpack('>I', disc.read(4))[0]
        disc.seek(base)
        header = disc.read(256)
        for i in range(7):
            off, va, size = (struct.unpack_from('>I', header, j+4*i)[0]
                             for j in (0, 0x48, 0x90))
            if not size:
                continue
            disc.seek(base+off)
            raw = disc.read(size)
            if len(raw) != size:
                raise ValueError('Truncated DOL text section')
            for k in range(0, len(raw)-3, 4):
                word = struct.unpack_from('>I', raw, k)[0]
                if word >> 26 != 18:
                    continue
                displacement = word & 0x03fffffc
                if displacement & 0x02000000:
                    displacement -= 0x04000000
                target = (displacement if word & 2 else va+k+displacement) & 0xffffffff
                if target in found:
                    found[target].append(dict(address=va+k, link=bool(word & 1)))
    for target in census['targets']:
        if found[target['address']] != target['sites']:
            raise ValueError('Animation render/material direct-reference census mismatch')
    return census['targets']


def verify_render_resource(iso, expected):
    from collections import Counter
    spec = expected['resource_example']
    entry = next(e for e in inventory(iso)['files'] if e['path'] == spec['path'])
    with Path(iso).open('rb') as disc:
        disc.seek(entry['offset'])
        raw = disc.read(entry['size'])
    def word(o):
        return struct.unpack_from('>I', raw, o)[0]
    def histogram(values):
        return [dict(value=k,count=v) for k,v in sorted(Counter(values).items())]
    shape,draw,call,texture = [word(o) for o in (0x14c,0x19c,0x150,0x198)]
    ns,nd,nc,nt = [word(o) for o in (0xe8,0x138,0xec,0x134)]
    counts = [word(call+8*i+4) & 0xffff for i in range(nc)]
    found = dict(shape_count=ns,draw_count=nd,call_count=nc,texture_count=nt,
                 shape_mode_histogram=histogram(word(shape+168*i+160) for i in range(ns)),
                 shape_cull_histogram=histogram(word(shape+168*i+164) for i in range(ns)),
                 draw_texture_count_histogram=histogram(word(draw+108*i) for i in range(nd)),
                 texture_kind_histogram=histogram(word(texture+64*i+8) for i in range(nt)),
                 draw_single_texcoord_histogram=histogram(struct.unpack_from('>b',raw,draw+108*i+48)[0] for i in range(nd)),
                 call_count_low16_histogram=histogram(counts),
                 call_vertices_low16_total=sum(counts),
                 fan_triangles_all_calls=sum(max(0,n-2) for n in counts))
    if found != expected['resource_render_example']:
        raise ValueError('Original shape/draw/call/texture metadata mismatch')
    return found


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    if args.output and args.output.resolve() in (Path(args.iso).resolve(), MANIFEST.resolve()):
        parser.error('Cannot overwrite original ISO or pinned manifest')
    result = verify(args.iso, MANIFEST)
    expected = json.loads(MANIFEST.read_text())
    result['binary32_constants'] = [dict(address=c['address'], word=c['hex'],
                                       value=struct.unpack('>f', bytes.fromhex(c['hex']))[0])
                                    for c in expected['constants']]
    result['direct_reference_sites'] = verify_census(args.iso, expected)
    result['resource_example'] = verify_resource_example(args.iso, expected)
    result['resource_render_example'] = verify_render_resource(args.iso, expected)
    result['scope'] = 'Static complete renderProc/materialProc local request bodies;texture/map/GX/SDK/resource/source/runtime domains open'
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(json.dumps(result, indent=2)+'\n')
    print(f"Verified {result['body_checks']} containing bodies, {result['instruction_checks']} instructions, "
          f"{result['constant_checks']} constants, {result['data_checks']} spans, "
          f"{result['import_checks']} imports and {sum(len(t['sites']) for t in result['direct_reference_sites'])} direct caller sites and one original shape/draw resource example")


if __name__ == '__main__':
    main()
