#!/usr/bin/env python3
"""Check the authored packed-entry map and its existing original-ISO evidence suites.

Map accounting is structural; binary suites support the linked local contracts.
This does not execute original code or validate a native address translator.
"""
import argparse
import importlib
import json
import re
from pathlib import Path
from iso_assets import DEFAULT_ISO

SUITES = (
    'verify_event_manager_loop',
    'verify_event_root_constructors',
    'verify_event_descendant_constructors',
    'verify_event_lifetime_services',
    'verify_event_operand_namespaces',
    'verify_event_loop_and_arithmetic',
    'verify_event_switch_cases',
    'verify_event_data_memory_opcodes',
    'verify_event_dispatch_tail',
    'verify_event_context_initialization',
)


def _behavior_contract(root,name):
    """Locate a behavior contract by basename in the subsystem folder tree."""
    direct=root/'docs/behavior'/name
    if direct.is_file():
        return direct
    matches=list((root/'docs/behavior').rglob(name))
    if len(matches)!=1:
        raise ValueError(f'Expected exactly one behavior contract named {name}, found {len(matches)}')
    return matches[0]


def verify(iso):
    root=Path(__file__).resolve().parent.parent
    layout=json.loads((root/'tools/event_entry_layout.json').read_text())
    if layout['schema']!=1 or layout['stride']!=0x1b0 or layout['endianness']!='big':
        raise ValueError('Expected original 432-byte big-endian map')
    next_offset=0
    raw_bytes=0
    names=set()
    for field in layout['fields']:
        if field['offset']!=next_offset or field['size']<=0 or field['name'] in names:
            raise ValueError(f'Gap,overlap,invalid extent or duplicate field: {field}')
        names.add(field['name'])
        next_offset+=field['size']
        if field['access']=='unknown raw bytes':
            raw_bytes+=field['size']
        if not _behavior_contract(root,field['contract']).is_file():
            raise ValueError(f'Missing evidence contract: {field["contract"]}')
    if next_offset!=layout['stride'] or raw_bytes!=44:
        raise ValueError('Entry extent/raw span accounting differs')
    document=_behavior_contract(root,'EVENT_ENTRY_PACKED_LAYOUT.md').read_text()
    rows=re.findall(r'^\| ([0-9a-f]{3}(?:\.\.[0-9a-f]{3})?) \| (\d+) \| ([^|]+) \| ([^|]+) \|',document,re.MULTILINE)
    expected=[]
    for field in layout['fields']:
        start,size=field['offset'],field['size']
        span=f'{start:03x}' if size<=4 else f'{start:03x}..{start+size-1:03x}'
        expected.append((span,str(size),field['name'],field['access']))
    if [(span,size,name.strip(),access.strip()) for span,size,name,access in rows]!=expected:
        raise ValueError('Documented map differs from machine-readable field map')
    results={name:importlib.import_module(name).verify(iso) for name in SUITES}
    return dict(schema=1,scope='Authored map structural accounting plus delivered static ISO evidence;no native ABI or execution claim',
                entry_bytes=next_offset,known_role_bytes=next_offset-raw_bytes,
                unknown_raw_bytes=raw_bytes,fields=len(names),evidence_suites=results)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    parser.add_argument('--output')
    args=parser.parse_args()
    result=verify(args.iso)
    if args.output:
        output=Path(args.output)
        if output.resolve()==Path(args.iso).resolve():
            raise ValueError('Output must not overwrite ISO')
        output.parent.mkdir(parents=True,exist_ok=True)
        output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result,sort_keys=True))
