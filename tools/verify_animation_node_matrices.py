#!/usr/bin/env python3
"""Verify US animation node builders and their SDK matrix primitives.

Static evidence only;no original game instruction is executed.
"""
import argparse
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO, inventory
from verify_field_lecture_callbacks import verify
from verify_animation_main import verify_resource_example
from verify_animation_draw import verify_address_producers, verify_resource_draw

MANIFEST = Path(__file__).with_name('animation_node_matrices_evidence.json')


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
            raise ValueError('Animation node matrix direct-reference census mismatch')
    return census['targets']


def verify_node_resource(iso, expected):
    from collections import Counter
    entry = next(e for e in inventory(iso)['files']
                 if e['path'] == expected['resource_example']['path'])
    with Path(iso).open('rb') as disc:
        disc.seek(entry['offset'])
        raw = disc.read(entry['size'])
    def word(o):
        return struct.unpack_from('>I', raw, o)[0]
    count,base = word(0x144),word(0x1a8)
    flags = Counter(word(base+88*i+84) for i in range(count))
    nodes = [word(base+88*i+80) for i in range(count)]
    found = dict(group_joint_words=[dict(value=k,count=v) for k,v in sorted(flags.items())],
                 node_float_count=word(0x140),max_node_index=max(nodes),
                 root_node_index=nodes[-1])
    if found != expected['resource_node_example']:
        raise ValueError('Original node resource metadata mismatch')
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
    result['resource_draw_example'] = verify_resource_draw(args.iso, expected)
    result['resource_node_example'] = verify_node_resource(args.iso, expected)
    result['derived_addresses'] = verify_address_producers(args.iso, expected)
    result['scope'] = 'Static complete node builders/SDK matrix primitives and trig wrappers;libm/FPSCR/resource/render/source/runtime domains open'
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(json.dumps(result, indent=2)+'\n')
    print(f"Verified {result['body_checks']} containing bodies, {result['instruction_checks']} instructions, "
          f"{result['constant_checks']} constants, {result['data_checks']} spans, "
          f"{result['import_checks']} imports and {sum(len(t['sites']) for t in result['direct_reference_sites'])} direct builder sites,seven derived addresses and one original node resource example")


if __name__ == '__main__':
    main()
