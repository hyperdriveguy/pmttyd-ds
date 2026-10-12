#!/usr/bin/env python3
"""Cross-reference `artifacts/reference` function names against a decomp snapshot.

Reports which reference-binary functions have a recovered C definition in a
public decompilation snapshot and which do not. Name matching is static text
evidence only: a matching definition name is not a proof of US-equivalent
behavior, and an absent definition is not a proof of unrecovered semantics.

The snapshot is the pinned upstream revision under
`artifacts/reference/decomp-<sha>/`; keep reports outside source history.
"""
import argparse
import csv
import json
import re
from pathlib import Path

DEFINITION = re.compile(
    r'^(?:[A-Za-z_]\w*(?:\s+(?:const|static|inline|unsigned|signed|struct|'
    r'enum|union|volatile|s32|u32|s16|u16|s8|u8|f32|u64|s64|void|char|short|'
    r'int|long|float|double|bool|size_t|Vec|MarioWork|BattleUnit))*'
    r'\s*\**\s*)(\w+)\s*\(')
MACRO_FUNC = re.compile(r'\b(?:USER_FUNC|EVT_DEFINE_USER_FUNC)\s*\(\s*(\w+)\s*\)')


def reference_names(root: Path):
    """Collect reference function names and their listing entry addresses."""
    names = {}
    for disasm in sorted(root.rglob('*.disasm')):
        if disasm.stat().st_size == 0:
            continue
        name = disasm.stem
        try:
            head = disasm.read_text(errors='replace').splitlines()[:8]
        except OSError:
            continue
        entry = None
        for line in head:
            match = re.match(r'^([0-9a-f]{8}) <\.text>:', line)
            if match:
                entry = match.group(1)
                break
        record = names.setdefault(name, {'entry': entry, 'paths': []})
        record['paths'].append(str(disasm.relative_to(root)))
        if entry and not record['entry']:
            record['entry'] = entry
    return names


def snapshot_definitions(root: Path):
    """Collect function definition names and their source paths."""
    definitions = {}
    for source in sorted(root.rglob('*.c')):
        names = set()
        for line in source.read_text(errors='replace').splitlines():
            match = DEFINITION.match(line)
            if match and match.group(1) not in ('if', 'for', 'while', 'switch',
                                                'return', 'sizeof', 'else'):
                names.add(match.group(1))
        for name in names:
            definitions.setdefault(name, set()).add(str(source.relative_to(root)))
    for source in sorted(root.rglob('*.h')):
        text = source.read_text(errors='replace')
        for match in MACRO_FUNC.finditer(text):
            definitions.setdefault(match.group(1), set()).add(
                str(source.relative_to(root)))
    return definitions


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--reference', default='artifacts/reference')
    parser.add_argument('--snapshot', required=True)
    parser.add_argument('--symbols', default='artifacts/reference/us_symbols.csv',
                        help='US DOL symbol table used to resolve canonical names')
    parser.add_argument('--output', required=True)
    args = parser.parse_args()
    reference = Path(args.reference).resolve()
    snapshot = Path(args.snapshot).resolve()
    names = reference_names(reference)
    definitions = snapshot_definitions(snapshot)
    symbols = {}
    with Path(args.symbols).open() as handle:
        for row in csv.DictReader(handle):
            if row.get('sec_type') == 'text' and row.get('ram_addr'):
                symbols[row['ram_addr'].lower()] = row['name']
    records = []
    for name in sorted(names):
        paths = set(definitions.get(name, ()))
        entry = names[name]['entry']
        symbol = symbols.get(entry) if entry else None
        if symbol and symbol != name:
            paths |= definitions.get(symbol, set())
        records.append(dict(
            name=name, entry=entry, symbol=symbol,
            listing=names[name]['paths'],
            decompiled=bool(paths),
            sources=sorted('snapshot:' + p for p in paths),
            evidence='listing-name' if paths and name in definitions else
                     ('canonical-symbol' if paths else 'static-name-absence')))
    missing = [r for r in records if not r['decompiled']]
    result = dict(
        schema=2,
        reference=str(reference),
        snapshot=str(snapshot),
        limitation='Static name matching only; matching names are not US behavior proof, '
                   'and an absent definition is not a proof of unrecovered semantics',
        reference_functions=len(records),
        with_snapshot_definition=len(records) - len(missing),
        without_snapshot_definition=len(missing),
        functions=records,
    )
    output = Path(args.output).resolve()
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(result, indent=1) + '\n')
    print(json.dumps({k: v for k, v in result.items() if k != 'functions'},
                     indent=1))


if __name__ == '__main__':
    main()
