#!/usr/bin/env python3
"""Verify US animation Main evaluation and initialization bodies.

Static evidence only;no original game instruction is executed.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO, inventory
from verify_field_lecture_callbacks import verify

MANIFEST = Path(__file__).with_name('animation_main_evidence.json')


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
            raise ValueError('Animation Main direct-reference census mismatch')
    return census['targets']


def verify_resource_example(iso, expected):
    """Check serialized resource identity/offsets;does not emulate loaded poses."""
    spec = expected['resource_example']
    entry = next(e for e in inventory(iso)['files'] if e['path'] == spec['path'])
    with Path(iso).open('rb') as disc:
        disc.seek(entry['offset'])
        raw = disc.read(entry['size'])
    if len(raw) != spec['size'] or hashlib.sha256(raw).hexdigest() != spec['sha256']:
        raise ValueError('Original animation resource mismatch')
    def word(offset):
        return struct.unpack_from('>I', raw, offset)[0]
    def scalar(offset):
        return struct.unpack_from('>f', raw, offset)[0]
    names = word(0x1ac)
    matches = [i for i in range(word(0x148))
               if raw[names+64*i:names+64*i+60].split(b'\0')[0]
               == spec['animation_name'].encode('ascii')]
    if matches != [spec['animation_index']]:
        raise ValueError('Original animation name/index mismatch')
    offset = word(names+64*matches[0]+60)
    control = offset+word(offset+0x24)
    keys = offset+word(offset+0x28)
    found = dict(base_counts=[word(o) for o in (0xf0,0xf8,0x13c,0x140,0x12c)],
                 animation_offset=offset,
                 control=[word(control),scalar(control+4),scalar(control+8)],
                 keys=[dict(time=scalar(keys+44*k),
                            counts=[word(keys+44*k+o) for o in (8,16,24,32,40)])
                       for k in range(word(offset+8))])
    if any(found[k] != spec[k] for k in found):
        raise ValueError('Original animation resource layout mismatch')
    return dict(path=spec['path'],sha256=spec['sha256'],**found)


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
    result['scope'] = 'Static complete animation Main/init local bodies;resource/draw/clock/helper/SDK/source/runtime domains open'
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(json.dumps(result, indent=2)+'\n')
    print(f"Verified {result['body_checks']} containing bodies, {result['instruction_checks']} instructions, "
          f"{result['constant_checks']} constants, {result['data_checks']} spans, "
          f"{result['import_checks']} imports and {sum(len(t['sites']) for t in result['direct_reference_sites'])} direct caller sites and one serialized resource example")


if __name__ == '__main__':
    main()
