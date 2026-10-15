#!/usr/bin/env python3
"""Losslessly export original embedded recovery data; no effect admission implied.

JSON is editable source; PMREC001 keeps original addresses and big-endian bytes.
The source ISO is opened read-only. Neither this pack nor JSON contains textures,
Maya evaluation, effect scheduling or sound playback implementations.
"""
import argparse
import hashlib
import json
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO

MAGIC = b'PMREC001'
# Complete source data blocks, not executable function bodies. Gaps/padding in
# these bounded blocks are retained so address aliases never get relocated.
SPANS = (
    ('signed_conversion_bias', 0x802c1f18, 8, 'u32'),
    ('recovery_vectors_and_sfx_names', 0x802f3b20, 0x170, 'u8'),
    ('recovery_maya_keys', 0x80371fa8, 0x6f4, 'u16'),
    ('recovery_dispatch_tables', 0x8037269c, 0x88, 'u32'),
    ('recovery_colors', 0x804179d8, 0x24, 'u8'),
    ('recovery_maya_face', 0x804179fc, 0x2a, 'u16'),
    ('recovery_maya_descriptor_graph', 0x80417a28, 0x28, 'u32'),
    ('maya_lifecycle_constants', 0x8042019c, 0x1c, 'u32'),
    ('recovery_constants', 0x80422db0, 0x9c, 'u32'),
)
LICENSE = 'Nintendo/Intelligent Systems original US assets; local use, no redistribution rights'


def read_original(iso):
    """Read the requested DOL spans, rejecting ambiguous/truncated mappings."""
    with Path(iso).open('rb') as disc:
        if disc.read(8) != b'G8ME01\0\0':
            raise ValueError('US G8ME01 disc0 revision0 required')
        disc.seek(0x420)
        raw = disc.read(4)
        if len(raw) != 4:
            raise ValueError('Truncated disc header')
        dol, = struct.unpack('>I', raw)
        disc.seek(dol)
        header = disc.read(256)
        if len(header) != 256:
            raise ValueError('Truncated DOL header')
        sections = [tuple(struct.unpack_from('>I', header, p + 4*i)[0]
                          for p in (0, 0x48, 0x90)) for i in range(18)]
        rows = []
        for name, address, size, encoding in SPANS:
            matches = [offset + address - start for offset, start, length in sections
                       if offset and start <= address and address + size <= start + length]
            if len(matches) != 1:
                raise ValueError('Unmapped/ambiguous source span: ' + name)
            disc.seek(dol + matches[0])
            data = disc.read(size)
            if len(data) != size:
                raise ValueError('Truncated source span: ' + name)
            code = {'u8': 'B', 'u16': 'H', 'u32': 'I'}[encoding]
            values = list(struct.unpack('>' + code * (size // struct.calcsize(code)), data))
            rows.append(dict(name=name, address=f'{address:08x}', encoding=encoding,
                             values=values, original_sha256=hashlib.sha256(data).hexdigest()))
    return dict(format='PMREC001', source='US G8ME01 revision0 main.dol embedded data',
                license=LICENSE, spans=rows)


def encode(source):
    if source.get('format') != 'PMREC001' or not source.get('license'):
        raise ValueError('Recovery source schema/license required')
    rows = source.get('spans', [])
    if not isinstance(rows, list) or not 1 <= len(rows) <= 64:
        raise ValueError('Invalid source span count')
    chunks = []
    for row in rows:
        address = int(row['address'], 16)
        encoding = row['encoding']
        if encoding not in ('u8', 'u16', 'u32'):
            raise ValueError('Invalid integer encoding')
        code = {'u8': 'B', 'u16': 'H', 'u32': 'I'}[encoding]
        values = row['values']
        if not isinstance(values, list) or any(type(x) is not int for x in values):
            raise ValueError('Integer source values required')
        try:
            data = struct.pack('>' + code * len(values), *values)
        except struct.error as error:
            raise ValueError('Source value out of range') from error
        if not data or len(data) > 65536 or not 0 <= address < address + len(data) <= 0x100000000:
            raise ValueError('Invalid source span extent')
        chunks.append((address, data))
    chunks.sort()
    if any(a + len(b) > c for (a, b), (c, _) in zip(chunks, chunks[1:])):
        raise ValueError('Overlapping source spans')
    if sum(len(b) for _, b in chunks) > 65536:
        raise ValueError('Recovery source too large')
    return MAGIC + struct.pack('<I', len(chunks)) + b''.join(
        struct.pack('<II', a, len(b)) for a, b in chunks) + b''.join(b for _, b in chunks)


def export(iso, destination):
    destination = Path(destination)
    # Never overwrite an editable source or the input disc through aliases.
    if destination.exists() and (not destination.is_dir() or any(destination.iterdir())):
        raise ValueError('Import destination must be empty')
    source = read_original(iso)
    packed = encode(source)
    destination.mkdir(parents=True, exist_ok=True)
    (destination / 'recovery-effect.json').write_text(json.dumps(source, indent=2) + '\n')
    (destination / 'recovery-effect.bin').write_bytes(packed)
    return source


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest='command', required=True)
    importer = sub.add_parser('import')
    importer.add_argument('--iso', default=DEFAULT_ISO)
    importer.add_argument('--destination', required=True)
    compiler = sub.add_parser('compile')
    compiler.add_argument('source')
    compiler.add_argument('--output', required=True)
    args = parser.parse_args()
    if args.command == 'import':
        export(args.iso, args.destination)
    else:
        source, output = Path(args.source), Path(args.output)
        if output.resolve() == Path(DEFAULT_ISO).resolve():
            raise ValueError('Cannot overwrite original ISO')
        if source.resolve() == output.resolve():
            raise ValueError('Compiled output must differ from editable source')
        data = encode(json.loads(source.read_text()))
        output.parent.mkdir(parents=True, exist_ok=True)
        output.write_bytes(data)


if __name__ == '__main__':
    main()
