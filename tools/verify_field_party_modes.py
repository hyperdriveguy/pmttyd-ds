#!/usr/bin/env python3
"""Verify US field mode/driver bodies and a scoped direct control-index store census."""
import argparse
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO
from verify_field_lecture_callbacks import verify

MANIFEST = Path(__file__).with_name('field_party_mode_evidence.json')


def census(iso):
    stores = []
    sections = []
    with Path(iso).open('rb') as disc:
        disc.seek(0x420)
        base = struct.unpack('>I', disc.read(4))[0]
        disc.seek(base)
        header = disc.read(256)
        bss_address, bss_size = struct.unpack_from('>2I', header, 0xd8)
        if not bss_address <= 0x8041ead0 or 0x8041ead4 > bss_address+bss_size:
            raise ValueError('Control-index word outside original BSS')
        for i in range(7):
            off, va, size = (struct.unpack_from('>I', header, j+i*4)[0] for j in (0, 0x48, 0x90))
            if not size:
                continue
            disc.seek(base+off)
            data = disc.read(size)
            if len(data) != size or size % 4:
                raise ValueError('Truncated or unaligned DOL text')
            sections.append(dict(address=va, size=size))
            for n in range(0, size, 4):
                word = struct.unpack_from('>I', data, n)[0]
                width = {36:4,37:4,38:1,39:1,44:2,45:2}.get(word>>26)
                displacement = struct.unpack('>h', data[n+2:n+4])[0]
                if width and (word>>16)&31 == 13 and displacement <= 0x1bb3 and displacement+width > 0x1bb0:
                    stores.append(dict(address=va+n, instruction=f'{word:08x}'))
    return dict(scope='DOL text D-form integer byte/half/word stores based directly on r13 overlapping control-index word; excludes aliases,indexed/floating stores,RELs and bulk clearing',
                text_sections=sections, stores=stores,
                control_index_address=0x8041ead0,
                bss_address=bss_address, bss_size=bss_size)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    if args.output and args.output.resolve() in (Path(args.iso).resolve(), MANIFEST.resolve()):
        parser.error('Cannot overwrite ISO or pinned manifest')
    result = verify(args.iso, MANIFEST)
    result['scope'] = 'Static US party mode initialization and driver/index census; no original execution'
    result['control_index_census'] = census(args.iso)
    expected = json.loads(MANIFEST.read_text())
    if result['control_index_census']['stores'] != expected['direct_control_index_stores']:
        raise ValueError('Direct control-index writer census mismatch')
    if [result['control_index_census']['bss_address'], result['control_index_census']['bss_size']] != expected['bss_span']:
        raise ValueError('Original BSS header span mismatch')
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(json.dumps(result, indent=2)+'\n')
    print(f"Verified {result['body_checks']} bodies,{result['instruction_checks']} anchors,"
          f"{result['constant_checks']} constants,{result['data_checks']} data spans,"
          f"{result['import_checks']} field imports and scoped direct-index census")


if __name__ == '__main__':
    main()
