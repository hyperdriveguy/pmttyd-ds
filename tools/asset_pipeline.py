#!/usr/bin/env python3
"""Editable source assets -> native RomFS, with reversible original-pack overlays."""
import argparse
import hashlib
import json
import math
from pathlib import Path, PurePosixPath
import shutil
import struct
import subprocess
import tempfile
import wave
import sys
import re
import PIL

from PIL import Image
from asset_animation import decode as decode_animation, encode as encode_animation

ROOT = Path(__file__).resolve().parents[1]
KINDS = {'texture', 'atlas', 'model', 'actor', 'sfx', 'music', 'animation', 'pack'}


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def write_json(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    text = json.dumps(value, indent=2, allow_nan=False)
    # Keep numeric rows on one line for reviewable mesh/key diffs.
    number = r'-?\d+(?:\.\d+)?(?:[eE][+-]?\d+)?'
    text = re.sub(r'\[\s*(' + number + r'(?:\s*,\s*' + number + r')*)\s*\]',
                  lambda m: '[' + ', '.join(v.strip() for v in m.group(1).split(',')) + ']', text)
    path.write_text(text + '\n')


def contained(root, name):
    rel = PurePosixPath(name)
    if not name or not rel.parts or str(rel) != name or rel.is_absolute() or '..' in rel.parts or '\\' in name:
        raise ValueError(f'Expected a relative asset path: {name}')
    path = root / str(rel)
    if not path.resolve().is_relative_to(root.resolve()):
        raise ValueError(f'Asset path escapes its root: {name}')
    return path


def integer(value, low, high):
    if type(value) is not int or not low <= value <= high:
        raise ValueError(f'Expected integer in {low}..{high}: {value}')
    return value


def floats(row, size):
    if not isinstance(row, list) or len(row) != size:
        raise ValueError(f'Expected {size} vertex components')
    if any(type(x) not in (int, float) or not math.isfinite(x) or abs(x) > 3.4e38 for x in row):
        raise ValueError('Vertex components must be finite float32 values')
    return row


def read_manifest(path, root):
    doc = json.loads(path.read_text())
    if doc.get('schema') != 1 or not isinstance(doc.get('assets'), list):
        raise ValueError('Asset manifest requires schema:1 and assets:[]')
    ids = set(); outputs = set()
    for entry in doc['assets']:
        aid = entry.get('id')
        if not isinstance(aid, str) or not aid or aid in ids:
            raise ValueError(f'Missing or duplicate asset ID: {aid}')
        ids.add(aid)
        if entry.get('kind') not in KINDS or not entry.get('license'):
            raise ValueError(f'{aid}: kind and license are required')
        if 'inherit_original_visibility' in entry and (entry['kind'] != 'model' or type(entry['inherit_original_visibility']) is not bool):
            raise ValueError(f'{aid}: visibility inheritance requires a model boolean')
        source = contained(root, entry['source'])
        if not source.is_file():
            raise ValueError(f'{aid}: source missing: {source}')
        names = output_names(entry)
        for name in names:
            contained(root, name)
            if name == 'asset-pipeline-stamp.json':
                raise ValueError('Output name is reserved for the build stamp')
            if name in outputs:
                raise ValueError(f'Duplicate output: {name}')
            outputs.add(name)
    return doc


def output_names(entry):
    name = entry['output']; kind = entry['kind']
    suffix = {'texture': '.t3x', 'atlas': '.t3x', 'actor': '.pmmesh', 'animation': '.pmanim', 'sfx': '.wav', 'music': '.wav'}
    if kind in suffix and not name.endswith(suffix[kind]):
        raise ValueError(f'{entry["id"]}: output must end in {suffix[kind]}')
    if kind == 'model':
        return [name + '.vertices', name + '.batches']
    if kind in ('sfx', 'music'):
        return [name, name + '.loop']
    if kind == 'atlas':
        return [name, name + '.atlas.json']
    return [name]


def compile_texture(source, entry, target, tex3ds):
    with Image.open(source) as image:
        width, height = image.size
    if any(n < 8 or n > 1024 or n & (n - 1) for n in (width, height)):
        raise ValueError('Native textures require power-of-two dimensions, 8..1024')
    fmt = entry.get('format', 'rgba8')
    if fmt not in ('rgba8', 'rgb565'):
        raise ValueError('Supported texture formats: rgba8, rgb565')
    subprocess.run([tex3ds, '-f', fmt, '-z', 'auto', '-o', str(target), str(source)],
                   check=True, stdout=subprocess.DEVNULL)


def make_atlas(source, root, stage):
    doc = json.loads(source.read_text())
    width, height = doc['size']
    for n in (width, height):
        integer(n, 8, 1024)
        if n & (n - 1):
            raise ValueError('Atlas size must be power-of-two')
    padding = integer(doc.get('padding', 2), 0, 16)
    image = Image.new('RGBA', (width, height)); used = []; frames = {}
    for frame in doc['frames']:
        name = frame['name']
        if not name or name in frames:
            raise ValueError('Atlas frame names must be unique')
        sprite_path = contained(root, frame['source'])
        with Image.open(sprite_path) as sprite:
            sprite = sprite.convert('RGBA')
        x, y = frame['position']; integer(x, 0, width); integer(y, 0, height)
        w, h = sprite.size
        box = (x - padding, y - padding, x + w + padding, y + h + padding)
        if box[0] < 0 or box[1] < 0 or box[2] > width or box[3] > height:
            raise ValueError(f'Atlas frame outside image: {name}')
        if any(box[0] < b[2] and box[2] > b[0] and box[1] < b[3] and box[3] > b[1] for b in used):
            raise ValueError(f'Overlapping padded atlas frame: {name}')
        used.append(box)
        # Edge extrusion preserves authored pixels and prevents filtering bleed.
        tile = Image.new('RGBA', (w + 2 * padding, h + 2 * padding))
        tile.paste(sprite, (padding, padding))
        if padding:
            tile.paste(sprite.crop((0, 0, 1, h)).resize((padding, h)), (0, padding))
            tile.paste(sprite.crop((w - 1, 0, w, h)).resize((padding, h)), (padding + w, padding))
            tile.paste(tile.crop((0, padding, tile.width, padding + 1)).resize((tile.width, padding)), (0, 0))
            tile.paste(tile.crop((0, padding + h - 1, tile.width, padding + h)).resize((tile.width, padding)), (0, padding + h))
        image.paste(tile, box[:2])
        frames[name] = {'rect': [x, y, w, h], 'uv': [x / width, 1 - (y + h) / height, (x + w) / width, 1 - y / height]}
    if not frames:
        raise ValueError('Atlas must contain frames')
    path = stage / 'atlas.png'; image.save(path)
    return path, {'size': [width, height], 'padding': padding, 'frames': frames}


def model_from_obj(source, entry):
    """Blender triangulated OBJ: native Y-up coordinates, explicit UVs/materials."""
    positions = []; uvs = []; vertices = []; batches = []
    materials = entry.get('materials', {})
    current = None
    scale = entry.get('scale', 1)
    if type(scale) not in (int, float) or not math.isfinite(scale) or scale <= 0:
        raise ValueError('OBJ scale must be finite and positive')
    def idx(index, values):
        number = int(index)
        if not number or not -len(values) <= number <= len(values):
            raise ValueError('OBJ index out of range')
        return values[number - 1 if number > 0 else number]
    for line in source.read_text().splitlines():
        parts = line.split('#', 1)[0].split()
        if not parts:
            continue
        key = parts[0]
        if key == 'v':
            positions.append(floats([float(v) for v in parts[1:]], 3))
        elif key == 'vt':
            uvs.append(floats([float(v) for v in parts[1:]], 2))
        elif key == 'usemtl':
            current = ' '.join(parts[1:])
        elif key == 'f':
            if len(parts) != 4 or current not in materials:
                raise ValueError('OBJ requires triangulated faces and mapped usemtl names')
            mat = materials[current]
            color = floats(mat.get('color', [1, 1, 1, 1]), 4)
            metadata = [mat['texture'], mat.get('flags', 0), mat.get('wrap_s', 0), mat.get('wrap_t', 0)]
            if not batches or batches[-1][2:] != metadata:
                batches.append([len(vertices), 0, *metadata])
            for token in parts[1:]:
                indices = token.split('/')
                if len(indices) < 2 or not indices[1]:
                    raise ValueError('OBJ face requires UVs')
                p = idx(indices[0], positions); uv = idx(indices[1], uvs)
                vertices.append([*(v * scale for v in p), *uv, *color])
                batches[-1][1] += 1
    return {'schema': 1, 'texture_count': entry['texture_count'], 'vertices': vertices, 'batches': batches}


def inherit_model_visibility(doc, original_prefix):
    """Retain source event ownership across editable triangle/batch ordering.

    Match float32 positions, leaving UV, color, texture and geometry edits intact.
    New geometry keeps its authored flags; ambiguous source ownership fails closed.
    """
    _model_payload(doc, original_prefix) # Validate authored domains before rewriting.
    original = Path(str(original_prefix) + '.vertices').read_bytes()
    packed = Path(str(original_prefix) + '.batches').read_bytes()
    count, _ = struct.unpack_from('<II', packed)
    def key(rows):
        return tuple(sorted(struct.pack('<3f', *row[:3]) for row in rows))
    ownership = {}
    for i in range(count):
        start, size, _, flags, _, _ = struct.unpack_from('<IIiIII', packed, 8 + i * 24)
        group = flags >> 26
        if not group:
            continue
        for n in range(start, start + size, 3):
            identity = key([struct.unpack_from('<3f', original, (n + j) * 36) for j in range(3)])
            previous = ownership.setdefault(identity, group)
            if previous != group:
                raise ValueError('Ambiguous original event ownership for editable triangle')
    batches = []
    vertices = doc['vertices']
    for start, size, tid, flags, ws, wt in doc['batches']:
        for n in range(start, start + size, 3):
            group = ownership.get(key(vertices[n:n + 3]))
            inherited = flags if group is None else (flags & 0x03ffffff) | (group << 26)
            row = [n, 3, tid, inherited, ws, wt]
            if batches and batches[-1][0] + batches[-1][1] == n and batches[-1][2:] == row[2:]:
                batches[-1][1] += 3
            else:
                batches.append(row)
    return dict(doc, batches=batches)


def _model_payload(doc, prefix):
    if doc.get('schema') != 1:
        raise ValueError('Model requires schema:1')
    vertices = doc['vertices']; batches = doc['batches']
    count = integer(doc['texture_count'], 0, 256)
    if not vertices or len(vertices) * 36 > 16 * 1024 * 1024 or len(vertices) % 3 or len(batches) > 512:
        raise ValueError('Model exceeds native triangle/size/batch limits')
    payload = b''.join(struct.pack('<9f', *floats(row, 9)) for row in vertices)
    records = []
    max_event_group = 14 if prefix.name == 'opening-gor_00' else 13 if prefix.name == 'opening-muj_20' else 13 if prefix.name == 'gor_02' else 12
    for b in batches:
        if len(b) != 6:
            raise ValueError('Batch requires start,count,texture,flags,wrapS,wrapT')
        start, size, tid, flags, ws, wt = b
        integer(start, 0, len(vertices)); integer(size, 0, len(vertices) - start)
        integer(tid, -1, count - 1); integer(flags, 0, 0xffffffff)
        integer(ws, 0, 2); integer(wt, 0, 2)
        if start % 3 or size % 3 or (flags >> 8 & 65535) > 1024 or flags >> 26 > max_event_group:
            raise ValueError('Batch violates native triangle/visibility domain')
        records.append(struct.pack('<IIiIII', *b))
    return payload, struct.pack('<II', len(batches), count) + b''.join(records)


def compile_model(doc, prefix):
    payload, batches = _model_payload(doc, prefix)
    Path(str(prefix) + '.vertices').write_bytes(payload)
    Path(str(prefix) + '.batches').write_bytes(batches)


def compile_actor(doc, target):
    poses = doc['poses']; integer(len(poses), 1, 64); names = set()
    raw = bytearray(struct.pack('<8sI', b'PMCHAR1\0', len(poses)))
    for pose in poses:
        name = pose['name']; encoded = name.encode('ascii'); vertices = pose['vertices']
        if not encoded or len(encoded) >= 32 or '\0' in name or name in names:
            raise ValueError('Actor requires unique ASCII pose names of 1..31 bytes')
        names.add(name); integer(len(vertices), 3, 8192)
        if len(vertices) % 3:
            raise ValueError('Actor poses require triangles')
        raw.extend(struct.pack('<32sI', encoded, len(vertices)))
        for row in vertices:
            floats(row, 5)
            if not all(0 <= v <= 1 for v in row[3:]):
                raise ValueError('Actor UVs must be in 0..1')
            raw.extend(struct.pack('<5f', *row))
    target.write_bytes(raw)


def compile_audio(source, entry, target):
    with wave.open(str(source), 'rb') as stream:
        channels, width, rate, frames = stream.getparams()[:4]
        if channels not in (1, 2) or width != 2 or not 8000 <= rate <= 48000 or not frames:
            raise ValueError('Audio requires PCM16 WAV, mono/stereo, 8000..48000 Hz')
        pcm = stream.readframes(frames)
        if len(pcm) != frames * channels * 2:
            raise ValueError('Truncated PCM source')
    # Emit canonical 44-byte WAV header required by the native reader.
    with wave.open(str(target), 'wb') as out:
        out.setparams((channels, width, rate, frames, 'NONE', 'not compressed'))
        out.writeframes(pcm)
    loop = entry.get('loop')
    if loop is not None:
        if len(loop) != 2:
            raise ValueError('Loop requires [start,end] sample frames')
        integer(loop[0], 0, frames - 1); integer(loop[1], loop[0] + 1, frames)
        Path(str(target) + '.loop').write_bytes(struct.pack('<II', *loop))


def prepare(output, work):
    state = work / 'installed.json'
    if not state.exists():
        return
    doc = json.loads(state.read_text())
    # Do not overwrite a pack regenerated by another converter since our install.
    for record in doc['outputs']:
        if record['had_original']:
            backup = contained(work / 'baseline', record['path'])
            if not backup.exists() or ('baseline_sha256' in record and digest(backup) != record['baseline_sha256']):
                raise ValueError(f'Original overlay backup missing or changed: {backup}')
        target = contained(output, record['path'])
        current = digest(target) if target.exists() else None
        if current != record['installed_sha256']:
            raise ValueError(f'Overlay output changed externally; preserve it and resolve before prepare: {target}')
    for record in doc['outputs']:
        target = contained(output, record['path']); backup = contained(work / 'baseline', record['path'])
        if record['had_original']:
            target.parent.mkdir(parents=True, exist_ok=True); shutil.copyfile(backup, target)
        else:
            target.unlink(missing_ok=True)
    state.unlink()


def build(manifest, root, output, work, tex3ds):
    if (work / 'installed.json').exists():
        raise ValueError('Run prepare before original converters and build')
    doc = read_manifest(manifest, root)
    output.mkdir(parents=True, exist_ok=True); work.mkdir(parents=True, exist_ok=True)
    tool_key = digest(__file__) + digest(ROOT / 'tools/asset_animation.py') + PIL.__version__ + sys.version
    if any(e['kind'] in ('texture', 'atlas') for e in doc['assets']):
        executable = shutil.which(tex3ds) or tex3ds
        tool_key += digest(executable)
    records = []; installs = []
    with tempfile.TemporaryDirectory(dir=work) as scratch:
        stage = Path(scratch)
        for entry in doc['assets']:
            source = contained(root, entry['source']); names = output_names(entry)
            target = contained(stage, entry['output']); target.parent.mkdir(parents=True, exist_ok=True)
            kind = entry['kind']; inputs = {entry['source']: digest(source)}
            original_prefix = None
            if entry.get('inherit_original_visibility', False):
                if kind != 'model':
                    raise ValueError('Original visibility inheritance requires a model')
                original_prefix = contained(output, entry['output'])
                for suffix in ('.vertices', '.batches'):
                    inputs['original-visibility' + suffix] = digest(Path(str(original_prefix) + suffix))
            if kind == 'atlas':
                for frame in json.loads(source.read_text())['frames']:
                    inputs[frame['source']] = digest(contained(root, frame['source']))
            key = hashlib.sha256((tool_key + json.dumps([entry, inputs], sort_keys=True)).encode()).hexdigest()
            cache = work / 'cache' / key
            marker = cache / 'outputs.json'
            cached = False
            if marker.exists():
                saved = json.loads(marker.read_text())
                cached = all((digest(contained(cache, n)) if contained(cache, n).exists() else None) == h for n, h in saved.items()) and set(saved) == set(names)
            if cached:
                for name in names:
                    previous = contained(cache, name)
                    if previous.exists():
                        generated = contained(stage, name)
                        generated.parent.mkdir(parents=True, exist_ok=True); shutil.copyfile(previous, generated)
            else:
                if kind == 'texture':
                    compile_texture(source, entry, target, tex3ds)
                elif kind == 'atlas':
                    image, metadata = make_atlas(source, root, stage)
                    compile_texture(image, entry, target, tex3ds)
                    write_json(Path(str(target) + '.atlas.json'), metadata)
                elif kind == 'model':
                    model = model_from_obj(source, entry) if source.suffix.lower() == '.obj' else json.loads(source.read_text())
                    if original_prefix is not None:
                        model = inherit_model_visibility(model, original_prefix)
                    compile_model(model, target)
                elif kind == 'actor':
                    compile_actor(json.loads(source.read_text()), target)
                elif kind == 'animation':
                    animation = json.loads(source.read_text())
                    json.dumps(animation, allow_nan=False)
                    target.write_bytes(encode_animation(animation))
                elif kind in ('sfx', 'music'):
                    compile_audio(source, entry, target)
                else:
                    if source.suffix.lower() == '.iso':
                        raise ValueError('ISO images cannot be packaged')
                    shutil.copyfile(source, target)
                saved = {}
                for name in names:
                    generated = contained(stage, name); previous = contained(cache, name)
                    saved[name] = digest(generated) if generated.exists() else None
                    if generated.exists():
                        previous.parent.mkdir(parents=True, exist_ok=True); shutil.copyfile(generated, previous)
                    else:
                        previous.unlink(missing_ok=True)
                write_json(marker, saved)
            for name in names:
                generated = contained(stage, name)
                installs.append((name, generated))
            records.append({'id': entry['id'], 'kind': kind, 'license': entry['license'], 'inputs': inputs, 'cached': cached,
                            'outputs': {name: digest(contained(stage, name)) for name in names if contained(stage, name).exists()}})
        # All assets compile before any runtime output is changed.
        installed = []
        for name, generated in installs:
            target = contained(output, name); backup = contained(work / 'baseline', name)
            had_original = target.exists()
            if had_original:
                backup.parent.mkdir(parents=True, exist_ok=True); shutil.copyfile(target, backup)
            installed.append({'path': name, 'had_original': had_original,
                              'installed_sha256': digest(generated) if generated.exists() else None,
                              'baseline_sha256': digest(backup) if had_original else None})
        write_json(work / 'installed.json', {'schema': 1, 'outputs': installed})
        try:
            for name, generated in installs:
                target = contained(output, name)
                if generated.exists():
                    target.parent.mkdir(parents=True, exist_ok=True); shutil.copyfile(generated, target)
                else:
                    target.unlink(missing_ok=True)
        except OSError:
            # Restore even a partially installed transaction.
            for record in installed:
                target = contained(output, record['path'])
                if record['had_original']:
                    shutil.copyfile(contained(work / 'baseline', record['path']), target)
                else:
                    target.unlink(missing_ok=True)
            (work / 'installed.json').unlink()
            raise
    write_json(work / 'build.json', {'schema': 1, 'tool_sha256': digest(__file__), 'manifest_sha256': digest(manifest), 'assets': records})
    # A root-level stamp makes nested RomFS edits visible to the 3DS make rule.
    stamp = output / 'asset-pipeline-stamp.json'
    signature = hashlib.sha256(json.dumps([{k: v for k, v in r.items() if k != 'cached'} for r in records], sort_keys=True).encode()).hexdigest()
    value = {'schema': 1, 'signature': signature, 'tool_sha256': digest(__file__)}
    if not stamp.exists() or json.loads(stamp.read_text()) != value:
        write_json(stamp, value)
    print(f'Compiled {len(records)} editable assets')


def export_model(prefix, target):
    raw = Path(str(prefix) + '.vertices').read_bytes()
    batch = Path(str(prefix) + '.batches').read_bytes()
    n, textures = struct.unpack_from('<II', batch)
    if len(raw) % 36 or len(batch) != 8 + n * 24:
        raise ValueError('Invalid native model for export')
    doc = {'schema': 1, 'texture_count': textures,
           'vertices': [list(v) for v in struct.iter_unpack('<9f', raw)],
           'batches': [list(v) for v in struct.iter_unpack('<IIiIII', batch[8:])]}
    write_json(target, doc)


def export_actor(source, target):
    raw = source.read_bytes()
    if raw[:8] != b'PMCHAR1\0':
        raise ValueError('Expected PMCHAR1 mesh')
    count = struct.unpack_from('<I', raw, 8)[0]; pos = 12; poses = []
    for _ in range(count):
        name, n = struct.unpack_from('<32sI', raw, pos); pos += 36
        vertices = [list(v) for v in struct.iter_unpack('<5f', raw[pos:pos + n * 20])]; pos += n * 20
        poses.append({'name': name.split(b'\0')[0].decode('ascii'), 'vertices': vertices})
    if pos != len(raw):
        raise ValueError('Actor pack extent mismatch')
    write_json(target, {'schema': 1, 'poses': poses})


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('command', choices=['build', 'prepare', 'check', 'export-model', 'export-actor', 'export-animation'])
    parser.add_argument('--root', type=Path, default=ROOT)
    parser.add_argument('--manifest', type=Path, default=Path('assets/manifest.json'))
    parser.add_argument('--output', type=Path, default=Path('romfs'))
    parser.add_argument('--work', type=Path, default=Path('local-assets/pipeline'))
    parser.add_argument('--tex3ds', default=shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds')
    parser.add_argument('--source', type=Path)
    parser.add_argument('--destination', type=Path)
    args = parser.parse_args(); root = args.root.resolve()
    manifest = args.manifest if args.manifest.is_absolute() else root / args.manifest
    output = args.output if args.output.is_absolute() else root / args.output
    work = args.work if args.work.is_absolute() else root / args.work
    try:
        if args.command == 'prepare':
            prepare(output, work)
        elif args.command == 'build':
            build(manifest, root, output, work, args.tex3ds)
        elif args.command == 'check':
            print(f'Validated {len(read_manifest(manifest, root)["assets"])} asset recipes')
        else:
            if args.source is None or args.destination is None:
                parser.error('export requires --source and --destination')
            if args.command == 'export-animation':
                write_json(args.destination, decode_animation(args.source.read_bytes()))
            else:
                (export_model if args.command == 'export-model' else export_actor)(args.source, args.destination)
    except (ValueError, KeyError, OSError, subprocess.CalledProcessError, struct.error, wave.Error) as error:
        parser.exit(1, f'asset-pipeline: {error}\n')


if __name__ == '__main__':
    main()
