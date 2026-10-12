#!/usr/bin/env python3
"""Verify US partner slit/roll gates,paper wrappers,slit-floor and exit leaves.

Static evidence only;no original game instruction is executed.
"""
import argparse
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO
from verify_field_lecture_callbacks import verify

MANIFEST = Path(__file__).with_name('field_partner_slit_roll_evidence.json')


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
            raise ValueError('Partner slit/roll direct-reference census mismatch')
    return census['targets']


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    if args.output and args.output.resolve() in (Path(args.iso).resolve(), MANIFEST.resolve()):
        parser.error('Cannot overwrite original ISO or pinned manifest')
    result = verify(args.iso, MANIFEST)
    result['direct_reference_sites'] = verify_census(args.iso, json.loads(MANIFEST.read_text()))
    result['scope'] = 'Static complete partner slit/roll,paper/slit-floor/exit local bodies;animation/collision/source/numeric/runtime domains open'
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(json.dumps(result, indent=2)+'\n')
    print(f"Verified {result['body_checks']} containing bodies, {result['instruction_checks']} instructions, "
          f"{result['constant_checks']} constants, {result['data_checks']} spans, "
          f"{result['import_checks']} imports and {sum(len(t['sites']) for t in result['direct_reference_sites'])} direct caller sites")


if __name__ == '__main__':
    main()
