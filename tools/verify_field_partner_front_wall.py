#!/usr/bin/env python3
"""Verify static US partner front wall and filter services evidence."""
import argparse
import json
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_field_lecture_callbacks import verify

MANIFEST = Path(__file__).with_name('field_partner_front_wall_evidence.json')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    if args.output and args.output.resolve() in (Path(args.iso).resolve(), MANIFEST.resolve()):
        parser.error('Cannot overwrite original ISO or pinned manifest')
    result = verify(args.iso, MANIFEST)
    result['scope'] = 'Static US partner front wall/filter local bodies and backend result-layout only; no original execution or full ray backend closure'
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(json.dumps(result, indent=2)+'\n')
    print(f"Verified {result['body_checks']} DOL bodies, {result['instruction_checks']} instructions, "
          f"{result['constant_checks']} constants, {result['data_checks']} data spans "
          f"and {result['import_checks']} field bindings")


if __name__ == '__main__':
    main()
