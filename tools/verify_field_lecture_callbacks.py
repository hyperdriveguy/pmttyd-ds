#!/usr/bin/env python3
"""Check US field lecture callback bytes and actual imported call sites read-only.

Pinned metadata is not an execution oracle or a claim of complete party services.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO, inventory
from rel_event_inventory import read_bindings

MANIFEST = Path(__file__).with_name('field_lecture_callback_evidence.json')


def verify(iso, manifest=MANIFEST):
    expected = json.loads(Path(manifest).read_text())
    info = inventory(iso)
    with Path(iso).open('rb') as disc:
        if disc.read(8) != b'G8ME01\0\0':
            raise ValueError('Expected US disc0 revision0')
        disc.seek(0x420)
        base = struct.unpack('>I', disc.read(4))[0]
        disc.seek(base)
        header = disc.read(256)
        def read(address, size):
            matches = []
            for i in range(18):
                off, va, length = (struct.unpack_from('>I', header, j+i*4)[0] for j in (0, 0x48, 0x90))
                if va <= address and address+size <= va+length:
                    matches.append(base+off+address-va)
            if len(matches) != 1:
                raise ValueError('Unmapped or ambiguous DOL span')
            disc.seek(matches[0])
            data = disc.read(size)
            if len(data) != size:
                raise ValueError('Truncated DOL evidence')
            return data
        for body in expected['bodies']:
            if hashlib.sha256(read(body['address'], body['size'])).hexdigest() != body['sha256']:
                raise ValueError(f"Body mismatch: {body['name']}")
        for anchor in expected['anchors']+expected['constants']:
            if read(anchor['address'], len(anchor['hex'])//2).hex() != anchor['hex']:
                raise ValueError(f"Instruction/constant mismatch: {anchor['address']:#x}")
        for span in expected.get('data', []):
            data = read(span['address'], span['size'])
            if hashlib.sha256(data).hexdigest() != span['sha256']:
                raise ValueError('Original data span identity mismatch')
            if 'words' in span and (len(data) != 4*len(span['words']) or
                                    list(struct.unpack('>'+str(len(span['words']))+'I', data)) != span['words']):
                raise ValueError('Original data word/dispatch binding mismatch')
        entry = next(e for e in info['files'] if e['path'] == 'rel/gor.rel')
        disc.seek(entry['offset'])
        raw = disc.read(entry['size'])
        if hashlib.sha256(raw).hexdigest() != expected['rel_sha256']:
            raise ValueError('US gor identity mismatch')
        _, imports, _ = read_bindings(raw)
        for binding in expected['rel_imports']:
            if imports.get(binding['file_offset']) != binding['address']:
                raise ValueError('Actual lecture callback import mismatch')
    return dict(schema=1, scope='Static field intersection and party callback bodies; no original execution',
                body_checks=len(expected['bodies']), instruction_checks=len(expected['anchors']),
                constant_checks=len(expected['constants']), import_checks=len(expected['rel_imports']),
                data_checks=len(expected.get('data', [])),
                bodies=expected['bodies'], rel_sha256=expected['rel_sha256'])


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
    print(f"Verified {result['body_checks']} DOL bodies, {result['instruction_checks']} instructions, "
          f"{result['constant_checks']} constants and {result['import_checks']} used field imports")


if __name__ == '__main__':
    main()
