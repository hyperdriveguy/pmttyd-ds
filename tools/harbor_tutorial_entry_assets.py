#!/usr/bin/env python3
"""One-time original Crump entry import; never run by normal builds."""
import argparse
import hashlib
import json
from pathlib import Path
import tempfile
from PIL import Image
from asset_animation import decode
from asset_pipeline import write_json
from battle_pose_directory import source_directory
from character_mesh import compile_mesh
from iso_assets import DEFAULT_ISO, extract

LICENSE = 'Nintendo/Intelligent Systems original game asset; no redistribution license granted'
CLIPS = ['S_1', 'R_2', 'R_1', 'A_1A', 'A_1B', 'A_1C', 'D_1']
PINS = {'crump.model': ('a/c_kanbu', '34e4c470ee711eac49fdde6b4988c4d0d0c79d2bf1055fa58e6fe46079ccbf01'),
        'crump.tpl': ('a/c_kanbu-', '077869459c3b3852fdd2f9e531b7894afa1de6c10d6a21f6ab95204edf685ce1')}

def import_assets(iso, destination):
    destination = Path(destination)
    if destination.exists() and any(destination.iterdir()):
        raise ValueError('Original import requires an empty destination; preserve existing edits')
    # Validate and bake privately before publishing any new editable asset.
    with tempfile.TemporaryDirectory() as temporary:
        work = Path(temporary)
        provenance = {}
        for name, (source, expected) in PINS.items():
            extract(iso, source, work/name)
            if hashlib.sha256((work/name).read_bytes()).hexdigest() != expected:
                raise ValueError('Unexpected original resource: '+source)
            provenance[name] = json.loads((work/(name+'.provenance.json')).read_text())
            provenance[name]['license'] = LICENSE
        model, texture = work/'crump.model', work/'crump.tpl'
        base = compile_mesh([(model, texture, CLIPS)], work/'base.pmmesh', work/'base.png', work/'base.pmanim')
        extra = compile_mesh([(model, texture, CLIPS+['Z_1'])], work/'extra.pmmesh', work/'extra.png', work/'extra.pmanim')
        primary, combined = (decode((work/(name+'.pmanim')).read_bytes()) for name in ('base', 'extra'))
        if combined['clips'][:-1] != primary['clips'] or base['texture_cells'] != extra['texture_cells']:
            raise ValueError('Entry import would alter existing clip/atlas coordinates')
        if Image.open(work/'base.png').tobytes() != Image.open(work/'extra.png').tobytes():
            raise ValueError('Entry import would require a different atlas')
        fragment = dict(combined, clips=[combined['clips'][-1]])
        if fragment['clips'][0]['name'] != 'Z_1' or source_directory(model.read_bytes())[2][0] != 'Z_1':
            raise ValueError('Unexpected original initial descriptor')
        provenance['crump-z1.json'] = {'source_disc': 'G8ME01', 'source_path': 'a/c_kanbu',
            'source_sha256': PINS['crump.model'][1], 'license': LICENSE,
            'conversion': 'Only original Z_1 converted after existing seven clips; same texture-cell order and unchanged atlas pixels. Normal build appends to current edited primary, never regenerates it.',
            'reference_clips': CLIPS, 'reference_atlas_rgba_sha256': hashlib.sha256(Image.open(work/'base.png').tobytes()).hexdigest()}
        destination.mkdir(parents=True, exist_ok=True)
        for name in PINS:
            (destination/name).write_bytes((work/name).read_bytes())
        write_json(destination/'crump-z1.json', fragment)
        write_json(destination/'provenance.json', {'schema': 1, 'assets': provenance})
        (destination/'LICENSE.txt').write_text(LICENSE+'.\nOriginal rights retained. Tracking source assets grants no new license.\n')

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--destination', type=Path, required=True)
    args = parser.parse_args()
    import_assets(args.iso, args.destination)
