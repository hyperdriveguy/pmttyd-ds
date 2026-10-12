#!/usr/bin/env python3
"""Verify US depth TEV blocks/source leaves and composed local reviewed coverage.

Instruction coverage supports manual contracts. It proves neither original
execution nor complete SDK/GX/source/scene/image behavior.
"""
import argparse
import json
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_field_lecture_callbacks import verify
from verify_animation_render_material import verify_census
from verify_animation_texture_light import verify_color_arithmetic

MANIFEST = Path(__file__).with_name('map_tev_depth_evidence.json')
PARTS = ('map_tev_ordinary_evidence.json','map_tev_fixed_evidence.json',
         'map_tev_time_evidence.json','map_tev_projected_evidence.json',
         'map_tev_terminal_evidence.json','map_tev_depth_evidence.json')


def check_coverage(specs):
    start, end = 0x80021b80, 0x8002493c
    words = {}
    for spec in specs:
        for anchor in spec['anchors']:
            a = anchor['address']
            if start <= a < end:
                if a % 4 or len(anchor['hex']) != 8:
                    raise ValueError('Malformed TEV instruction anchor')
                if a in words and words[a] != anchor['hex']:
                    raise ValueError('Contradictory overlapping TEV instruction evidence')
                words[a] = anchor['hex']
    missing = set(range(start,end,4))-words.keys()
    if missing:
        raise ValueError(f'Missing reviewed TEV words: {min(missing):#x}')
    return dict(address=start,size=end-start,instruction_words=len(words),
                scope='Complete composed local instruction coverage supporting manual request contracts;not hardware/runtime proof')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    parser.add_argument('--output',type=Path)
    args = parser.parse_args()
    paths = [MANIFEST.with_name(p) for p in PARTS]
    if args.output and args.output.resolve() in {Path(args.iso).resolve(),*(p.resolve() for p in paths)}:
        parser.error('Cannot overwrite ISO or any pinned manifest')
    result = verify(args.iso,MANIFEST)
    specs = [json.loads(p.read_text()) for p in paths]
    # Recheck every contributing original body/word/data set before unioning.
    for path in paths[:-1]:
        verify(args.iso,path)
    result['composed_local_coverage'] = check_coverage(specs)
    expected = specs[-1]
    result['direct_reference_sites'] = verify_census(args.iso,expected)
    result['color_arithmetic'] = verify_color_arithmetic(expected)
    result['scope'] = 'Complete depth local blocks/seven source leaves and complete composed TEV local request coverage;SDK/GX/source/lifetime/runtime/full-game remain open'
    if args.output:
        args.output.parent.mkdir(parents=True,exist_ok=True)
        args.output.write_text(json.dumps(result,indent=2)+'\n')
    print(f"Verified {result['instruction_checks']} depth/source words, {result['body_checks']} identities, "
          f"{result['data_checks']} spans, {result['import_checks']} imports, six direct sites and "
          f"{result['composed_local_coverage']['instruction_words']} composed TEV words")


if __name__ == '__main__':
    main()
