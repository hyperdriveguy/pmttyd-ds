#!/usr/bin/env python3
"""Catalog sound identities/dependencies; package the slice's GameCube PCM fallbacks.

Categories are name-derived implementation hints, never recovered story conditions.
No ISO code is executed. Original indexes and PCM stay in ignored local-assets/RomFS.
"""
import argparse
import csv
import hashlib
import json
from pathlib import Path
import struct

from audio_convert import sample, stm, sample_directory
from iso_assets import DEFAULT_ISO, inventory, extract
from switch_audio import NintendoWare

ROOT = Path(__file__).resolve().parents[1]


def category(name):
    name = name.upper()
    if 'MSG' in name or 'MESSAGE' in name or 'WINDOW' in name:
        return 'dialogue'
    if 'ENV' in name:
        return 'ambience'
    if name.startswith(('BGM_STG', 'BGM_M_STG', 'BGM_O_STG', 'STG_', 'BGM_MAP')):
        return 'overworld'
    if 'BTL' in name or 'BATTLE' in name or name.startswith(('SFX_AC_', 'SFX_CONDITION_')):
        return 'battle'
    if name.startswith(('SFX_STG', 'SFX_END', 'SFX_OPN')):
        return 'event'
    if name.startswith(('SFX_ENM', 'SFX_BOSS')):
        return 'battle'
    if name.startswith(('SFX_ITEM', 'SE_ITEM')):
        return 'item'
    if 'EVT' in name or name.startswith(('FF_', 'BGM_EVT')):
        return 'event'
    if any(s in name for s in ('CURSOR', 'SELECT', 'DECIDE', 'SYS_', 'FILE_', 'MENU')):
        return 'interface'
    if any(s in name for s in ('MARIO', 'MAR_', 'FLD', 'MOBJ', 'VOICE')):
        return 'field-character'
    return 'unclassified'


def project_effects(raw):
    """Original absolute group/table offsets; FX IDs differ from macro/sample IDs."""
    effects = {}; pos = 0
    while pos + 4 <= len(raw):
        end = struct.unpack_from('>I', raw, pos)[0]
        if end == 0xffffffff:
            return effects
        if not pos + 40 <= end <= len(raw):
            raise ValueError('Invalid MusyX group extent')
        group, kind = struct.unpack_from('>HH', raw, pos + 4)
        if kind == 1:
            table = struct.unpack_from('>I', raw, pos + 28)[0]
            count = struct.unpack_from('>H', raw, table)[0]
            if not pos + 40 <= table or table + 4 + count * 10 > end:
                raise ValueError('Invalid MusyX FX table')
            for i in range(count):
                fx, obj, priority, voices, velocity, pan, key, _ = struct.unpack_from('>HH6B', raw, table + 4 + i * 10)
                effects.setdefault(fx, []).append(dict(group_id=group, object_id=obj,
                    priority=priority, voices=voices, velocity=velocity, pan=pan, key=key))
        elif kind != 0:
            raise ValueError('Unknown MusyX group type')
        pos = end
    raise ValueError('Missing MusyX project terminator')


def macro_samples(raw):
    pos, end = struct.unpack_from('>II', raw)
    result = {}
    while pos + 4 <= end:
        size = struct.unpack_from('>I', raw, pos)[0]
        if size == 0xffffffff:
            return result
        if size < 8 or (size - 8) % 8 or pos + size > end:
            raise ValueError('Invalid MusyX macro extent')
        mid = struct.unpack_from('>H', raw, pos + 4)[0]
        # Opcode occupies low byte of the first BE word; STARTSAMPLE operand
        # occupies its middle halfword. These are direct references, not execution.
        refs = sorted({struct.unpack_from('>H', raw, off + 1)[0]
                       for off in range(pos + 8, pos + size, 8) if raw[off + 3] == 0x10})
        result[mid] = dict(sample_ids=refs, offset=pos, size=size,
            opcodes=[raw[o + 3] for o in range(pos + 8, pos + size, 8)])
        pos += size
    raise ValueError('Missing macro terminator')


def gamecube_tables(iso):
    info = inventory(iso)
    if info['disc_id'] != 'G8ME01':
        raise ValueError('US G8ME01 required')
    with Path(iso).open('rb') as disc:
        disc.seek(0); header = disc.read(0x440)
        if header[7] != 0:
            raise ValueError('Version-specific tables require revision 0')
        dol = struct.unpack_from('>I', header, 0x420)[0]
        disc.seek(dol); h = disc.read(256)
        sections = [(struct.unpack_from('>I', h, i * 4)[0],
                     struct.unpack_from('>I', h, 0x48 + i * 4)[0],
                     struct.unpack_from('>I', h, 0x90 + i * 4)[0]) for i in range(18)]
        def read(va, size):
            for off, base, length in sections:
                if base <= va and va + size <= base + length:
                    disc.seek(dol + off + va - base)
                    data = disc.read(size)
                    if len(data) == size: return data
            raise ValueError(f'Unmapped DOL table address {va:#x}')
        def string(va):
            # Empty string is a zero-filled BSS sentinel in this executable.
            if va == 0x804218c8: return ''
            out = bytearray()
            for i in range(256):
                c = read(va + i, 1)
                if c == b'\0': return out.decode('shift_jis')
                out.extend(c)
            raise ValueError('Unterminated sound name')
        rows = []; spans = []
        for table, base, size, stride in [('bgm', 0x80315ac8, 0x1060, 16), ('sfx', 0x80316b28, 0x14b68, 20)]:
            raw = read(base, size)
            spans.append(dict(table=table, address=base, size=size, sha256=hashlib.sha256(raw).hexdigest()))
            for off in range(0, size, stride):
                nameptr, flags, streamptr = struct.unpack_from('>III', raw, off)
                name = string(nameptr)
                if not name: continue
                stream = string(streamptr)
                rows.append(dict(platform='gamecube', table=table, table_index=off // stride,
                    address=base + off, name=name, flags=flags, fx_id=flags & 0xffff if table == 'sfx' else None,
                    stream_alias=stream, category=category(name), parameters=list(raw[off + 12:off + stride]),
                    trigger_evidence='name category only; event condition and ordering require original caller binding'))
    return rows, spans, info


def switch_dependencies(root):
    ar = json.loads((root / 'archive-inventory.json').read_text())
    manifest = json.loads((root / 'manifest.json').read_text())
    files = {r['file_id']: r for r in manifest['archive_files'] if 'variant_id' not in r}
    wars = {r['wave_archive_id']: r['file_id'] for r in ar['wave_archives']}
    waves = {(r['file_id'], r['wave_id']): r for r in manifest['audio'] if 'wave_id' in r and 'variant_id' not in r}
    streams = {sid: r for r in manifest['audio'] for sid in r.get('sound_ids', [])}
    cache = {}; rows = []
    for sound in ar['sounds']:
        row = {**sound, 'platform': 'switch', 'category': category(sound.get('name', '')),
               'trigger_evidence': 'archive identity/name only; original caller conditions unresolved'}
        if sound['kind'] == 0x2201:
            row['pcm'] = streams[sound['sound_id']]['output']
        elif sound['kind'] == 0x2202:
            fid = sound['file_id']
            if fid not in cache:
                nw = NintendoWare((root / files[fid]['output']).read_bytes(), b'FWSD')
                base = next(iter(nw.sections.values()))[0] + 8
                wave_table = nw.ref(base, base)
                pairs = [nw.unpack('II', wave_table + 4 + i * 8) for i in range(nw.u32(wave_table))]
                items = nw.table(nw.ref(base, base + 8), 0x4900)
                cache[fid] = (nw, pairs, items)
            nw, pairs, items = cache[fid]
            item = items[sound['wave_sound_index']]
            notes_table = nw.ref(item, item + 16)
            deps = []
            for note in nw.table(notes_table, 0x4902):
                war, wid = pairs[nw.u32(note)]
                war &= 0xffffff
                file_id = wars[war]
                pcm = waves[(file_id, wid)]
                deps.append(dict(wave_archive_id=war, file_id=file_id, wave_id=wid, pcm=pcm['output']))
            row['wave_dependencies'] = deps
            row['group_variants'] = [r for r in ar.get('group_variants', []) if r['file_id'] == fid or any(r['file_id'] == d['file_id'] for d in deps)]
            row['playback'] = 'dependencies only; FWSD tracks, pitch/envelopes/layering not rendered'
        rows.append(row)
    return rows


def build(iso, output, romfs=None, switch=None):
    iso = Path(iso).resolve()
    for target in (output, romfs, switch):
        if target is not None and (target.resolve() == iso or target.resolve() in iso.parents):
            raise ValueError('Output/reference directory must be separate from ISO')
    if romfs is not None:
        romfs.mkdir(parents=True, exist_ok=True)
    output.mkdir(parents=True, exist_ok=True)
    rawdir = output / 'raw'; rawdir.mkdir(exist_ok=True)
    for suffix in ('proj', 'pool', 'sdir', 'samp'):
        extract(iso, 'sound/proj/pmario.' + suffix, rawdir / ('pmario.' + suffix))
    effects = project_effects((rawdir / 'pmario.proj').read_bytes())
    macros = macro_samples((rawdir / 'pmario.pool').read_bytes())
    rows, spans, inv = gamecube_tables(iso)
    streams = {Path(f['path']).stem: f['path'] for f in inv['files'] if f['path'].startswith('sound/stream/')}
    for row in rows:
        alias = Path(row['stream_alias']).name
        if alias in streams: row['stream_source'] = streams[alias]
        if row['table'] == 'sfx' and row['flags'] & (1 << 29):
            row['objects'] = [{**fx, 'direct_macro': macros.get(fx['object_id'])} for fx in effects.get(row['fx_id'], [])]
    bindings = json.loads((ROOT / 'config/audio_bindings.json').read_text())
    byname = {r['name']: r for r in rows}
    if [r['clip'] for r in bindings['effects']] != ['confirm', 'jump', 'hit', 'success', 'guard', 'save-block', 'goomba-step-left', 'goomba-step-right']:
        raise ValueError('Cue filenames/order must match native AudioCue')
    selected = []
    for binding in bindings['effects']:
        row = byname[binding['gamecube_name']]
        objs = row.get('objects', [])
        if len(objs) != 1 or not objs[0]['direct_macro'] or len(objs[0]['direct_macro']['sample_ids']) != 1:
            raise ValueError('Slice fallback requires exactly one direct sample: ' + row['name'])
        sid = objs[0]['direct_macro']['sample_ids'][0]
        target = (romfs if romfs else output / 'pcm') / (binding['clip'] + '.wav')
        pcm = sample(rawdir / 'pmario.sdir', rawdir / 'pmario.samp', sid, target)
        # Slice effects are bounded one-shots. Preserve source loop metadata in
        # catalog but do not install looping effect sidecars into the native pack.
        Path(str(target) + '.loop').unlink(missing_ok=True)
        selected.append({**binding, 'fx_id': row['fx_id'], 'object_id': objs[0]['object_id'],
            'sample_id': sid, 'pcm': pcm, 'volume': row['parameters'][0],
            'playback': 'GameCube primary-sample fallback; macro modulation/child voices not rendered'})
    music_dir = romfs if romfs else output / 'pcm'
    for binding in bindings['music']:
        if not binding['source']: continue
        source = rawdir / (binding['clip'] + '.stm')
        extract(iso, binding['source'], source)
        stm(source, music_dir / (binding['clip'] + '.wav'))
    switchrows = switch_dependencies(switch) if switch else []
    catalog = dict(schema=1, source_disc='G8ME01 revision 0', evidence=spans,
        gamecube_samples=[{**r, 'library_pcm': f"local-assets/audio/pcm/samples/pmario-{r['id']:04x}.wav"} for r in sample_directory(rawdir / 'pmario.sdir')],
        gamecube=rows, switch=switchrows, slice=selected, music=bindings['music'],
        policy='Named Switch references may guide implementations; ambiguous cases use original GameCube identities. Never equate FX/macro/sample IDs.',
        license='Original assets retain copyright; local derivatives only; no redistribution rights granted')
    (output / 'catalog.json').write_text(json.dumps(catalog, indent=2) + '\n')
    with (output / 'catalog.csv').open('w', newline='') as out:
        writer = csv.writer(out)
        writer.writerow(['platform', 'category', 'name', 'sound_or_table_id', 'source_or_pcm', 'dependencies', 'evidence'])
        for row in rows + switchrows:
            writer.writerow([row['platform'], row['category'], row['name'], row.get('sound_id', row.get('table_index')),
                row.get('pcm', row.get('stream_source', row.get('stream_alias', ''))),
                json.dumps(row.get('wave_dependencies', row.get('objects', []))), row['trigger_evidence']])
    if romfs:
        (romfs / 'audio-map.json').write_text(json.dumps({'schema': 1, 'effects': selected, 'music': bindings['music']}, indent=2) + '\n')
    print(f"Mapped {len(rows)} GameCube and {len(switchrows)} Switch sounds; packed {len(selected)} slice fallbacks")


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--iso', default=DEFAULT_ISO)
    p.add_argument('--output', type=Path, default=ROOT / 'local-assets/audio-map')
    p.add_argument('--romfs', type=Path)
    p.add_argument('--switch', type=Path)
    args = p.parse_args()
    build(args.iso, args.output, args.romfs, args.switch)
