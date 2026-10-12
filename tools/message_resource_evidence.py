#!/usr/bin/env python3
"""Read-only census of original US message string pairs; emits metadata, not text.

Follows the loader's first-empty-string boundary. This is static evidence, not
message execution, encoding conversion or a control-token interpreter.
"""
import argparse
import hashlib
import json
import re
from pathlib import Path
from iso_assets import DEFAULT_ISO, inventory


def inspect_resource(data):
    strings = []
    offset = 0
    while offset < len(data):
        end = data.find(b'\0', offset)
        if end < 0:
            raise ValueError(f'Unterminated string at {offset:#x}')
        if end == offset:
            break
        strings.append((offset, data[offset:end]))
        offset = end + 1
    if offset == len(data):
        raise ValueError('Missing empty-string resource terminator')
    pairs = []
    seen = set()
    duplicate_keys = []
    tokens = {}
    for i in range(0, len(strings) - 1, 2):
        key_offset, key = strings[i]
        text_offset, text = strings[i+1]
        if key in seen:
            duplicate_keys.append(key.hex())
        seen.add(key)
        for raw in re.findall(rb'<([^<>]*)>', text):
            token = raw.split(b' ', 1)[0].decode('ascii', errors='backslashreplace')
            tokens[token] = tokens.get(token, 0) + 1
        pairs.append(dict(key_hex=key.hex(), key_offset=key_offset,
                          text_offset=text_offset, text_size=len(text),
                          text_sha256=hashlib.sha256(text).hexdigest()))
    return dict(string_count=len(strings), pair_count=len(pairs),
                unmatched_key=bool(len(strings) % 2), terminator_offset=offset,
                trailing_nonzero_bytes=sum(x != 0 for x in data[offset+1:]),
                duplicate_key_hex=duplicate_keys,
                high_byte_count=sum(x >= 128 for x in data[:offset]),
                token_lexeme_counts=dict(sorted(tokens.items())), pairs=pairs)


def report(iso):
    info = inventory(iso)
    if info['disc_id'] != 'G8ME01':
        raise ValueError('US G8ME01 ISO required')
    resources = []
    with Path(iso).open('rb') as disc:
        for entry in info['files']:
            if not entry['path'].startswith('msg/US/'):
                continue
            disc.seek(entry['offset'])
            data = disc.read(entry['size'])
            if len(data) != entry['size']:
                raise ValueError('Truncated resource')
            resources.append(dict(path=entry['path'], iso_offset=entry['offset'],
                                  size=len(data), sha256=hashlib.sha256(data).hexdigest(),
                                  **inspect_resource(data)))
    return dict(schema=1, evidence='static US resource census; no original execution',
                resource_count=len(resources),
                pair_count=sum(x['pair_count'] for x in resources),
                resources=resources)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--output', required=True)
    args = parser.parse_args()
    result = report(args.iso)
    output = Path(args.output)
    if output.resolve() == Path(args.iso).resolve():
        raise ValueError('Cannot overwrite source ISO')
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(result, indent=2) + '\n')
    print(f"{result['resource_count']} US resources; {result['pair_count']} string pairs")
