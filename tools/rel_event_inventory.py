#!/usr/bin/env python3
"""Inventory annotated REL event spans and relocated operands, never execute them.

Reports remain local: strings may be original resource identifiers. A relocation
is a binding, not evidence that a condition or event executes in a story state.
"""
import argparse
import csv
import hashlib
import json
from pathlib import Path
import struct
from dol_event_inventory import parse_commands
from iso_assets import DEFAULT_ISO, inventory


def read_bindings(raw):
    if len(raw) < 0x4c:
        raise ValueError('Truncated REL header')
    def u(o):
        return struct.unpack_from('>I', raw, o)[0]
    count, table = u(0x0c), u(0x10)
    if not count or table + count*8 > len(raw):
        raise ValueError('Invalid section table')
    sections = []
    for i in range(count):
        off, size = struct.unpack_from('>II', raw, table+i*8)
        off &= ~3
        if off and off+size > len(raw):
            raise ValueError('Section outside REL')
        sections.append((off,size))
    imp, size = u(0x28), u(0x2c)
    if size % 8 or imp+size > len(raw):
        raise ValueError('Invalid import table')
    local, imports, foreign = {}, {}, {}
    for j in range(imp,imp+size,8):
        target_module, stream = struct.unpack_from('>II',raw,j)
        section, offset = None, 0
        while True:
            if stream+8 > len(raw):
                raise ValueError('Unterminated relocation stream')
            delta, kind, target, addend = struct.unpack_from('>HBBI',raw,stream)
            stream += 8
            if kind == 203:
                break
            if kind == 202:
                if target >= count:
                    raise ValueError('Invalid source section')
                section, offset = target, 0
                continue
            offset += delta
            if kind != 1:
                continue
            if section is None or offset+4 > sections[section][1]:
                raise ValueError('Relocation outside source section')
            if not sections[section][0]:
                continue  # BSS has no original file bytes to inspect.
            site = sections[section][0]+offset
            if site in local or site in imports or site in foreign:
                raise ValueError('Duplicate ADDR32 relocation site')
            if target_module == 0:
                imports[site] = addend
            elif target_module == u(0):
                if target >= count or addend > sections[target][1]:
                    raise ValueError('Invalid local target')
                off = sections[target][0]
                local[site] = dict(section=target, addend=addend,
                                   file_offset=off+addend if off and addend < sections[target][1] else None)
            else:
                foreign[site] = dict(module=target_module, section=target, addend=addend)
    return local, imports, foreign


def inventory_events(rel, symbols, module, selections):
    raw = rel if isinstance(rel, bytes) else Path(rel).read_bytes()
    local, imports, foreign = read_bindings(raw)
    with Path(symbols).open() as source:
        rows = list(csv.DictReader(source))
    dol_names = {int(r['ram_addr'], 16): r['name'] for r in rows if r['area'] == '_main' and r['ram_addr']}
    local_names = {int(r['file_addr'], 16): r['name'] for r in rows if r['area'] == module and r['file_addr']}
    events = []
    for selection in selections:
        name, _, namespace = selection.partition('@')
        matches = [r for r in rows if r['area'] == module and r['type'] == 'evt'
                   and r['name'] == name and (not namespace or r['namespace'] == namespace)]
        if len(matches) != 1:
            raise ValueError(f'Choose one annotated REL event: {selection}')
        row = matches[0]
        start, size = int(row['file_addr'], 16), int(row['size'], 16)
        if start < 0 or size <= 0 or start + size > len(raw):
            raise ValueError('Annotated event outside REL')
        commands = parse_commands(raw[start:start + size], start, {})
        for command in commands:
            for i, operand in enumerate(command['operands']):
                site = command['address'] + 4 + i * 4
                if site in imports:
                    operand['dol_address'] = imports[site]
                    operand['annotation'] = dol_names.get(imports[site])
                if site in foreign:
                    operand['foreign_import'] = foreign[site]
                if site in local:
                    binding = local[site]
                    operand['rel_section'] = binding['section']
                    operand['rel_addend'] = binding['addend']
                    pointer = binding['file_offset']
                    if pointer is None:
                        continue
                    operand['rel_offset'] = pointer
                    operand['annotation'] = local_names.get(pointer)
                    # Bounded printable resource identifier; never dereference process VAs.
                    end = raw.find(b'\0', pointer, min(pointer + 256, len(raw))) if 0 <= pointer < len(raw) else -1
                    if end > pointer:
                        value = raw[pointer:end]
                        if all(32 <= byte < 127 for byte in value):
                            operand['identifier'] = value.decode('ascii')
        events.append(dict(name=row['name'], namespace=row['namespace'], file_offset=start,
                           size=size, sha256=hashlib.sha256(raw[start:start + size]).hexdigest(),
                           commands=commands))
    return dict(schema=1, module=module, rel_size=len(raw), rel_sha256=hashlib.sha256(raw).hexdigest(),
                limitation='Static event spans and self/module0 ADDR32 bindings only. '
                'Foreign imports retained but not resolved. No condition execution, loaded addresses or complete control-flow claim.',
                events=events)


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('names', nargs='+')
    source = p.add_mutually_exclusive_group()
    source.add_argument('--rel', help='Read an existing local REL')
    source.add_argument('--iso', default=DEFAULT_ISO, help='Read rel/MODULE.rel from US ISO directly')
    p.add_argument('--module', required=True)
    p.add_argument('--symbols', required=True)
    p.add_argument('--output', required=True)
    a = p.parse_args()
    output = Path(a.output)
    input_path = a.rel or a.iso
    if output.resolve() in (Path(input_path).resolve(), Path(a.symbols).resolve()):
        raise ValueError('Cannot overwrite source')
    if a.rel:
        raw = Path(a.rel).read_bytes()
    else:
        info = inventory(a.iso)
        if info['disc_id'] != 'G8ME01':
            raise ValueError('US G8ME01 required')
        matches = [f for f in info['files'] if f['path'] == f'rel/{a.module}.rel']
        if len(matches) != 1:
            raise ValueError('Choose one shipped REL module')
        with Path(a.iso).open('rb') as disc:
            disc.seek(6)
            if disc.read(2) != b'\0\0':
                raise ValueError('US disc0 revision0 required')
            disc.seek(matches[0]['offset'])
            raw = disc.read(matches[0]['size'])
        if len(raw) != matches[0]['size']:
            raise ValueError('Truncated REL')
    report = inventory_events(raw, a.symbols, a.module, a.names)
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(report, indent=2) + '\n')
    for e in report['events']:
        print(f"{e['name']}: {len(e['commands'])} commands; {e['sha256']}")


if __name__ == '__main__':
    main()
