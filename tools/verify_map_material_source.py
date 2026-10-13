#!/usr/bin/env python3
"""Verify complete local US map material/light/texture-matrix/source setter bodies."""
import argparse
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO
from verify_field_lecture_callbacks import verify
from verify_animation_render_material import verify_census

MANIFEST = Path(__file__).with_name('map_material_source_evidence.json')


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
    result['binary32_constants'] = [dict(address=c['address'],word=c['hex'],
                                       value=struct.unpack('>f',bytes.fromhex(c['hex']))[0])
                                    for c in expected['constants']]
    result['scope'] = 'Complete six local map material/light/texture-matrix/source setter bodies;loader/source/GX/numeric/lifetime/runtime remain open;no original execution'
    if args.output:
        args.output.parent.mkdir(parents=True,exist_ok=True)
        args.output.write_text(json.dumps(result,indent=2)+'\n')
    print(f"Verified {result['instruction_checks']} scoped words, {result['constant_checks']} constants, "
          f"{result['data_checks']} original spans, {result['import_checks']} imports and direct map material/source sites")


if __name__ == '__main__':
    main()
