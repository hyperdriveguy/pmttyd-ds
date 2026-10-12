#!/usr/bin/env python3
"""Read-only US DOL references and shipped REL imports; not runtime reachability.

Outputs metadata only. Symbol names are external annotations. No-reference is
not proof of unused code: indirect/computed calls and unrecognized symbols remain.
"""
import argparse
from collections import Counter, defaultdict
import csv
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO, inventory
from behavior_evidence import FUNCTIONS


def audit(iso_path, symbols_path):
    info = inventory(iso_path)
    if info['disc_id'] != 'G8ME01':
        raise ValueError('US revision 0 required')
    symbol_bytes = Path(symbols_path).read_bytes()
    rows = list(csv.DictReader(symbol_bytes.decode().splitlines()))
    symbols = {int(r['ram_addr'], 16): r for r in rows
               if r['area'] == '_main' and r['sec_type'] == 'text'
               and r['ram_addr'] and int(r['size'], 16) > 0}
    branches, pointers, imports = defaultdict(list), Counter(), defaultdict(list)
    with Path(iso_path).open('rb') as disc:
        def read(offset, size):
            if offset < 0 or size < 0 or offset + size > info['disc_size']:
                raise ValueError('Outside disc')
            disc.seek(offset)
            data = disc.read(size)
            if len(data) != size:
                raise ValueError('Truncated disc')
            return data
        revision = read(7, 1)[0]
        if revision != 0:
            raise ValueError('US revision 0 required')
        dol = struct.unpack('>I', read(0x420, 4))[0]
        header = read(dol, 256)
        dol_end = 256
        action_table = None
        for i in range(18):
            off, addr, size = (struct.unpack_from('>I', header, b + i * 4)[0]
                               for b in (0, 0x48, 0x90))
            if not size:
                continue
            dol_end = max(dol_end, off + size)
            data = read(dol + off, size)
            if addr <= 0x80364348 and 0x80364348 + 0x1a4 <= addr + size:
                action_table = data[0x80364348 - addr:0x80364348 - addr + 0x1a4]
            for j in range(0, len(data) - 3, 4):
                word = struct.unpack_from('>I', data, j)[0]
                if i < 7 and word >> 26 == 18:
                    delta = word & 0x03fffffc
                    if delta & 0x02000000:
                        delta -= 0x04000000
                    target = (delta if word & 2 else addr + j + delta) & 0xffffffff
                    if target in symbols:
                        branches[target].append({'site': addr + j, 'link': bool(word & 1)})
                elif i >= 7 and word in symbols:
                    pointers[word] += 1  # Candidate pointers; may be integer coincidences.
        dol_sha256 = hashlib.sha256(read(dol, dol_end)).hexdigest()
        for entry in info['files']:
            if not entry['path'].endswith('.rel'):
                continue
            raw = read(entry['offset'], entry['size'])
            def u32(o):
                return struct.unpack_from('>I', raw, o)[0]
            imp, length = u32(0x28), u32(0x2c)
            if length % 8 or imp + length > len(raw):
                raise ValueError('Invalid REL import table')
            for j in range(imp, imp + length, 8):
                module, stream = struct.unpack_from('>II', raw, j)
                if module != 0:
                    continue
                section, offset = None, 0
                for _ in range(len(raw) // 8):
                    delta, kind, target_section, addend = struct.unpack_from('>HBBI', raw, stream)
                    stream += 8
                    if kind == 203:
                        break
                    if kind == 202:
                        section, offset = target_section, 0
                        continue
                    offset += delta
                    if kind not in (0, 201) and addend in symbols:
                        imports[addend].append({'module': entry['path'], 'section': section,
                                               'offset': offset, 'relocation_type': kind})
                else:
                    raise ValueError('Unterminated REL relocation stream')
    if action_table is None:
        raise ValueError('Action-command table not mapped')
    action_entries = []
    for offset in range(0, len(action_table), 20):
        command_id = struct.unpack_from('>I', action_table, offset)[0]
        if command_id == 0:
            break
        words = struct.unpack_from('>5I', action_table, offset)
        action_entries.append({'id': command_id, 'table_address': 0x80364348 + offset,
                               'callbacks': {role: {'address': address,
                                                   'annotation': symbols.get(address, {}).get('name')}
                                             for role, address in zip(('main', 'result', 'display', 'delete'), words[1:])}})
    else:
        raise ValueError('No action-command sentinel')
    documented = set(FUNCTIONS)
    records = []
    for address, row in sorted(symbols.items()):
        namespace_name = row['name'] + '@' + row['namespace']
        records.append({'name': row['name'], 'namespace': row['namespace'],
                        'address': address, 'size': int(row['size'], 16),
                        'fingerprinted': row['name'] in documented or namespace_name in documented,
                        'direct_link_sites': sum(b['link'] for b in branches[address]),
                        'direct_nonlink_sites': sum(not b['link'] for b in branches[address]),
                        'data_word_candidates': pointers[address],
                        'rel_imports': imports[address]})
    return {'schema': 1, 'disc_id': info['disc_id'], 'revision': revision,
            'dol_sha256': dol_sha256,
            'action_command_table': action_entries,
            'symbols_sha256': hashlib.sha256(symbol_bytes).hexdigest(),
            'limitations': 'Static references only; no branch feasibility, execution frequency, '
                            'indirect/computed reachability, or complete semantic coverage claim. '
                            'REL imports include code/data references, not necessarily event callbacks.',
            'summary': {'named_dol_text_entries': len(records),
                        'fingerprinted_entries': sum(r['fingerprinted'] for r in records),
                        'with_direct_link_reference': sum(bool(r['direct_link_sites']) for r in records),
                        'with_data_word_candidate': sum(bool(r['data_word_candidates']) for r in records),
                        'with_shipped_rel_import': sum(bool(r['rel_imports']) for r in records),
                        'with_any_static_reference': sum(bool(r['direct_link_sites'] or r['direct_nonlink_sites']
                                                            or r['data_word_candidates'] or r['rel_imports'])
                                                         for r in records)}, 'functions': records}


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--iso', default=DEFAULT_ISO)
    p.add_argument('--symbols', required=True)
    p.add_argument('--output', required=True)
    a = p.parse_args()
    output = Path(a.output).resolve()
    if output in (Path(a.iso).resolve(), Path(a.symbols).resolve()):
        raise ValueError('Cannot overwrite input')
    report = audit(a.iso, a.symbols)
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report['summary'], indent=2))


if __name__ == '__main__':
    main()
