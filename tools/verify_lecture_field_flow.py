#!/usr/bin/env python3
"""Verify scoped US field lecture evidence against a pinned metadata manifest.

Read-only original ISO; reports contain hashes, operands and keys, never dialogue.
Event hashes verify bytes, not execution or the semantics of called services.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO, inventory
from rel_event_inventory import read_bindings
from dol_event_inventory import parse_commands
from message_resource_evidence import inspect_resource

MANIFEST = Path(__file__).with_name('lecture_field_evidence.json')


def verify(iso):
    expected = json.loads(MANIFEST.read_text())
    info = inventory(iso)
    with Path(iso).open('rb') as disc:
        if disc.read(8) != b'G8ME01\0\0':
            raise ValueError('US disc0 revision0 required')
        def read_file(path):
            entry = next(x for x in info['files'] if x['path'] == path)
            disc.seek(entry['offset'])
            data = disc.read(entry['size'])
            if len(data) != entry['size']:
                raise ValueError('Truncated original resource')
            return data
        raw = read_file('rel/gor.rel')
        message_raw = read_file('msg/US/gor_02.txt')
    for data, key in ((raw, 'rel_sha256'), (message_raw, 'message_sha256')):
        if hashlib.sha256(data).hexdigest() != expected[key]:
            raise ValueError(f'Resource identity mismatch: {key}')
    local, imports, foreign = read_bindings(raw)
    for event in expected['events']:
        off, size = event['file_offset'], event['size']
        data = raw[off:off+size]
        if hashlib.sha256(data).hexdigest() != event['sha256']:
            raise ValueError(f"Event bytes mismatch: {event['name']}")
        if len(parse_commands(data, off, {})) != event['commands']:
            raise ValueError(f"Event count mismatch: {event['name']}")
    bindings = []
    for site in sorted(set(local) | set(imports) | set(foreign)):
        if not any(e['file_offset'] <= site < e['file_offset']+e['size'] for e in expected['events']):
            continue
        bindings.append([site, local.get(site), imports.get(site), foreign.get(site)])
    digest = hashlib.sha256(json.dumps(bindings, sort_keys=True).encode()).hexdigest()
    if digest != expected['bindings_sha256'] or len(bindings) != expected['binding_count']:
        raise ValueError('Scoped event relocation bindings mismatch')
    for anchor in expected['anchors']:
        words = list(struct.unpack_from('>'+str(len(anchor['words']))+'I', raw, anchor['file_offset']))
        if words != anchor['words']:
            raise ValueError(f"Operand anchor mismatch: {anchor['description']}")
    for site, target in expected['local_links']:
        if local.get(site, {}).get('file_offset') != target:
            raise ValueError(f'Local entry/NPC binding mismatch: {site:#x}')
    pairs = inspect_resource(message_raw)['pairs']
    for message in expected['messages']:
        site = message['file_offset']
        key = message['key'].encode()
        off = local.get(site+12, {}).get('file_offset')
        if off is None or raw[off:off+len(key)+1] != key+b'\0':
            raise ValueError('Original message key binding mismatch')
        if imports.get(site+4) != message['service']:
            raise ValueError('Message service binding mismatch')
        matched = [p for p in pairs if p['key_hex'] == key.hex()]
        if len(matched) != 1 or matched[0]['text_sha256'] != message['text_sha256']:
            raise ValueError('Original US payload identity mismatch')
        if 'speaker_offset' in message:
            if local.get(site+20, {}).get('file_offset') != message['speaker_offset']:
                raise ValueError('Original speaker binding mismatch')
    return dict(schema=1, scope='Static US lecture field entry/choice/return requests; not original execution',
                events=expected['events'], binding_count=len(bindings),
                operand_anchors=len(expected['anchors']), local_links=len(expected['local_links']),
                messages=expected['messages'], rel_sha256=expected['rel_sha256'],
                message_resource='msg/US/gor_02.txt', message_sha256=expected['message_sha256'])


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    if args.output and args.output.resolve() in (Path(args.iso).resolve(), MANIFEST.resolve()):
        parser.error('Cannot overwrite original ISO or pinned manifest')
    result = verify(args.iso)
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(json.dumps(result, indent=2)+'\n')
    print(f"Verified {len(result['events'])} event spans, {result['binding_count']} relocations, "
          f"{result['operand_anchors']} operand anchors, {result['local_links']} entry/NPC links "
          f"and {len(result['messages'])} US message sites")


if __name__ == '__main__':
    main()
