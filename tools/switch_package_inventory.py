#!/usr/bin/env python3
"""Read-only PFS0 inventory and hashes; never emits ticket/key contents.

Container names are metadata, not proof of title/version or NCA authenticity.
No decryption, extraction, or emulator profile access is performed here.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct

DEFAULT_NSP = Path('/home/carson/games/roms/Paper Mario The Thousand-Year Door '
                   '[BASE][0100ECD018EBE000][US][v0].nsp')


def inventory(path):
    path = Path(path)
    with path.open('rb') as source:
        size = source.seek(0, 2)
        source.seek(0)
        header = source.read(16)
        if len(header) != 16:
            raise ValueError('Truncated PFS0 header')
        magic, count, strings_size, reserved = struct.unpack('<4sIII', header)
        start = 16 + count * 24 + strings_size
        if magic != b'PFS0' or reserved or count > 100000 or start > size:
            raise ValueError('Invalid PFS0 header/count/bounds')
        if strings_size > 16 * 1024 * 1024:
            raise ValueError('Unreasonable string table')
        records = source.read(count * 24)
        strings = source.read(strings_size)
        entries, names, ranges = [], set(), []
        for index in range(count):
            offset, length, name_offset, reserved = struct.unpack_from(
                '<QQII', records, index * 24)
            end = strings.find(b'\0', name_offset)
            if reserved or name_offset >= strings_size or end < 0:
                raise ValueError('Invalid entry/name bounds')
            name = strings[name_offset:end].decode('utf-8')
            if not name or name in names or '/' in name or '\\' in name or name in ('.', '..'):
                raise ValueError('Unsafe or duplicate container name')
            absolute = start + offset
            if absolute + length > size:
                raise ValueError('Entry outside container')
            names.add(name)
            ranges.append((absolute, absolute + length))
            entries.append(dict(name=name, offset=absolute, size=length))
        previous_end = start
        for begin, end in sorted(ranges):
            if begin < previous_end:
                raise ValueError('Overlapping entries')
            previous_end = end
        # Hash the container and each member in a single ordered pass, gaps included.
        whole = hashlib.sha256()
        position = 0
        source.seek(0)

        def consume(length, member=None):
            nonlocal position
            while length:
                block = source.read(min(length, 4 * 1024 * 1024))
                if not block:
                    raise ValueError('Container changed/truncated during hashing')
                whole.update(block)
                if member is not None:
                    member.update(block)
                position += len(block)
                length -= len(block)

        for entry in sorted(entries, key=lambda e: e['offset']):
            consume(entry['offset'] - position)
            digest = hashlib.sha256()
            consume(entry['size'], digest)
            entry['sha256'] = digest.hexdigest()
        consume(size - position)
        if source.read(1):
            raise ValueError('Container grew during hashing')
    return dict(format='PFS0', source=str(path.resolve()), size=size,
                sha256=whole.hexdigest(), data_offset=start, entries=entries)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--nsp', type=Path, default=DEFAULT_NSP)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    if args.output.resolve() == args.nsp.resolve() or (
            args.output.exists() and args.output.samefile(args.nsp)):
        parser.error('Output must not overwrite source')
    result = inventory(args.nsp)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=2) + '\n')
    print(f"Inventoried {len(result['entries'])} entries; SHA256 {result['sha256']}")


if __name__ == '__main__':
    main()
