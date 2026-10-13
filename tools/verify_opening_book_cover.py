#!/usr/bin/env python3
"""Read-only US procedural demo_book and camera8 initial lens verification.

Source hashes verify inspected geometry/state and camera setup; they do not
claim original SDK numeric parity or cover later runtime camera mutations.
"""
import argparse
import hashlib
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO, inventory
from verify_opening_book_requests import verify as verify_requests
from rel_event_inventory import inventory_events

CAMERA = [
    (0x80010514, 0x90, '10c139d886365135337282f7b0bf0ff24a16b715fc99d1a75b46ff086b015521'),
    (0x8041f6cc, 4, 'b4be016994a699fb6f1a6b61fa14065baacfc6d51af90abee79ebee422889a93'),
    (0x8041f6d8, 4, '21ffb33d3d4b668e4f995ef41446c3c0c640e906043a38accc054ca1494c32d6'),
    (0x8041f6d4, 4, '9dc1b62d60acff118f222dc22cb76a1c7e9f2fcc1ae7d16c56861eb0301bd59a'),
    (0x8041f720, 4, 'be9703ca3587e6d86a7fa4784ed7719a1559aa32bb21806c8f0e4a2ecdb31647'),
    (0x802bf658, 8, 'e62977bcf0da63d5d7cf6dcaa4b451dd794a47d6dbf7d93ebf20fd4c8bbe6ef1'),
]

def verify(iso, symbols):
    count = verify_requests(iso, symbols)
    entry=next(f for f in inventory(iso)['files'] if f['path']=='rel/dmo.rel')
    with Path(iso).open('rb') as source:
        source.seek(entry['offset']);raw=source.read(entry['size'])
    for offset,size,digest in [(0x1238,0x52c,'38bd23df9a5030f67b22b30e81a7789baebda8d88f202c3f04b0f05956b7dd9c'),(0x1dc,0x154,'39f367642182b440413b6ae685e3157f3426b9b6e49e78cff45665286fe72732')]:
        if hashlib.sha256(raw[offset:offset+size]).hexdigest()!=digest:raise ValueError('Original illustration/adjust source changed')
        count+=1
    commands={c['address']:c for c in inventory_events(raw,symbols,'dmo',['demo_evt'])['events'][0]['commands']}
    expected={0x2f08:('evt_sub_intpl_msec_init',[0,0,1000,3000]),0x2f28:('evt_sub_intpl_msec_get_value',[]),0x2f3c:('adjust',[4264967296])}
    for address,(name,values) in expected.items():
        operands=commands[address]['operands']
        if operands[0].get('annotation')!=name or [o['raw'] for o in operands[1:]]!=values:raise ValueError('Original final adjustment call changed')
        count+=1
    if commands[0x2f30]['opcode']!=61 or [o['raw'] for o in commands[0x2f30]['operands']]!=[4264967296,4065991296]:raise ValueError('Original final adjustment divisor changed')
    count+=1
    with Path(iso).open('rb') as source:
        source.seek(0x420)
        dol = struct.unpack('>I', source.read(4))[0]
        source.seek(dol)
        header = source.read(256)
        for address, size, digest in CAMERA:
            for i in range(18):
                offset, start, length = (struct.unpack_from('>I', header, b + i*4)[0] for b in [0, 0x48, 0x90])
                if start <= address and address + size <= start + length:
                    source.seek(dol + offset + address - start)
                    if hashlib.sha256(source.read(size)).hexdigest() != digest:
                        raise ValueError(f'Original camera8 source changed at {address:x}')
                    break
            else:
                raise ValueError('Camera8 anchor outside loaded DOL')
    return count + len(CAMERA)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--symbols', default='artifacts/reference/us_symbols.csv')
    args = parser.parse_args()
    print(f'{verify(args.iso, args.symbols)} original book/capture/camera8 anchors verified read-only')
