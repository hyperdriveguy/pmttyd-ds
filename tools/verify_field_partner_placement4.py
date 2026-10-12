#!/usr/bin/env python3
"""Verify US placement4 bytes and reconstruct copied angle-array provenance.

This is bounded instruction analysis, not execution of the original game.
"""
import argparse
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO
from verify_field_lecture_callbacks import verify

MANIFEST = Path(__file__).with_name('field_partner_placement4_evidence.json')


def decode_initializer(raw, base_register, base_address):
    """Track original word addresses through this straight-line copy block."""
    regs, frame = {}, {}
    if len(raw) % 4:
        raise ValueError('Unaligned initializer')
    for (word,) in struct.iter_unpack('>I', raw):
        op, rt, ra = word >> 26, word >> 21 & 31, word >> 16 & 31
        displacement = word & 0xffff
        if displacement & 0x8000:
            displacement -= 0x10000
        if op == 32 and ra == base_register:
            regs[rt] = base_address + displacement
        elif op == 32 and ra == 1:
            regs[rt] = frame[displacement]
        elif op == 36 and ra == 1:
            frame[displacement] = regs[rt]
        elif op == 31 and word >> 1 & 1023 == 444 and rt == (word >> 11 & 31):
            regs[ra] = regs[rt]
        else:
            raise ValueError(f'Unexpected copy-block instruction {word:#010x}')
    return frame


def verify_layout(iso, expected):
    layout = expected['initializer']
    with Path(iso).open('rb') as disc:
        disc.seek(0x420)
        base = struct.unpack('>I', disc.read(4))[0]
        disc.seek(base)
        header = disc.read(256)
        start, size = layout['start'], layout['end'] - layout['start']
        matches = []
        for i in range(18):
            off, va, length = (struct.unpack_from('>I', header, j+4*i)[0]
                               for j in (0, 0x48, 0x90))
            if va <= start and start+size <= va+length:
                matches.append(base+off+start-va)
        if len(matches) != 1:
            raise ValueError('Unmapped or ambiguous initializer')
        disc.seek(matches[0])
        raw = disc.read(size)
        if len(raw) != size:
            raise ValueError('Truncated initializer')
    frame = decode_initializer(raw, layout['base_register'], layout['base_address'])
    count = 0
    for array in layout['arrays']:
        for i in range(array['words']):
            slot = array['frame_offset'] + 4*i
            if frame.get(slot) != array['source_address'] + 4*i:
                raise ValueError(f"Angle provenance mismatch: {array['name']}[{i}]")
            count += 1
    for slot in layout['uninitialized_frame_offsets']:
        if slot in frame:
            raise ValueError('Query header unexpectedly initialized by angle copy')
    return count


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    if args.output and args.output.resolve() in (Path(args.iso).resolve(), MANIFEST.resolve()):
        parser.error('Cannot overwrite original ISO or pinned manifest')
    result = verify(args.iso, MANIFEST)
    result['angle_provenance_checks'] = verify_layout(args.iso, json.loads(MANIFEST.read_text()))
    result['scope'] = 'Static complete placement4 local graph and copied-angle layout; called backend/libm/source/runtime remain open'
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(json.dumps(result, indent=2)+'\n')
    print(f"Verified {result['body_checks']} containing bodies, {result['instruction_checks']} instructions, "
          f"{result['constant_checks']} constants, {result['data_checks']} spans, "
          f"{result['import_checks']} imports and {result['angle_provenance_checks']} angle word sources")


if __name__ == '__main__':
    main()
