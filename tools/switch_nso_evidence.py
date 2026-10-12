#!/usr/bin/env python3
"""Fingerprint reviewed Switch main NSO ranges; optionally wrap segments as ELF.

Input must be an uncompressed NSO produced locally by hactool. The ELF is an
inspection artifact, not a runnable port. Addresses are module-relative.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct

BUILD_ID = '78f37bb55d015be3b368ec22af595455f1544dc1000000000000000000000000'
RANGES = (
    ('defence_parameter_query', 0x779ff0, 0x77a180),
    ('counter_parameter_query', 0x77a180, 0x77a310),
    ('defence_update_prefix_partial', 0x7225c4, 0x722930),
)
ANCHORS = {
    0x77a044: 0xb940092a,  # first table field +8
    0x77a070: 0xb940c929,  # seventh table field +c8
    0x77a0b4: 0x4b090108,  # subtract unit modifiers
    0x77a0bc: 0x11000d08,  # add 3
    0x77a0c0: 0x0aa87d08,  # lower clamp
    0x77a0c8: 0x1a89b108,  # upper clamp
    0x77a16c: 0x1e220280,  # signed integer -> float result
    0x77a23c: 0xb9568d48,  # same modifier field in counter query
    0x77a2fc: 0x1e220280,
    0x722820: 0x1e213800,  # runtime decrement, not an immediate 1
    0x722848: 0x1e223821,
}


def inspect(path):
    raw = Path(path).read_bytes()
    if len(raw) < 256 or raw[:4] != b'NSO0':
        raise ValueError('Expected NSO0 header')
    if struct.unpack_from('<I', raw, 12)[0] & 7:
        raise ValueError('Decompress NSO first')
    if raw[0x40:0x60].hex() != BUILD_ID:
        raise ValueError('Unreviewed build ID')
    segments = []
    for name, offset in zip(('.text', '.rodata', '.data'), (0x10, 0x20, 0x30)):
        file_offset, address, size = struct.unpack_from('<III', raw, offset)
        if file_offset < 256 or file_offset + size > len(raw):
            raise ValueError('Segment outside input')
        segments.append(dict(name=name, file_offset=file_offset, address=address, size=size))

    def at(address, size):
        matches = [s for s in segments if s['address'] <= address
                   and address + size <= s['address'] + s['size']]
        if len(matches) != 1:
            raise ValueError('Unmapped/ambiguous address')
        seg = matches[0]
        offset = seg['file_offset'] + address - seg['address']
        return raw[offset:offset+size]

    for address, expected in ANCHORS.items():
        if struct.unpack('<I', at(address, 4))[0] != expected:
            raise ValueError(f'Reviewed instruction differs at {address:#x}')
    ranges = [dict(label=label, address=start, size=end-start,
                   sha256=hashlib.sha256(at(start, end-start)).hexdigest())
              for label, start, end in RANGES]
    report = dict(build_id=BUILD_ID, nso_sha256=hashlib.sha256(raw).hexdigest(),
                  segments=segments, ranges=ranges, verified_anchor_count=len(ANCHORS))
    return raw, report


def elf_wrapper(raw, segments):
    result = bytearray(64)
    headers = [bytes(64)]
    names = b'\0.text\0.rodata\0.data\0.shstrtab\0'
    for seg, name_offset, flags in zip(segments, (1, 7, 15), (6, 2, 3)):
        offset = len(result)
        result += raw[seg['file_offset']:seg['file_offset']+seg['size']]
        headers.append(struct.pack('<IIQQQQIIQQ', name_offset, 1, flags,
                                   seg['address'], offset, seg['size'], 0, 0, 4, 0))
    offset = len(result)
    result += names
    headers.append(struct.pack('<IIQQQQIIQQ', 21, 3, 0, 0, offset, len(names), 0, 0, 1, 0))
    section_headers = len(result)
    result += b''.join(headers)
    result[:64] = struct.pack('<16sHHIQQQIHHHHHH', b'\x7fELF\x02\x01\x01'+bytes(9),
                             2, 183, 1, 0, 0, section_headers, 0, 64, 0, 0, 64, 5, 4)
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--nso', type=Path, default=Path('local-assets/switch/decoded/main.nso'))
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--elf', type=Path)
    args = parser.parse_args()
    paths = [args.nso, args.output] + ([args.elf] if args.elf else [])
    if len({p.resolve() for p in paths}) != len(paths):
        parser.error('Input/report/ELF paths must differ')
    for i, a in enumerate(paths):
        for b in paths[i+1:]:
            if a.exists() and b.exists() and a.samefile(b):
                parser.error('Input/report/ELF files must differ')
    raw, report = inspect(args.nso)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2)+'\n')
    if args.elf:
        args.elf.parent.mkdir(parents=True, exist_ok=True)
        args.elf.write_bytes(elf_wrapper(raw, report['segments']))
    print(f"Verified {len(ANCHORS)} instruction anchors; {len(RANGES)} range fingerprints")


if __name__ == '__main__':
    main()
