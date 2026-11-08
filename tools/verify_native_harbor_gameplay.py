#!/usr/bin/env python3
"""Pin original phase ordering/tags and whole Crump dialogue hook, ISO read-only.
The native driver is a port scheduling policy, not original scheduler execution.
"""
import argparse
import hashlib
import json
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO, inventory
SPANS = [{'name': 'Phase', 'address': 2148647604, 'size': 156, 'sha256': '2010dae50a20ef4cb62d7d69f98d2e00264fb5c328e24c3ac2244423966db8b3'}, {'name': 'Move', 'address': 2148645924, 'size': 1680, 'sha256': '57a7cfe302fd8f68ccd7d0fa464f72dc5465fda72c4e16c6907378fa8dc1bb6e'}, {'name': 'MarioPhase', 'address': 2151770712, 'size': 4, 'sha256': 'ac894ab085f50b81fe000e99ccb27e5543de69f63d4f9105daab15dce90f81b3'}, {'name': 'CrumpPhaseEvent', 'file': 'rel/gor.rel', 'offset': 411984, 'size': 1128, 'sha256': '51287cf09226ca2a1cb598e3f5c243eaa7e597d58b44b6fbba76bb1b4f3d4140'}, {'name': 'CrumpPhaseTag', 'file': 'rel/gor.rel', 'offset': 396392, 'size': 4, 'sha256': '8a3044222ef06194ca6ec01b989000752c076c1c1aae1a0d81c82a5c94288b6d'}, {'name': 'GetResultPrizeLv', 'address': 2148562372, 'size': 156, 'sha256': '6d3c6674c779159d5fdb18e886e613e08187d3d25a2995c47c979db035a0c33a'}, {'name': 'ActionPrizeLevel', 'address': 2148507880, 'size': 68, 'sha256': '378c08e78a8712fb4bdd987c138139d6396e532b47586b17b9a8ee3010ca8ef5'}]

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
                policy='Native tutorial scheduling over unchanged original EVT/command/action/damage owners; not whole Main/End')

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    print(json.dumps(verify(parser.parse_args().iso), indent=2))
