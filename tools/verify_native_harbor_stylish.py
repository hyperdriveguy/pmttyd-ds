#!/usr/bin/env python3
"""Pin original Stylish decisions and actual zero-SP PointInc prefix reference, ISO read-only.
Native simulation policy adapts source scheduling; no original execution claim.
"""
import argparse
import hashlib
import json
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO, inventory
SPANS = [{'name': 'ACRStart', 'address': 2148560380, 'size': 184, 'sha256': '7eb12e360119f4f6aa1165740bc8289c8abd17206acba76ca464c72f9cb3bb76'}, {'name': 'AcrobatStart', 'address': 2149134268, 'size': 40, 'sha256': '5dcd3155d683c2e8ea41916b5b4f26a17ff08f51eedf165e3f5281f93c8f000c'}, {'name': 'AcrobatMain', 'address': 2149133748, 'size': 520, 'sha256': '4ad6402d8097fa3b262a1a75dbfaa85246e701cd0995cfd90a50edb168e391c0'}, {'name': 'ACRGetResult', 'address': 2148560252, 'size': 128, 'sha256': 'f63d4a3a813d76f95c48212651fa4098a49c0141caa11dd749b7f50d0066d1de'}, {'name': 'AcrobatGetResult', 'address': 2149133572, 'size': 24, 'sha256': '82753d04a13a380b5f4b7b2ad51ebea8974fa70fab3e74a3b6f7118a8e7f927f'}, {'name': 'PointInc', 'address': 2149594256, 'size': 504, 'sha256': '89f6ddf62dfa3bbfe50d279132092d4927e6d41c116c01ce643e749e8cb05b31'}, {'name': 'BreakSlotGetWork', 'address': 2149601868, 'size': 16, 'sha256': '78c442cd3a8d835c51e54e160ee702bd26547edbe06c69fad9a80202fcc65231'}, {'name': 'PouchGetPtr', 'address': 2148359856, 'size': 8, 'sha256': 'dbcd2f415efe4aa4e5684ee546cf73d97cc641b5a0290f1b46b14ab606786537'}]

def verify(iso=DEFAULT_ISO):
    rows = {row['path']: row for row in inventory(iso)['files']}
    with Path(iso).open('rb') as source:
        if source.read(8) != b'G8ME01\0\0':
            raise ValueError('Original US revision zero required')
        source.seek(0x420)
        dol, = struct.unpack('>I', source.read(4))
        source.seek(dol)
        header = source.read(256)
        for span in SPANS:
            if 'file' in span:
                offset = rows[span['file']]['offset'] + span['offset']
            else:
                address, size = span['address'], span['size']
                for i in range(18):
                    off, base, count = [struct.unpack_from('>I', header, table + 4*i)[0] for table in (0, 0x48, 0x90)]
                    if base <= address and address + size <= base + count:
                        offset = dol + off + address - base
                        break
                else:
                    raise ValueError('Source DOL span unavailable')
            source.seek(offset)
            raw = source.read(span['size'])
            if hashlib.sha256(raw).hexdigest() != span['sha256']:
                raise ValueError('Original source changed: ' + span['name'])
    return dict(spans=len(SPANS), source='G8ME01 revision0', original_executed=False,
                policy='Native physical Stylish policy + genuine PointInc zero-domain only; debug display/enabled reels remain held')

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    print(json.dumps(verify(parser.parse_args().iso), indent=2))
