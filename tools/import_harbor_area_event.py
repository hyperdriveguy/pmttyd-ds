#!/usr/bin/env python3
"""Import original US gor00 area initialization data into a new editable source."""
import argparse, hashlib, json, struct, tempfile
from pathlib import Path
from iso_assets import DEFAULT_ISO, extract
from rel_data_pack import package
from field_map_npc_assets import Dol
from dol_event_closure import linked_events, merge_segments
LICENSE = 'Nintendo/Intelligent Systems original game asset; no redistribution license granted'
HARBOR_ROOM = 1
HARBOR_INITIALIZER = 0x9050ef7c
HARBOR_INITIALIZER_BYTES = 0x870
LIGHT_ORIGINS = 0x802f3748
LIGHT_ORIGIN_BYTES = 24

def import_source(iso, symbols, destination):
    destination = Path(destination)
    if destination.exists() and (not destination.is_dir() or any(destination.iterdir())):
        raise ValueError('New imports require an empty destination; existing asset edits are preserved')
    with tempfile.TemporaryDirectory() as temporary:
        path = Path(temporary)/'gor.rel'
        extract(iso, 'rel/gor.rel', path)
        raw = path.read_bytes()
    digest = hashlib.sha256(raw).hexdigest()
    if digest != 'd4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a':
        raise ValueError('Expected original US revision0 gor.rel')
    packed, metadata = package(raw, symbols, 'gor', ['gor_00.o::gor_00_init_evt'])
    segments = []
    for i in range(struct.unpack_from('>I', packed, 8)[0]):
        address, size, offset = struct.unpack_from('>III', packed, 12+12*i)
        segments.append((address, packed[offset:offset+size]))
    linked, linked_metadata = linked_events(iso, raw, metadata['objects'], symbols)
    dol=Dol(iso)
    try:
        light_origins=dol.read(LIGHT_ORIGINS,LIGHT_ORIGIN_BYTES)
    finally:
        dol.close()
    packed = merge_segments(segments+linked+[(LIGHT_ORIGINS,light_origins)])
    spans = []
    for i in range(struct.unpack_from('>I', packed, 8)[0]):
        address, size, offset = struct.unpack_from('>III', packed, 12+12*i)
        spans.append(dict(address=address, bytes=packed[offset:offset+size].hex()))
    root = next(o for o in metadata['objects'] if o['name']=='gor_00_init_evt')
    if root['address'] != HARBOR_INITIALIZER or root['size'] != HARBOR_INITIALIZER_BYTES:
        raise ValueError('Unexpected original harbor area root')
    document = dict(schema=1, room=HARBOR_ROOM, root=root['address'], segments=spans, license=LICENSE)
    provenance = dict(schema=1, iso_id='G8ME01 revision0', source='rel/gor.rel', source_sha256=digest,
        closure=metadata, linked_dol_events=linked_metadata,
        light_origins=dict(source="sys/main.dol",address=LIGHT_ORIGINS,size=LIGHT_ORIGIN_BYTES,sha256=hashlib.sha256(light_origins).hexdigest()),
        policy='Original nonexecuting transitive REL data and linked DOL EVT closure, including actual scanner prefetch words. ADDR32 relocations retain native section addresses. No PPC text or invented event words. World-service/data dependencies remain separate.', license=LICENSE)
    destination.mkdir(parents=True, exist_ok=True)
    (destination/'event.json').write_text(json.dumps(document, indent=2)+'\n')
    (destination/'provenance.json').write_text(json.dumps(provenance, indent=2)+'\n')
    return document

if __name__ == '__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--symbols', default='artifacts/reference/us_symbols.csv')
    parser.add_argument('--destination', default='assets/original/events/harbor-area-init')
    arguments=parser.parse_args()
    import_source(arguments.iso, arguments.symbols, arguments.destination)
