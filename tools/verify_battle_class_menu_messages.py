#!/usr/bin/env python3
"""Read-only original US provenance for the mapped class-menu label owner."""
import argparse
import hashlib
import json
from pathlib import Path
import re
from iso_assets import DEFAULT_ISO, inventory
from verify_action_class_table import CONSTANTS, verify as verify_source
from message_resource_evidence import inspect_resource


def verify(iso=DEFAULT_ISO, global_resource=None):
    verify_source(iso)
    keys = {address: bytes.fromhex(raw)[:-1] for address, raw in CONSTANTS.items()}
    source = (Path(__file__).resolve().parents[1] /
              'source/battle/battle_class_menu_messages.cpp').read_text()
    mapped = {int(address, 16): key.encode('ascii') for address, key in
              re.findall(r'\{(0x[0-9a-f]+),\s*"([^"]+)"\}', source)}
    if mapped != keys:
        raise ValueError('Mapped source-key/ID pairs differ from original DOL bytes')
    info = inventory(iso)
    entry, = [e for e in info['files'] if e['path'] == 'msg/US/global.txt']
    with Path(iso).open('rb') as disc:
        disc.seek(entry['offset'])
        data = disc.read(entry['size'])
    if len(data) != entry['size']:
        raise ValueError('Truncated original global message resource')
    if global_resource is not None and Path(global_resource).read_bytes() != data:
        raise ValueError('Native global message copy differs from original US bytes')
    census = inspect_resource(data)
    records = []
    for address, key in sorted(keys.items()):
        pair, = [p for p in census['pairs'] if bytes.fromhex(p['key_hex']) == key]
        begin = pair['text_offset']
        raw = data[begin:begin + pair['text_size'] + 1]
        records.append(dict(source_key=hex(address), key=key.decode('ascii'),
                            text_offset=begin, size_including_nul=len(raw),
                            sha256_including_nul=hashlib.sha256(raw).hexdigest()))
    return dict(source='Original US G8ME01 revision0, read-only', labels=records,
                resource_sha256=hashlib.sha256(data).hexdigest(),
                native_copy_checked=global_resource is not None,
                limitation='Static provenance only; C++ tests own actual VM mapping proof')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--global-resource')
    args = parser.parse_args()
    print(json.dumps(verify(args.iso, args.global_resource), sort_keys=True))


if __name__ == '__main__':
    main()
