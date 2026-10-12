#!/usr/bin/env python3
"""Verify scoped US projected texture/shadow TEV blocks and source leaves."""
import argparse
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO
from verify_field_lecture_callbacks import verify
from verify_animation_render_material import verify_census

MANIFEST = Path(__file__).with_name('map_tev_projected_evidence.json')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    parser.add_argument('--output',type=Path)
    args = parser.parse_args()
    if args.output and args.output.resolve() in (Path(args.iso).resolve(),MANIFEST.resolve()):
        parser.error('Cannot overwrite ISO or pinned manifest')
    result = verify(args.iso,MANIFEST)
    expected = json.loads(MANIFEST.read_text())
    result['direct_reference_sites'] = verify_census(args.iso,expected)
    result['reviewed_ranges'] = expected['reviewed_ranges']
    result['binary32_constants'] = [dict(address=c['address'],word=c['hex'],
                                       value=struct.unpack('>f',bytes.fromhex(c['hex']))[0])
                                    for c in expected['constants']]
    result['scope'] = 'Containing TEV identity with complete flags400/20000 blocks/four projected-shadow leaves;four remaining middle branches/GX/source/lifetime/runtime open;no original execution'
    if args.output:
        args.output.parent.mkdir(parents=True,exist_ok=True)
        args.output.write_text(json.dumps(result,indent=2)+'\n')
    print(f"Verified {result['instruction_checks']} scoped words, {result['constant_checks']} constants, "
          f"{result['data_checks']} original spans, {result['import_checks']} imports and six direct TEV sites")


if __name__ == '__main__':
    main()
