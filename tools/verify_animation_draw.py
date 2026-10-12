#!/usr/bin/env python3
"""Verify US animation draw wrapper,inner,group traversals and translucency leaf.

Static evidence only;no original game instruction is executed.
"""
import argparse
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO, inventory
from verify_field_lecture_callbacks import verify
from verify_animation_main import verify_resource_example

MANIFEST = Path(__file__).with_name('animation_draw_evidence.json')


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
            raise ValueError('Animation draw direct-reference census mismatch')
    return census['targets']


def verify_resource_draw(iso, expected):
    spec = expected['resource_example']
    entry = next(e for e in inventory(iso)['files'] if e['path'] == spec['path'])
    with Path(iso).open('rb') as disc:
        disc.seek(entry['offset'])
        raw = disc.read(entry['size'])
    def word(o):
        return struct.unpack_from('>I', raw, o)[0]
    index = word(0x144)-1
    sibling, child, shape, visibility, node, joint = struct.unpack_from(
        '>iiiIII', raw, word(0x1a8)+88*index+64)
    found = dict(pass_mask=word(0xc4),group_count=word(0x144),
                 group_offset=word(0x1a8),root=dict(index=index,sibling=sibling,
                 child=child,shape=shape,visibility=visibility,node=node,joint=joint))
    if found != expected['resource_draw_example']:
        raise ValueError('Original draw resource metadata mismatch')
    return found


def verify_address_producers(iso, expected):
    with Path(iso).open('rb') as disc:
        disc.seek(0x420)
        base = struct.unpack('>I', disc.read(4))[0]
        disc.seek(base)
        header = disc.read(256)
        def instruction(address):
            spans = [(o,v,n) for i in range(18)
                     for o,v,n in [(struct.unpack_from('>I',header,i*4)[0],
                                   struct.unpack_from('>I',header,0x48+i*4)[0],
                                   struct.unpack_from('>I',header,0x90+i*4)[0])]
                     if v <= address and address+4 <= v+n]
            if len(spans) != 1:
                raise ValueError('Ambiguous address producer mapping')
            off,va,_ = spans[0]
            disc.seek(base+off+address-va)
            return disc.read(4)
        for item in expected['address_producers']:
            hi = instruction(item['high_site'])
            lo = instruction(item['low_site'])
            if hi.hex() != item['high_hex'] or lo.hex() != item['low_hex']:
                raise ValueError('Address producer instruction mismatch')
            high,low = struct.unpack('>I',hi)[0],struct.unpack('>I',lo)[0]
            if high >> 26 != 15 or (high >> 16) & 31 or low >> 26 != 14 or (low >> 16) & 31 != (high >> 21) & 31:
                raise ValueError('Expected matching lis/addi producer')
            signed = low & 0xffff
            if signed & 0x8000:
                signed -= 0x10000
            address = (((high & 0xffff) << 16)+signed) & 0xffffffff
            if address != item['address']:
                raise ValueError('Signed-low original address mismatch')
    return expected['address_producers']


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
    result['resource_draw_example'] = verify_resource_draw(args.iso, expected)
    result['derived_addresses'] = verify_address_producers(args.iso, expected)
    result['scope'] = 'Static complete animation draw wrapper/inner/traversal/translucency bodies;group matrices/render/material/SDK/resource/source/runtime domains open'
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(json.dumps(result, indent=2)+'\n')
    print(f"Verified {result['body_checks']} containing bodies, {result['instruction_checks']} instructions, "
          f"{result['constant_checks']} constants, {result['data_checks']} spans, "
          f"{result['import_checks']} imports and {sum(len(t['sites']) for t in result['direct_reference_sites'])} direct caller sites and one original draw resource example and three derived addresses")


if __name__ == '__main__':
    main()
