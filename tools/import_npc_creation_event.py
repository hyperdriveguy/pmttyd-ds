#!/usr/bin/env python3
"""Explicit original NPC event import; refuses any nonempty source destination."""
import argparse, hashlib, json, struct, tempfile
from pathlib import Path
from iso_assets import DEFAULT_ISO, extract
from rel_data_pack import package
from field_map_npc_assets import Dol
LICENSE = 'Nintendo/Intelligent Systems original game asset; no redistribution license granted'

def import_source(iso, symbols, destination):
    destination = Path(destination)
    if destination.exists() and (not destination.is_dir() or any(destination.iterdir())):
        raise ValueError('New imports require an empty destination; existing asset edits are preserved')
    with tempfile.TemporaryDirectory() as directory:
        path = Path(directory) / 'gor.rel'; extract(iso, 'rel/gor.rel', path); raw = path.read_bytes()
        digest = hashlib.sha256(raw).hexdigest()
        if digest != 'd4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a':
            raise ValueError('Expected original US revision0 gor.rel')
        packed, metadata = package(raw, symbols, 'gor', ['gor_00.o::enter_gorotsuki_town_init'])
    spans = []
    for i in range(struct.unpack_from('>I', packed, 8)[0]):
        a, n, off = struct.unpack_from('>III', packed, 12 + 12*i)
        spans.append(dict(address=a, bytes=packed[off:off+n].hex()))
    dol = Dol(iso)
    try:
        zero = dol.read(0x8041fd84, 4)
    finally:
        dol.close()
    if zero != bytes(4):
        raise ValueError('Unexpected original SetTribe zero literal')
    spans.append(dict(address=0x8041fd84, bytes=zero.hex())); spans.sort(key=lambda x:x['address'])
    doc = dict(schema=1, root=0x90505208, segments=spans, license=LICENSE)
    provenance = dict(schema=1, iso_id='G8ME01 revision0', source='rel/gor.rel', source_sha256=digest,
        closure=metadata, dol_constant=dict(source='sys/main.dol', address=0x8041fd84, size=4, sha256=hashlib.sha256(zero).hexdigest()),
        policy='Original nonexecuting data/EVT closure, including real trailing scanner word; only source ADDR32 relocations use native section addresses. Hex JSON is editable source; PMEVT is generated.', license=LICENSE)
    destination.mkdir(parents=True, exist_ok=True)
    (destination/'event.json').write_text(json.dumps(doc, indent=2)+'\n')
    (destination/'provenance.json').write_text(json.dumps(provenance, indent=2)+'\n')
    return doc

if __name__ == '__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso', default=DEFAULT_ISO)
    p.add_argument('--symbols', default='artifacts/reference/us_symbols.csv')
    p.add_argument('--destination', default='assets/original/events/harbor-npc-creation')
    a=p.parse_args();import_source(a.iso, a.symbols, a.destination)
