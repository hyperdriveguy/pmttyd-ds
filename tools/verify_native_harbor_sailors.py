#!/usr/bin/env python3
"""Read-only US source pins for the bounded native Harbor sailor constructor."""
import hashlib
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO, inventory
from field_map_npc_assets import parsed_records
from rel_event_inventory import read_bindings


def verify(iso=DEFAULT_ISO):
    records = inventory(iso)['files']
    def read(name):
        matches = [item for item in records if item['path'] == name]
        if len(matches) != 1:
            raise ValueError('Missing unique source ' + name)
        item = matches[0]
        with Path(iso).open('rb') as disc:
            if disc.read(8) != b'G8ME01\0\0':
                raise ValueError('US revision zero required')
            disc.seek(item['offset'])
            raw = disc.read(item['size'])
        if len(raw) != item['size']:
            raise ValueError('Truncated source')
        return raw
    tags, rel = read('m/gor_00/s'), read('rel/gor.rel')
    if hashlib.sha256(tags).hexdigest() != '361cc4c7e2f85bc4b632fcc6daab9283fa39028676b2d27bb9c74a0989dcf32a':
        raise ValueError('Original tag source changed')
    if hashlib.sha256(rel).hexdigest() != 'd4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a':
        raise ValueError('Original gor REL changed')
    parsed = parsed_records(tags)
    if len(parsed) != 17:
        raise ValueError('Original tag count changed')
    local, _, _ = read_bindings(rel)
    for ordinal, table_row, name, regular in [(2, 3, 'ガラの悪い水夫', 0x1ca0), (12, 11, '水夫ボム兵Ｂ', 0x3040)]:
        _, strings, _ = parsed[ordinal]
        if strings[0] != name.encode('shift_jis') or strings[4] != b'BTLNO_INVALID':
            raise ValueError('Original sailor identity/setup changed')
        row = 0x2443c + table_row * 0x5c
        reference = local[row + 0x10]
        if reference['section'] != 5 or reference['addend'] != regular:
            raise ValueError('Original regular script changed')
        name_ref = local[row]
        section_table = struct.unpack_from('>I', rel, 16)[0]
        section_start = struct.unpack_from('>I', rel, section_table + 8 * name_ref['section'])[0] & ~3
        at = section_start + name_ref['addend']
        if rel[at:rel.index(b'\0', at)] != strings[0]:
            raise ValueError('Tag/setup name mismatch')
    print('US Harbor 17 tags/two original setup rows/regular scripts pinned; ISO read-only')
if __name__ == '__main__':
    verify()
