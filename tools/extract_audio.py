#!/usr/bin/env python3
"""Extract original ISO audio read-only and convert a local loopable PCM library.

WAVs use the port's 44-byte PCM16 header and separate .wav.loop files containing
two little-endian uint32 PCM frame indices [start, end). Nothing is resampled,
normalized, faded or renamed to an inferred game event. The library is separate
from RomFS so a full soundtrack is not embedded in the early Rogueport slice.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import wave

from audio_convert import stm, sample_directory, convert_sample
from iso_assets import DEFAULT_ISO, inventory, extract

ROOT = Path(__file__).resolve().parents[1]
BANK_SUFFIXES = ('sdir', 'samp', 'pool', 'proj', 'etbl', 'slib')


def digest(path):
    with Path(path).open('rb') as source:
        return hashlib.file_digest(source, 'sha256').hexdigest()


def verify_pcm(path, record):
    """Check actual frame data and the exact sidecar consumed by native audio."""
    with wave.open(str(path), 'rb') as pcm:
        if (pcm.getnchannels(), pcm.getsampwidth(), pcm.getframerate(), pcm.getnframes()) != (
                record['channels'], 2, record['rate'], record['samples']):
            raise ValueError(f'PCM format mismatch: {path}')
        if len(pcm.readframes(record['samples'])) != record['samples'] * record['channels'] * 2:
            raise ValueError(f'Truncated PCM: {path}')
    sidecar = Path(str(path) + '.loop')
    if record['loop_enabled']:
        start, end = struct.unpack('<II', sidecar.read_bytes())
        if [start, end] != list(record['loop']) or not 0 <= start < end <= record['samples']:
            raise ValueError(f'Loop mismatch: {path}')
    elif sidecar.exists():
        raise ValueError(f'Unexpected loop sidecar: {path}')


def build(iso, output):
    iso, output = Path(iso).resolve(), Path(output).resolve()
    if output == iso or output in iso.parents:
        raise ValueError('Output must be separate from ISO')
    info = inventory(iso)
    if info['disc_id'] != 'G8ME01':
        raise ValueError('This pipeline requires the US G8ME01 disc')
    output.mkdir(parents=True, exist_ok=True)
    records, sources = [], []

    def acquire(name):
        target = output / 'raw' / name
        extract(iso, name, target)
        provenance = json.loads(Path(str(target) + '.provenance.json').read_text())
        sources.append(provenance)
        return target, provenance

    def record_pcm(path, metadata, provenance):
        verify_pcm(path, metadata)
        metadata.update(source=provenance['source_path'], source_sha256=provenance['sha256'],
                        output=str(path.relative_to(output)), pcm_sha256=digest(path),
                        loop_sidecar=str(path.relative_to(output)) + '.loop' if metadata['loop_enabled'] else None)
        records.append(metadata)

    streams = [f for f in info['files'] if f['path'].startswith('sound/stream/') and f['path'].endswith('.stm')]
    for index, item in enumerate(streams, 1):
        raw, provenance = acquire(item['path'])
        pcm = output / 'pcm' / 'streams' / (raw.stem + '.wav')
        record_pcm(pcm, stm(raw, pcm), provenance)
        print(f'Stream {index}/{len(streams)}: {raw.stem}', flush=True)

    banks = {}
    for suffix in BANK_SUFFIXES:
        banks[suffix] = acquire('sound/proj/pmario.' + suffix)
    directory = sample_directory(banks['sdir'][0])
    ids = [s['id'] for s in directory]
    if len(ids) != len(set(ids)):
        raise ValueError('Duplicate sample IDs; refusing ambiguous output names')
    bank = banks['samp'][0].read_bytes()
    looped = [s for s in directory if s['loop'][1] - s['loop'][0] > 1]
    for entry in looped:
        pcm = output / 'pcm' / 'samples' / f"pmario-{entry['id']:04x}.wav"
        metadata = convert_sample(entry, bank, pcm)
        metadata['directory_sha256'] = banks['sdir'][1]['sha256']
        record_pcm(pcm, metadata, banks['samp'][1])
    (output / 'sample-directory.json').write_text(json.dumps(directory, indent=2) + '\n')
    manifest = {
        'schema': 1, 'source_disc': info['disc_id'], 'iso': str(iso),
        'converter_sha256': {p.name: digest(p) for p in (Path(__file__), ROOT / 'tools/audio_convert.py', ROOT / 'tools/iso_assets.py')},
        'loop_units': 'PCM frames; start inclusive, end exclusive; before source-rate resampling',
        'license': 'Original game assets retain original copyright; local derivatives only, no redistribution license granted',
        'stream_count': len(streams), 'sample_bank_count': len(directory), 'looped_sample_count': len(looped),
        'sources': sources, 'audio': records,
        'limitations': 'MusyX macros, envelopes, pitch and game event bindings are not rendered. PCM repeats the decoded region; original ADPCM loop-history execution and audible seams have not been compared.'
    }
    (output / 'manifest.json').write_text(json.dumps(manifest, indent=2) + '\n')
    print(f"Converted and verified {len(streams)} streams and {len(looped)} looped samples -> {output}", flush=True)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--output', type=Path, default=ROOT / 'local-assets/audio')
    args = parser.parse_args()
    build(args.iso, args.output)
