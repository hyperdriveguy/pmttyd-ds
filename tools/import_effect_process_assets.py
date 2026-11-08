#!/usr/bin/env python3
"""Import original effect driver cold data and US texture library read-only."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO, inventory
from verify_effect_driver import verify, US_TEXTURE_SOURCE

LICENSE = 'Nintendo/Intelligent Systems original game asset; no redistribution license granted'
SPANS = (
    (0x802c1f20, 24, 'c8280fa8f6b9556cea470b3b5d3ae736b216fc86c28cceb20444e1bcfe63c779'),
    (0x80415ee0, 4, '99950f034476db441242aee98cde760c7c67c267f19b4dc9487e7f18c98dd800'),
    (0x80415ee4, 28, 'bb679a19398dfd1ef48a1da7bccad8b0510f027a20d2e900438f68fab5e9cf55'),
    (0x80419a40, 4, '91748e09007693d05c5d49ca9394b7c6f76e2423217fdabf20189eefb560ddfe'),
    (0x8041e908, 24, '9d908ecfb6b256def8b49a7c504e6c889c4b0e41fe6ce3e01863dd7b61a20aa0'),
    (0x8041ecd0, 8, 'af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc'),
    (0x80420180, 28, '560468cb70f238277f7929eea027e1a31ece0dbcf9c084bfb74b3a2703f664b9'),
)

def build(iso, destination):
    destination = Path(destination)
    if destination.exists() and (not destination.is_dir() or any(destination.iterdir())):
        raise ValueError('Import destination must be empty')
    verify(iso)
    segments, records = [], []
    with Path(iso).open('rb') as disc:
        disc.seek(0x420)
        dol, = struct.unpack('>I', disc.read(4))
        disc.seek(dol)
        header = disc.read(256)
        sections = [tuple(struct.unpack_from('>I', header, k+4*i)[0]
                          for k in (0, 0x48, 0x90)) for i in range(18)]
        bss, bss_size = struct.unpack_from('>II', header, 0xd8)
        for address, size, expected in SPANS:
            data = bytearray()
            initialized = []
            for at in range(address, address+size):
                offsets = [dol+off+at-base for off, base, length in sections
                           if base <= at < base+length]
                if len(offsets) == 1:
                    disc.seek(offsets[0]); value = disc.read(1)
                    if len(value) != 1: raise ValueError('Truncated DOL')
                    data += value; initialized.append(offsets[0])
                elif not offsets and bss <= at < bss+bss_size:
                    data.append(0)
                else:
                    raise ValueError('Unmapped/ambiguous original data')
            if hashlib.sha256(data).hexdigest() != expected:
                raise ValueError('Original effect cold data differs')
            segments.append(dict(address=address, bytes=data.hex()))
            records.append(dict(address=hex(address), bytes=size, sha256=expected,
                                source='initialized DOL' if initialized else 'original DOL BSS zero initialization',
                                iso_offset=initialized[0] if initialized else None, license=LICENSE))
        item = next(e for e in inventory(iso)['files'] if e['path'] == US_TEXTURE_SOURCE['path'])
        disc.seek(item['offset']); texture = disc.read(item['size'])
        if len(texture) != US_TEXTURE_SOURCE['size'] or hashlib.sha256(texture).hexdigest() != US_TEXTURE_SOURCE['sha256']:
            raise ValueError('Original US effect library differs')
    destination.mkdir(parents=True, exist_ok=True)
    (destination/'memory.json').write_text(json.dumps(dict(schema=1, segments=segments, license=LICENSE), indent=2)+'\n')
    (destination/'effect_us.tpl').write_bytes(texture)
    (destination/'provenance.json').write_text(json.dumps(dict(source='US G8ME01 disc0 revision0',
        spans=records, texture=dict(**US_TEXTURE_SOURCE, iso_offset=item['offset'], license=LICENSE),
        conversion='Original initialized bytes and original BSS zeros; padded format retains original adjacent bytes. TPL is unchanged and unrelocated. DVD root remains owned by existing UI startup.',
        license=LICENSE), indent=2)+'\n')

if __name__ == '__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--destination', required=True)
    args=parser.parse_args(); build(args.iso, args.destination)
