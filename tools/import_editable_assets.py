#!/usr/bin/env python3
"""Explicitly import current original conversions into tracked editable sources.
Never run this during a normal build: authored edits must not be overwritten.
"""
import argparse
import json
from pathlib import Path
import shutil
import struct
import wave

from asset_pipeline import ROOT, digest, export_actor, export_model, write_json
from asset_animation import decode

LICENSE = 'Nintendo/Intelligent Systems original game asset; no redistribution license granted'


def import_assets(root, selection, destination):
    root = root.resolve(); destination = destination.resolve()
    if not destination.is_relative_to(root):
        raise ValueError('Editable import destination must be inside the source root')
    if destination.exists() and any(destination.iterdir()):
        raise ValueError('Import destination must be empty; preserve existing authored edits')
    entries = []; provenance = []
    def register(kind, source, output, origin, **extra):
        relative = source.relative_to(root).as_posix()
        entries.append({'id': 'original/' + output, 'kind': kind, 'source': relative,
                        'output': output, 'license': LICENSE, **extra})
        provenance.append({'source': relative, 'sha256_at_import': digest(source),
                           'origin': origin, 'license': LICENSE})
    for texture in selection['textures']:
        source = root / texture['source']
        target = destination / 'textures' / (texture['output'].removesuffix('.t3x') + '.png')
        target.parent.mkdir(parents=True, exist_ok=True); shutil.copyfile(source, target)
        register('texture', target, texture['output'], texture['origin'], format=texture.get('format', 'rgba8'))
    for prefix in selection['models']:
        source = root / 'romfs' / prefix; target = destination / 'models' / (prefix + '.json')
        export_model(source, target)
        register('model', target, prefix, {'native_export': 'romfs/' + prefix,
                                         'conversion': 'little-endian native triangle rows and batch metadata to lossless JSON'})
    for source in sorted((root / 'romfs').glob('*.pmmesh')):
        target = destination / 'actors' / (source.stem + '.json'); export_actor(source, target)
        register('actor', target, source.name, {'native_export': 'romfs/' + source.name,
                                              'conversion': 'PMCHAR1 named frame-zero poses to lossless JSON'})
    for source in sorted((root / 'romfs').glob('*.pmanim')):
        target = destination / 'animations' / (source.stem + '.json')
        write_json(target, decode(source.read_bytes()))
        register('animation', target, source.name, {'native_export': 'romfs/' + source.name,
                     'conversion': 'PMANIM3/4 hierarchy, base arrays, sparse tick keys and material state to lossless JSON'})
    for source in sorted((root / 'romfs').glob('*.wav')):
        target = destination / 'audio' / source.name
        target.parent.mkdir(parents=True, exist_ok=True); shutil.copyfile(source, target)
        extra = {}; sidecar = Path(str(source) + '.loop')
        if sidecar.exists():
            extra['loop'] = list(struct.unpack('<II', sidecar.read_bytes()))
        with wave.open(str(source), 'rb') as pcm:
            music = pcm.getnchannels() == 2 or sidecar.exists()
        register('music' if music else 'sfx', target, source.name,
                 {'native_export': 'romfs/' + source.name,
                  'conversion': 'original PCM16 rate/channels retained; loop intervals are sample frames'}, **extra)
    upstream = {}
    for name in ['manifest.json', 'audio-map.json', 'opening-assets.json', 'audience-resources.json',
                 'professor.json', 'professor-mario.json', 'professor-fence.json', 'field-actors.json',
                 'harbor-crump.json', 'harbor-xnaut.json', 'trio-attack.json', 'trio-battle.json',
                 'harbor-tutorial-assets.json', 'battle-startup-resources.json']:
        path = root / 'romfs' / name
        if path.exists():
            upstream['romfs/' + name] = json.loads(path.read_text())
    write_json(destination / 'conversion-provenance.json', {'schema': 1, 'upstream_converter_manifests': upstream})
    write_json(destination / 'provenance.json', {'schema': 1, 'assets': provenance,
               'upstream_converter_manifests': (destination / 'conversion-provenance.json').relative_to(root).as_posix(),
               'reference_ledger': 'docs/ASSETS_ARCHIVE.md', 'selection': selection})
    write_json(destination / 'recipes.json', {'schema': 1, 'assets': entries})
    print(f'Imported {len(entries)} editable original assets into {destination}')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, default=ROOT)
    parser.add_argument('--selection', type=Path, default=Path('assets/import.json'))
    parser.add_argument('--destination', type=Path, default=Path('assets/original'))
    args = parser.parse_args()
    try:
        root = args.root.resolve()
        selection = args.selection if args.selection.is_absolute() else root / args.selection
        destination = args.destination if args.destination.is_absolute() else root / args.destination
        import_assets(root, json.loads(selection.read_text()), destination)
    except (ValueError, OSError, KeyError, struct.error) as error:
        parser.exit(1, f'asset-import: {error}\n')


if __name__ == '__main__':
    main()
