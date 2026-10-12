#!/usr/bin/env python3
"""Pack original US DOL event words for the native original-address reader.

No VM execution,rewriting,dialogue generation or callback-success substitution.
Outputs contain original game data and must stay outside source history.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from dol_event_inventory import inventory_events
from iso_assets import DEFAULT_ISO

MAGIC = b'PMEVT001'
DEFAULT_EVENTS = ['marioAttackEvent_NormalJump@battle_mario.o',
                  'marioAttackEvent_MajinaiPowerUpCheck@battle_mario.o',
                  'btldefaultevt_SuitoruBadgeEffect@battle_event_default.o',
                  'subset_attack_ready_jump_event@battle_event_subset.o']
PINNED_EVENTS = {
    'marioAttackEvent_NormalJump': (0x80355cf0, 0xf0c, '2858f3f63cfb95dc84eb6ed3b363c530e24200063044f37afd275d3847f9720c'),
    'marioAttackEvent_MajinaiPowerUpCheck': (0x80363388, 0x8c, '3d44dcff79577976ecc7a400be16eb972f8d464479a8b314b8bb22ec8cc661d4'),
    'btldefaultevt_SuitoruBadgeEffect': (0x8034f2e4, 0x1e8, 'dadf7e175f491ab9e4a12ae63411cd7806df2bd0e73ec2be0a12e0b3ba454179'),
    'subset_attack_ready_jump_event': (0x80416bd0, 8, '1e9fcd4ca7e6723c4a822c370faad68aa89031573b2376d87e4cc3c6626ede61'),
}


def build_pack(iso, symbols, selections):
    if not selections:
        raise ValueError('Select at least one original event')
    inventory = inventory_events(iso, symbols, selections)
    records = []
    with Path(iso).open('rb') as disc:
        disc.seek(0x420)
        dol = struct.unpack('>I', disc.read(4))[0]
        disc.seek(dol)
        header = disc.read(256)
        sections = [tuple(struct.unpack_from('>I', header, b + i * 4)[0]
                          for b in (0, 0x48, 0x90)) for i in range(18)]
        for event in inventory['events']:
            address, size = event['address'], event['size']
            pinned = PINNED_EVENTS.get(event['name'])
            if pinned and (address, size, event['sha256']) != pinned:
                raise ValueError('Selected Jump dependency differs from the pinned US source')
            matches = [(offset + address - va, size + 4) for offset, va, length in sections
                       if va <= address and address + size + 4 <= va + length]
            if len(matches) != 1:
                raise ValueError('Event plus label-scan trailing word must fit one DOL section')
            offset, length = matches[0]
            disc.seek(dol + offset)
            data = disc.read(length)
            if len(data) != length or hashlib.sha256(data[:-4]).hexdigest() != event['sha256']:
                raise ValueError('Truncated or changed event source')
            records.append((address, data))
    # Overlapping trailing read words/selected streams retain one identical
    # original address interval,never two competing values for an address.
    segments = []
    for address, data in sorted(records):
        if address & 3 or len(data) & 3 or address + len(data) > 0x100000000:
            raise ValueError('Invalid original event address extent')
        if segments and address <= segments[-1][0] + len(segments[-1][1]):
            start, previous = segments[-1]
            overlap = start + len(previous) - address
            shared = min(overlap, len(data))
            if previous[address-start:address-start+shared] != data[:shared]:
                raise ValueError('Conflicting overlapping event words')
            segments[-1] = (start, previous + data[shared:])
        else:
            segments.append((address, data))
    offset = 12 + 12 * len(segments)
    table, payload, spans = [], [], []
    for address, data in segments:
        table.append(struct.pack('>III', address, len(data), offset))
        spans.append(dict(address=address, size=len(data), offset=offset,
                          sha256=hashlib.sha256(data).hexdigest()))
        payload.append(data)
        offset += len(data)
    packed = MAGIC + struct.pack('>I', len(segments)) + b''.join(table + payload)
    manifest = dict(schema=1, format='PMEVT001', source=dict(iso=str(Path(iso)),
                    revision='US G8ME01 disc0 revision0', container='main.dol'),
                    conversion='Original big-endian words and addresses; four original bytes '
                    'after each symbol retained for unconditional terminal label-scan read; '
                    'overlapping/adjacent ranges merged without rewriting',
                    licensing='Original Nintendo/Intelligent Systems game data; no redistribution '
                    'rights granted. Keep source and compiled pack outside source history.',
                    size=len(packed), sha256=hashlib.sha256(packed).hexdigest(), segments=spans,
                    events=inventory['events'], limitation='Original data extraction only; '
                    'callback registry, source owners and native execution remain required')
    return packed, manifest


def write_pack(iso, symbols, selections, output):
    output = Path(output)
    manifest_path = Path(str(output) + '.json')
    protected = {Path(iso).resolve(), Path(symbols).resolve()}
    if output.resolve() in protected or manifest_path.resolve() in protected:
        raise ValueError('Cannot overwrite source ISO or symbol map')
    packed, manifest = build_pack(iso, symbols, selections)
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_bytes(packed)
    manifest_path.write_text(json.dumps(manifest, indent=2) + '\n')
    return manifest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--symbols', default='artifacts/reference/us_symbols.csv')
    parser.add_argument('--output', default='local-assets/events/jump.pmevt')
    parser.add_argument('events', nargs='*')
    args = parser.parse_args()
    manifest = write_pack(args.iso, args.symbols, args.events or DEFAULT_EVENTS, args.output)
    print(json.dumps(dict(events=len(manifest['events']), segments=len(manifest['segments']),
                         bytes=manifest['size'], sha256=manifest['sha256'])))


if __name__ == '__main__':
    main()
