#!/usr/bin/env python3
"""Package the native slice's Switch PCM tracks separately from GameCube/SFX."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import struct
import wave

ROOT = Path(__file__).resolve().parents[1]


def digest(path):
    h = hashlib.sha256()
    with path.open('rb') as f:
        for block in iter(lambda: f.read(1024 * 1024), b''):
            h.update(block)
    return h.hexdigest()


def validate_pcm(path, row):
    if digest(path) != row['pcm_sha256']:
        raise ValueError(f'PCM hash mismatch: {path}')
    with path.open('rb') as f:
        header = f.read(44)
    if len(header) != 44 or header[:4] != b'RIFF' or header[8:16] != b'WAVEfmt ' or header[36:40] != b'data':
        raise ValueError(f'Native PCM requires canonical 44-byte header: {path}')
    with wave.open(str(path), 'rb') as w:
        if (w.getframerate(), w.getnchannels(), w.getnframes(), w.getsampwidth()) != (row['rate'], row['channels'], row['samples'], 2):
            raise ValueError(f'PCM format mismatch: {path}')
    if row['channels'] not in (1, 2) or not 8000 <= row['rate'] <= 48000 or row['samples'] <= 0:
        raise ValueError(f'Unsupported native PCM: {path}')
    if path.stat().st_size != 44 + row['samples'] * row['channels'] * 2:
        raise ValueError(f'PCM payload extent mismatch: {path}')
    loop = row['loop']
    if bool(loop) != row['loop_enabled']:
        raise ValueError(f'Contradictory loop metadata: {path}')
    if loop and not 0 <= loop[0] < loop[1] <= row['samples']:
        raise ValueError(f'Invalid loop: {path}')
    sidecar = Path(str(path) + '.loop')
    if loop and (not sidecar.exists() or sidecar.read_bytes() != struct.pack('<II', *loop)):
        raise ValueError(f'Loop sidecar mismatch: {path}')
    if not loop and sidecar.exists():
        raise ValueError(f'Stale loop sidecar: {path}')


def package(library, output, config, soundtrack='switch'):
    destination = output / 'soundtrack-switch'
    marker = output / 'soundtrack-switch.bin'
    if soundtrack == 'gamecube':
        output.mkdir(parents=True, exist_ok=True)
        if destination.exists():
            shutil.rmtree(destination)
        # Keep a top-level dependency in both modes so switching back rebuilds
        # the embedded RomFS instead of leaving an older Switch binary intact.
        marker.write_bytes(b'GCM1')
        return []
    settings = json.loads(config.read_text())
    if settings['schema'] != 1 or len(settings['tracks']) != 18:
        raise ValueError('Expected the native 18-track catalog')
    manifest = json.loads((library / 'manifest.json').read_text())
    rows = {}
    for row in manifest['audio']:
        if row['output'].startswith('pcm/stream/'):
            for name in row['sound_names']:
                if name in rows:
                    raise ValueError(f'Ambiguous Switch sound name: {name}')
                rows[name] = row
    # Validate every source before publishing or replacing any product.
    selected = []
    for alias, name in settings['tracks']:
        if Path(alias).name != alias or not alias or name not in rows:
            raise ValueError(f'Unknown or invalid soundtrack binding: {alias}: {name}')
        row = rows[name]
        source = library / row['output']
        validate_pcm(source, row)
        selected.append((alias, name, source, row))
    marker.unlink(missing_ok=True)
    destination.mkdir(parents=True, exist_ok=True)
    records = []
    for alias, name, source, row in selected:
        target = destination / (alias + '.wav')
        if not target.exists() or digest(target) != row['pcm_sha256']:
            temporary = target.with_suffix('.tmp')
            shutil.copyfile(source, temporary)
            temporary.replace(target)
        sidecar = Path(str(target) + '.loop')
        if row['loop']:
            sidecar.write_bytes(struct.pack('<II', *row['loop']))
        else:
            sidecar.unlink(missing_ok=True)
        records.append(dict(alias=alias, sound_name=name, **row))
    (destination / 'manifest.json').write_text(json.dumps(dict(
        schema=1, tracks=records, selection_scope=settings['selection_scope'],
        conversion='Unchanged PCM16 source rate/channels; BFSAR loop frame indices preserved. Streamed through NDSP.',
        license='Nintendo/Intelligent Systems original assets; local use, no redistribution rights.'), indent=2) + '\n')
    marker.write_bytes(b'SWM1')
    return records


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--library', type=Path, default=ROOT / 'local-assets/switch/audio')
    p.add_argument('--output', type=Path, default=ROOT / 'romfs')
    p.add_argument('--config', type=Path, default=ROOT / 'config/switch_soundtrack.json')
    p.add_argument('--soundtrack', choices=('switch', 'gamecube'), default='switch')
    args = p.parse_args()
    records = package(args.library, args.output, args.config, args.soundtrack)
    print(f'{args.soundtrack} soundtrack ready: {len(records)} Switch tracks; existing GameCube SFX retained')


if __name__ == '__main__':
    main()
