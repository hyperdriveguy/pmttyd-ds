#!/usr/bin/env python3
"""Copy exact US message resources from the read-only ISO into ignored RomFS.

No text conversion, token removal or dialogue selection is performed.
"""
import argparse
import hashlib
import json
from pathlib import Path
from iso_assets import DEFAULT_ISO, inventory
from message_resource_evidence import inspect_resource


def build(iso, output):
    iso, output = Path(iso), Path(output)
    if output.resolve() == iso.resolve() or iso.resolve().is_relative_to(output.resolve()):
        raise ValueError('Message output must be separate from the ISO')
    info = inventory(iso)
    if info['disc_id'] != 'G8ME01':
        raise ValueError('Original US G8ME01 required')
    records = []
    output.mkdir(parents=True, exist_ok=True)
    with iso.open('rb') as disc:
        for entry in info['files']:
            if not entry['path'].startswith('msg/US/'):
                continue
            name = entry['path'][len('msg/US/'):]
            if '/' in name or not name.endswith('.txt'):
                raise ValueError('Unexpected US message resource path')
            disc.seek(entry['offset'])
            data = disc.read(entry['size'])
            if len(data) != entry['size']:
                raise ValueError('Truncated US message resource')
            census = inspect_resource(data)
            if census['unmatched_key'] or census['trailing_nonzero_bytes']:
                raise ValueError('Unexpected US message string boundary')
            target = output/name
            if not target.is_file() or target.read_bytes() != data:
                temporary = target.with_suffix('.txt.tmp')
                temporary.write_bytes(data)
                temporary.replace(target)
            records.append(dict(source_path=entry['path'], source_offset=entry['offset'],
                                size=len(data), sha256=hashlib.sha256(data).hexdigest(),
                                pair_count=census['pair_count'], duplicate_key_hex=census['duplicate_key_hex']))
    if not records:
        raise ValueError('No original US message resources')
    manifest = dict(source_disc='G8ME01', conversion='Byte-for-byte; original IDs and control tokens retained',
                    license='Local original-game assets; no redistribution rights', resources=records)
    (output/'manifest.json').write_text(json.dumps(manifest, indent=2)+'\n')
    return manifest


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--output', default='romfs/messages')
    args = parser.parse_args()
    result = build(args.iso, args.output)
    print(f"Original US messages: {len(result['resources'])} resources, "
          f"{sum(x['pair_count'] for x in result['resources'])} pairs")
