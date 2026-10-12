#!/usr/bin/env python3
"""Verify static US target direction/activity, front-floor and clearance evidence."""
import argparse
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO
from verify_field_lecture_callbacks import verify

MANIFEST = Path(__file__).with_name('field_partner_live_services_evidence.json')


def verify_rise_census(iso, expected):
    sites = []
    calls = []
    with Path(iso).open('rb') as disc:
        if disc.read(8) != b'G8ME01\0\0':
            raise ValueError('Expected US disc0 revision0')
        disc.seek(0x420)
        base = struct.unpack('>I', disc.read(4))[0]
        disc.seek(base)
        header = disc.read(256)
        for i in range(7):
            off, address, size = (struct.unpack_from('>I', header, j+i*4)[0]
                                  for j in (0, 0x48, 0x90))
            disc.seek(base+off)
            raw = disc.read(size)
            if len(raw) != size:
                raise ValueError('Truncated DOL text census')
            for offset in range(0, size-3, 4):
                word = struct.unpack_from('>I', raw, offset)[0]
                opcode = word >> 26
                here = address+offset
                if opcode in (14, *range(32, 56)) and word & 0x1fffff == (13 << 16 | 0x1cf0):
                    sites.append((here, opcode))
                if opcode == 18 and word & 1:
                    delta = word & 0x3fffffc
                    if delta & 0x2000000:
                        delta -= 0x4000000
                    target = (delta if word & 2 else here+delta) & 0xffffffff
                    if target == 0x80151d20:
                        calls.append(here)
    sites.sort()
    calls.sort()
    wanted = list(zip(expected['rise_sda_sites'], expected['rise_sda_opcodes']))
    if sites != wanted or calls != expected['rise_linked_calls']:
        raise ValueError('Scoped DOL clearance direct-access/call census mismatch')
    return len(sites), len(calls)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    if args.output and args.output.resolve() in (Path(args.iso).resolve(), MANIFEST.resolve()):
        parser.error('Cannot overwrite original ISO or pinned manifest')
    expected = json.loads(MANIFEST.read_text())
    result = verify(args.iso, MANIFEST)
    accesses, calls = verify_rise_census(args.iso, expected)
    result.update(scope='Static US target/front-floor/status/clearance local bodies; backend/reinit selected regions; direct DOL census only; no original execution',
                  rise_access_checks=accesses, rise_direct_call_checks=calls)
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(json.dumps(result, indent=2)+'\n')
    print(f"Verified {result['body_checks']} DOL identities, {result['instruction_checks']} instructions, "
          f"{result['constant_checks']} constants, {result['data_checks']} spans, "
          f"{result['import_checks']} imports; census {accesses} accesses/{calls} linked calls")


if __name__ == '__main__':
    main()
