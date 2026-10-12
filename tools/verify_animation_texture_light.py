#!/usr/bin/env python3
"""Verify complete US texture/light/color/fog request bodies without executing them."""
import argparse
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO
from verify_field_lecture_callbacks import verify
from verify_animation_render_material import verify_census

MANIFEST = Path(__file__).with_name('animation_texture_light_evidence.json')


def verify_color_arithmetic(expected):
    """Exhaust the two-byte input domain of the recovered signed-multiply sequence."""
    spec = expected['color_arithmetic_domain']
    if spec != dict(byte_pair_cases=65536, divisor=255, magic=0x80808081):
        raise ValueError('Unexpected original color arithmetic domain')
    magic = spec['magic'] - (1 << 32)
    checked = 0
    for a in range(256):
        for b in range(256):
            product = a*b
            signed_high = (magic*product) >> 32
            q = (signed_high+product) >> 7
            q += (q & 0xffffffff) >> 31
            if q != product//spec['divisor']:
                raise ValueError('Original byte-product reduction differs from floor(p/255)')
            checked += 1
    return dict(cases=checked, divisor=255,
                scope='Host proof of the reviewed integer sequence on byte pairs;no original execution')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    if args.output and args.output.resolve() in (Path(args.iso).resolve(), MANIFEST.resolve()):
        parser.error('Cannot overwrite original ISO or pinned manifest')
    result = verify(args.iso, MANIFEST)
    expected = json.loads(MANIFEST.read_text())
    result['direct_reference_sites'] = verify_census(args.iso, expected)
    result['color_arithmetic'] = verify_color_arithmetic(expected)
    result['binary32_constants'] = [dict(address=c['address'], word=c['hex'],
                                       value=struct.unpack('>f', bytes.fromhex(c['hex']))[0])
                                    for c in expected['constants']]
    result['scope'] = 'Static complete texture/smart/map-light/blend/fog/query local bodies;TEV/GX/resource/lifetime/numeric/runtime domains remain open'
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(json.dumps(result, indent=2)+'\n')
    print(f"Verified {result['body_checks']} complete bodies, {result['instruction_checks']} words, "
          f"{result['constant_checks']} constants, {result['data_checks']} spans, "
          f"{result['import_checks']} imports, {sum(len(t['sites']) for t in result['direct_reference_sites'])} direct sites "
          f"and {result['color_arithmetic']['cases']} host integer cases")


if __name__ == '__main__':
    main()
