#!/usr/bin/env python3
"""Check scoped US mode0 TEV,character-shadow and final publication evidence."""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO, inventory
from verify_field_lecture_callbacks import verify
from verify_animation_render_material import verify_census

MANIFEST = Path(__file__).with_name('map_tev_ordinary_evidence.json')


def verify_source(iso, expected):
    spec = expected['source_example']
    entry = next(e for e in inventory(iso)['files'] if e['path'] == spec['path'])
    with Path(iso).open('rb') as disc:
        disc.seek(entry['offset'])
        raw = disc.read(entry['size'])
    word = lambda o: struct.unpack_from('>I', raw, o)[0]
    count, base = word(0x138), word(0x19c)
    hist = Counter(word(base+108*i+8) for i in range(count))
    actual = dict(path=entry['path'], size=len(raw), sha256=hashlib.sha256(raw).hexdigest(),
                  draw_count=count, tev_mode_histogram=[dict(value=k,count=v) for k,v in sorted(hist.items())])
    if actual != spec:
        raise ValueError('Original Mario TEV mode/source identity mismatch')
    return actual


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    if args.output and args.output.resolve() in (Path(args.iso).resolve(), MANIFEST.resolve()):
        parser.error('Cannot overwrite ISO or pinned manifest')
    result = verify(args.iso, MANIFEST)
    expected = json.loads(MANIFEST.read_text())
    result['direct_reference_sites'] = verify_census(args.iso, expected)
    result['source_example'] = verify_source(args.iso, expected)
    result['reviewed_ranges'] = expected['reviewed_ranges']
    result['scope'] = 'One containing TEV identity/scoped303 words/four complete shadow leaves;other modes and middle postlude branches remain open;no original execution'
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(json.dumps(result, indent=2)+'\n')
    print(f"Verified {result['body_checks']} identities (one partial/four complete), "
          f"{result['instruction_checks']} scoped words, {result['data_checks']} spans, "
          f"{result['import_checks']} imports, {sum(len(t['sites']) for t in result['direct_reference_sites'])} direct sites and Mario TEV modes")


if __name__ == '__main__':
    main()
