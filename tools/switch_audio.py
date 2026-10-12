#!/usr/bin/env python3
"""Extract Switch 1.0.0 audio from the existing verified RomFS, read-only.

Authored NintendoWare table reader; format facts cross-checked with libpulsar
(MIT) and Citric Composer (no implementation copied). Decoding uses an external
vgmstream CLI (ISC and its dependency licenses). Original files and PCM outputs
belong in ignored local-assets, separate from the GameCube/native asset pack.
"""
import argparse
from concurrent.futures import ThreadPoolExecutor
import hashlib
import json
from pathlib import Path, PurePosixPath
import shutil
import struct
import subprocess
import wave

from extract_audio import digest, verify_pcm

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_DECODER = ROOT / 'artifacts/reference/vgmstream/build/cli/vgmstream-cli'


class NintendoWare:
    def __init__(self, raw, magic):
        self.raw = memoryview(raw)
        if len(raw) < 20 or bytes(raw[:4]) != magic or bytes(raw[4:6]) != b'\xff\xfe':
            raise ValueError(f'Invalid little-endian {magic!r} header')
        if self.u32(12) != len(raw):
            raise ValueError('NintendoWare declared size mismatch')
        self.sections = {}
        for index in range(self.unpack('H', 16)[0]):
            kind, _, offset, size = self.unpack('HHII', 20 + index * 12)
            self.span(offset, size)
            if kind in self.sections or size < 8 or self.u32(offset + 4) != size:
                raise ValueError('Invalid or duplicate section')
            self.sections[kind] = (offset, size)

    def span(self, offset, size):
        if offset < 0 or size < 0 or offset + size > len(self.raw):
            raise ValueError('NintendoWare range outside container')
        return self.raw[offset:offset + size]

    def unpack(self, fmt, offset):
        return struct.unpack('<' + fmt, self.span(offset, struct.calcsize('<' + fmt)))

    def u32(self, offset):
        return self.unpack('I', offset)[0]

    def ref(self, base, offset, expected=None):
        kind, _, delta = self.unpack('HHi', offset)
        if delta == -1:
            return None
        if expected is not None and kind != expected:
            raise ValueError(f'Unexpected reference type {kind:#x}, expected {expected:#x}')
        target = base + delta
        self.span(target, 1)
        return target

    def table(self, offset, kind):
        count = self.u32(offset)
        self.span(offset + 4, count * 8)
        return [self.ref(offset, offset + 4 + index * 8, kind) for index in range(count)]

    def string(self, offset):
        # Strings occur in metadata; bound the terminator search.
        data = bytes(self.span(offset, min(4096, len(self.raw) - offset)))
        end = data.find(b'\0')
        if end < 0:
            raise ValueError('Unterminated archive string')
        return data[:end].decode('utf-8')


def archive_inventory(raw):
    ar = NintendoWare(raw, b'FSAR')
    string_base = ar.sections[0x2000][0] + 8
    table = ar.ref(string_base, string_base, 0x2400)
    strings = []
    for index in range(ar.u32(table)):
        row = table + 4 + index * 12
        target = ar.ref(table, row, 0x1f01)
        text = ar.string(target)
        if len(text.encode('utf-8')) + 1 != ar.u32(row + 8):
            raise ValueError('String table length mismatch')
        strings.append(text)
    info = ar.sections[0x2001][0] + 8
    file_base = ar.sections[0x2002][0] + 8
    files = []
    for index, row in enumerate(ar.table(ar.ref(info, info + 48, 0x2106), 0x220a)):
        kind = ar.unpack('H', row)[0]
        target = ar.ref(row, row)
        record = {'file_id': index, 'entry_offset': row, 'source': 'sound/sound.bfsar'}
        if kind == 0x220d:
            record['external'] = ar.string(target)
        elif kind == 0x220c:
            offset, size = ar.unpack('II', target + 4)
            record.update(offset=None if offset == 0xffffffff else file_base + offset, size=size)
            if record['offset'] is not None:
                ar.span(record['offset'], size)
                # FGRP logical file sizes exclude final 64-byte alignment padding,
                # while their NintendoWare header/section sizes include it.
                if bytes(ar.span(record['offset'], 4)) == b'FGRP':
                    stored = ar.u32(record['offset'] + 12)
                    if stored not in (size, (size + 63) // 64 * 64):
                        raise ValueError('Unexpected group padding')
                    ar.span(record['offset'], stored)
                    record['stored_size'] = stored
        else:
            raise ValueError('Unknown archive file type')
        files.append(record)
    # Resolve group-resident files by their structured location table, not magic scanning.
    for group in list(files):
        if group.get('offset') is None:
            continue
        content = ar.span(group['offset'], group.get('stored_size', group['size']))
        if bytes(content[:4]) != b'FGRP':
            continue
        grp = NintendoWare(content, b'FGRP')
        table = grp.sections[0x7800][0] + 8
        data_base = grp.sections[0x7801][0] + 8
        for row in grp.table(table, 0x7900):
            file_id = grp.u32(row)
            relative, size = grp.unpack('II', row + 8)
            if relative == 0xffffffff or size == 0xffffffff:
                continue
            if grp.unpack('H', row + 4)[0] != 0x1f00 or file_id >= len(files):
                raise ValueError('Invalid group file binding')
            offset = group['offset'] + data_base + relative
            grp.span(data_base + relative, size)
            target = files[file_id]
            if target.get('offset') is not None:
                if bytes(ar.span(target['offset'], target['size'])) != bytes(ar.span(offset, size)):
                    raise ValueError('Conflicting group file copies')
            else:
                target.update(offset=offset, size=size, group_file_id=group['file_id'])
    wave_archives = []
    for index, row in enumerate(ar.table(ar.ref(info, info + 24, 0x2103), 0x2207)):
        flags = ar.u32(row + 8)
        record = {'wave_archive_id': index, 'file_id': ar.u32(row)}
        if flags & 1:
            record['name'] = strings[ar.u32(row + 12)]
        wave_archives.append(record)
    sounds = []
    for index, row in enumerate(ar.table(ar.ref(info, info, 0x2100), 0x2200)):
        kind = ar.unpack('H', row + 12)[0]
        detail = ar.ref(row, row + 12)
        flags = ar.u32(row + 20)
        record = {'sound_id': index, 'file_id': ar.u32(row), 'kind': kind,
                  'entry_offset': row, 'initial_volume': ar.span(row + 8, 1)[0]}
        if flags & 1:
            record['name'] = strings[ar.u32(row + 24)]
        if kind == 0x2201:
            ext = ar.ref(detail, detail + 24, 0x2210)
            if ext is not None:
                stream_type, start, end = ar.unpack('III', ext)
                loop_flag = (stream_type >> 8) & 255
                if loop_flag not in (0, 1):
                    raise ValueError('Unknown stream loop flag')
                record.update(stream_type=stream_type, loop_enabled=bool(loop_flag),
                              loop=[start, end] if loop_flag else None, extension_offset=ext)
        elif kind == 0x2202:
            record['wave_sound_index'] = ar.u32(detail)
        elif kind != 0x2203:
            raise ValueError('Unknown sound info type')
        sounds.append(record)
    return {'files': files, 'sounds': sounds, 'wave_archives': wave_archives}


def bind_external_groups(inventory, romfs):
    """Resolve the game's counted offset/size wrappers around external FGRPs."""
    inventory['group_variants'] = []
    for source in sorted((romfs / 'sound/userManagementFiles').glob('*.bfgrp.bin')):
        raw = source.read_bytes()
        count = struct.unpack_from('<I', raw)[0]
        if count > (len(raw) - 4) // 8:
            raise ValueError('Invalid external group count')
        for index in range(count):
            offset, logical_size = struct.unpack_from('<II', raw, 4 + index * 8)
            if offset < 4 + count * 8 or offset + logical_size > len(raw):
                raise ValueError('Invalid external group bounds')
            declared = struct.unpack_from('<I', raw, offset + 12)[0]
            if not logical_size <= declared <= logical_size + 63 or offset + declared > len(raw):
                raise ValueError('Invalid external group padding')
            grp = NintendoWare(memoryview(raw)[offset:offset + declared], b'FGRP')
            table = grp.sections[0x7800][0] + 8
            data_base = grp.sections[0x7801][0] + 8
            for row in grp.table(table, 0x7900):
                file_id = grp.u32(row)
                relative, size = grp.unpack('II', row + 8)
                if relative == 0xffffffff or size == 0xffffffff:
                    continue
                if grp.unpack('H', row + 4)[0] != 0x1f00 or file_id >= len(inventory['files']):
                    raise ValueError('Invalid external group binding')
                content = grp.span(data_base + relative, size)
                record = inventory['files'][file_id]
                if record.get('offset') is not None:
                    with (romfs / record['source']).open('rb') as previous:
                        previous.seek(record['offset']); old = previous.read(record['size'])
                    if old != bytes(content):
                        # Some user-managed groups use their own file-ID domain.
                        # Preserve the contextual resource rather than replacing
                        # a differently typed or differently encoded global file.
                        inventory['group_variants'].append({
                            'file_id': file_id, 'source': str(source.relative_to(romfs)),
                            'offset': offset + data_base + relative, 'size': size,
                            'external_group_index': index, 'variant_id': len(inventory['group_variants'])})
                else:
                    record.update(source=str(source.relative_to(romfs)), offset=offset + data_base + relative,
                                  size=size, external_group_index=index)


def wave_members(raw):
    ar = NintendoWare(raw, b'FWAR')
    table = ar.sections[0x6800][0] + 8
    data_base = ar.sections[0x6801][0] + 8
    count = ar.u32(table)
    ar.span(table + 4, count * 12)
    records = []
    for index in range(count):
        row = table + 4 + index * 12
        kind, _, offset, size = ar.unpack('HHII', row)
        if kind != 0x1f00:
            raise ValueError('Invalid wave member reference')
        offset += data_base
        content = ar.span(offset, size)
        NintendoWare(content, b'FWAV')
        records.append({'wave_id': index, 'offset': offset, 'size': size})
    return records


def safe_path(text):
    path = PurePosixPath(text)
    if not text or path.is_absolute() or '\\' in text or '..' in path.parts:
        raise ValueError('Unsafe archive filename')
    return Path(*path.parts)


def decode_job(job, decoder, output, verify_existing=False):
    source, relative, provenance, archive_loop = job
    target = output / 'pcm' / relative.with_suffix('.wav')
    target.parent.mkdir(parents=True, exist_ok=True)
    metadata = json.loads(subprocess.run([str(decoder), '-m', '-I', str(source)],
                          check=True, capture_output=True, text=True).stdout)
    if not verify_existing:
        subprocess.run([str(decoder), '-i', '-W', '1', '-o', str(target), str(source)],
                       check=True, capture_output=True, text=True)
    looping = metadata.get('loopingInfo')
    loop = archive_loop if archive_loop is not None else (
        [looping['start'], looping['end']] if looping else None)
    record = {**provenance, 'output': str(target.relative_to(output)),
              'rate': metadata['sampleRate'], 'channels': metadata['channels'],
              'samples': metadata['numberOfSamples'], 'encoding': metadata['encoding'],
              'loop': loop, 'loop_enabled': loop is not None}
    sidecar = Path(str(target) + '.loop')
    if loop is not None:
        if not 0 <= loop[0] < loop[1] <= record['samples']:
            raise ValueError(f'Invalid source loop for {source}')
        sidecar.write_bytes(struct.pack('<II', *loop))
    else:
        sidecar.unlink(missing_ok=True)
    verify_pcm(target, record)
    record['pcm_sha256'] = digest(target)
    record['loop_sidecar'] = str(sidecar.relative_to(output)) if loop is not None else None
    return record


def build(romfs, output, decoder, workers, verify_existing=False):
    romfs, output, decoder = romfs.resolve(), output.resolve(), decoder.resolve()
    if output == romfs or output in romfs.parents or romfs in output.parents:
        raise ValueError('Audio derivatives must be separate from source RomFS')
    sound = romfs / 'sound'
    output.mkdir(parents=True, exist_ok=True)
    sources = []
    # Preserve the entire sound tree, including event data and vibration resources.
    for source in sorted(p for p in sound.rglob('*') if p.is_file()):
        relative = source.relative_to(romfs)
        target = output / 'raw' / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(source, target)
        source_hash = digest(source)
        if digest(target) != source_hash:
            raise ValueError('Raw copy verification failed')
        sources.append({'source': str(relative), 'size': source.stat().st_size, 'sha256': source_hash})
    raw = (sound / 'sound.bfsar').read_bytes()
    inventory = archive_inventory(raw)
    bind_external_groups(inventory, romfs)
    ar = NintendoWare(raw, b'FSAR')
    jobs, extracted = [], []
    for record in inventory['files'] + inventory['group_variants']:
        if record.get('offset') is None:
            continue
        if record['source'] == 'sound/sound.bfsar':
            content = ar.span(record['offset'], record.get('stored_size', record['size']))
        else:
            with (romfs / record['source']).open('rb') as source:
                source.seek(record['offset']); content = source.read(record['size'])
        magic = bytes(content[:4]).decode('ascii')
        stem = f"file-{record['file_id']:04d}" if 'variant_id' not in record else f"group-variant-{record['variant_id']:04d}-file-{record['file_id']:04d}"
        relative = Path('banks') / f"{stem}.{magic.lower()}"
        target = output / 'raw' / relative
        target.parent.mkdir(parents=True, exist_ok=True); target.write_bytes(content)
        extracted.append({**record, 'magic': magic, 'output': str(target.relative_to(output)), 'sha256': digest(target)})
        if magic == 'FWAR':
            members = wave_members(content)
            record['wave_count'] = len(members)
            for member in members:
                relative = Path('waves') / stem / f"wave-{member['wave_id']:04d}.bfwav"
                path = output / 'raw' / relative; path.parent.mkdir(parents=True, exist_ok=True)
                path.write_bytes(content[member['offset']:member['offset'] + member['size']])
                jobs.append((path, relative, {'source': record['source'], 'file_id': record['file_id'],
                             'wave_id': member['wave_id'], 'source_offset': record['offset'] + member['offset'],
                             'source_size': member['size'], 'source_sha256': digest(path)}, None))
    streams = {}
    for sound_record in inventory['sounds']:
        file_record = inventory['files'][sound_record['file_id']]
        if sound_record['kind'] != 0x2201:
            continue
        name = file_record['external']
        loop = sound_record.get('loop')
        if name in streams and streams[name]['loop'] != loop:
            raise ValueError('Conflicting stream loop metadata')
        streams.setdefault(name, {'loop': loop, 'sounds': []})['sounds'].append(sound_record)
    opus_files = sorted(sound.rglob('*.opus'))
    resolved = {}
    for name in streams:
        relative = safe_path(name)
        if not (sound / relative).is_file():
            matches = [p for p in opus_files if p.name == relative.name]
            if len(matches) != 1:
                raise ValueError(f'Ambiguous or missing archive stream: {name}')
            relative = matches[0].relative_to(sound)
        resolved[name] = relative
    if {p.relative_to(sound) for p in opus_files} != set(resolved.values()):
        raise ValueError('External stream census differs from archive stream bindings')
    for name, stream in sorted(streams.items()):
        relative = resolved[name]
        source = output / 'raw' / 'sound' / relative
        jobs.append((source, relative, {'source': str(Path('sound') / relative),
                     'source_sha256': digest(source), 'sound_ids': [s['sound_id'] for s in stream['sounds']],
                     'sound_names': [s.get('name') for s in stream['sounds']], 'archive_external_path': name,
                     'loop_source': 'BFSAR stream extension'}, stream['loop']))
    (output / 'archive-inventory.json').write_text(json.dumps(inventory, indent=2) + '\n')
    print(f"Preserved {len(sources)} sound files; extracted {len(extracted)} archive files; decoding {len(jobs)} assets", flush=True)
    results = []
    with ThreadPoolExecutor(max_workers=workers) as pool:
        for index, record in enumerate(pool.map(lambda job: decode_job(job, decoder, output, verify_existing), jobs), 1):
            results.append(record)
            if index % 100 == 0 or index == len(jobs):
                print(f'Decoded and verified {index}/{len(jobs)}', flush=True)
    manifest = {'schema': 1, 'source_platform': 'Nintendo Switch', 'source_version': '1.0.0 base',
                'romfs': str(romfs), 'sources': sources, 'archive_files': extracted, 'audio': results,
                'stream_count': len(streams), 'wave_count': len(jobs) - len(streams),
                'converter_sha256': digest(Path(__file__)), 'decoder': str(decoder), 'decoder_sha256': digest(decoder),
                # vgmstream's informational -V prints valid JSON but returns 1.
                'decoder_version': json.loads(subprocess.run([str(decoder), '-V'], capture_output=True, text=True).stdout),
                'validation_mode': 'existing WAV metadata/data-length/hash audit' if verify_existing else 'decoded and verified',
                'loop_units': 'PCM frames [start,end), original source rate',
                'license': 'Original game copyright retained; local derivatives only, no redistribution rights granted',
                'limitations': 'Decoded source assets, not runtime mixes. Sound IDs/names and raw FWSD/group/control data retained; event mixing, envelopes, pitch and layered wave-sound playback are not rendered. No audible or hardware equivalence claim.'}
    (output / 'manifest.json').write_text(json.dumps(manifest, indent=2) + '\n')
    print(f"Completed: {len(streams)} streams and {manifest['wave_count']} bank waves -> {output}", flush=True)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--romfs', type=Path, default=ROOT / 'local-assets/switch/romfs')
    parser.add_argument('--output', type=Path, default=ROOT / 'local-assets/switch/audio')
    parser.add_argument('--decoder', type=Path, default=DEFAULT_DECODER)
    parser.add_argument('--workers', type=int, default=4)
    parser.add_argument('--verify-existing', action='store_true',
                        help='Audit existing WAVs against source metadata and regenerate the manifest without decoding')
    args = parser.parse_args()
    if not 1 <= args.workers <= 16:
        parser.error('--workers must be between 1 and 16')
    build(args.romfs, args.output, args.decoder, args.workers, args.verify_existing)
