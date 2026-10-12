#!/usr/bin/env python3
"""Inventory annotated US DOL event streams without executing them.

Reports raw operands and symbol annotations, never dialogue contents. Event
conditions, variable resolution and callback effects require separate analysis.
"""
import argparse
import csv
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO


def parse_commands(data, address, names):
    commands = []
    offset = 0
    while offset < len(data):
        if offset + 4 > len(data):
            raise ValueError('Truncated command word')
        word = struct.unpack_from('>I', data, offset)[0]
        count, opcode = word >> 16, word & 65535
        if count > 255 or opcode > 118 or offset + 4 + count * 4 > len(data):
            raise ValueError(f'Invalid/truncated annotated event at {address + offset:#x}')
        operands = struct.unpack_from(f'>{count}I', data, offset + 4)
        commands.append(dict(address=address + offset, opcode=opcode,
                             operands=[dict(raw=value, annotation=names.get(value)) for value in operands]))
        offset += 4 + count * 4
    return commands


def inventory_events(iso, symbols, selections):
    rows = list(csv.DictReader(Path(symbols).open()))
    names = {int(r['ram_addr'], 16): r['name'] for r in rows if r['area'] == '_main' and r['ram_addr']}
    selected = []
    for selection in selections:
        name, _, namespace = selection.partition('@')
        matches = [r for r in rows if r['area'] == '_main' and r['type'] == 'evt' and r['name'] == name
                   and (not namespace or r['namespace'] == namespace)]
        if len(matches) != 1:
            raise ValueError(f'Choose one annotated DOL event: {selection}')
        selected.append(matches[0])
    events = []
    with Path(iso).open('rb') as disc:
        if disc.read(8) != b'G8ME01\0\0':
            raise ValueError('US G8ME01 disc0 revision0 required')
        disc.seek(0x420)
        base = struct.unpack('>I', disc.read(4))[0]
        disc.seek(base)
        header = disc.read(256)
        for row in selected:
            address, size = int(row['ram_addr'], 16), int(row['size'], 16)
            if not 0 < size < 1024 * 1024:
                raise ValueError('Invalid event size')
            offsets = []
            for i in range(18):
                off, va, length = (struct.unpack_from('>I', header, b + i * 4)[0] for b in (0, 0x48, 0x90))
                if va <= address and address + size <= va + length:
                    offsets.append(off + address - va)
            if offsets != [int(row['file_addr'], 16)]:
                raise ValueError('Symbol file/VA mapping disagrees with DOL')
            disc.seek(base + offsets[0])
            data = disc.read(size)
            if len(data) != size:
                raise ValueError('Truncated event data')
            events.append(dict(name=row['name'], address=address, size=size,
                               sha256=hashlib.sha256(data).hexdigest(),
                               commands=parse_commands(data, address, names)))
    return dict(schema=1, limitation='Static event inventory only; no branch feasibility, '
                'variable evaluation, story attribution or runtime execution claim', events=events)


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('names', nargs='+')
    p.add_argument('--iso', default=DEFAULT_ISO)
    p.add_argument('--symbols', required=True)
    p.add_argument('--output', required=True)
    a = p.parse_args()
    output = Path(a.output)
    if output.resolve() in (Path(a.iso).resolve(), Path(a.symbols).resolve()):
        raise ValueError('Cannot overwrite input')
    report = inventory_events(a.iso, a.symbols, a.names)
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(report, indent=2) + '\n')
    for e in report['events']:
        print(f"{e['name']}: {len(e['commands'])} commands, SHA256 {e['sha256']}")


if __name__ == '__main__':
    main()
