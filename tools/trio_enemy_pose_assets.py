#!/usr/bin/env python3
"""Produce the original tik battle enemies' shared MODEL/TPL resource, read-only.

The CPU surface bank and atlas are native adaptations. Unchanged MODEL/TPL
companions, not those flattened triangles, feed retained source draw services.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import struct
import subprocess
from battle_pose_directory import source_directory, write_directory
from character_surface_mesh import compile_surface_mesh
from iso_assets import DEFAULT_ISO, extract
from rel_event_inventory import read_bindings

ASSET = 'trio-enemy-kuribo'
REL_SHA256 = 'ca30b06f0b7176dbe364349f239ef3ddfe66650e50ea1246921bd2a368a9dac7'

def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()

def source_roles(raw):
    """Read actual setup4 kinds/parts and first type28 match; no table guessing."""
    if hashlib.sha256(raw).hexdigest() != REL_SHA256:
        raise ValueError('Expected verified original US tik.rel')
    local, _, _ = read_bindings(raw)
    def word(offset):
        if offset < 0 or offset + 4 > len(raw):
            raise ValueError('Source word outside REL')
        return struct.unpack_from('>I', raw, offset)[0]
    def pointer(site):
        value = local.get(site, {}).get('file_offset')
        if value is None:
            raise ValueError('Required original local pointer missing')
        return value
    def name(offset):
        end = raw.find(b'\0', offset, offset + 64)
        if end < 0:
            raise ValueError('Unterminated original name')
        return raw[offset:end].decode('ascii')
    roles = []
    for i in range(3):
        setup = 0x263a0 + i * 0x30
        kind = pointer(setup)
        if raw[kind + 0xb4] != 1:
            raise ValueError('Expected one original enemy body part')
        part = pointer(kind + 0xb8)
        table = pointer(part + 0x48)
        # packedBattlePoseName(type28): the first exact/fallback tag28 wins.
        # Later type70 terminates. Do not inspect beyond the winning row.
        for row in range(4096):
            site = table + row * 8
            tag = struct.unpack_from('>i', raw, site)[0]
            if tag >= 70:
                raise ValueError('Original initial enemy pose unavailable')
            if tag == 28:
                initial = name(pointer(site + 4))
                break
        else:
            raise ValueError('Unbounded original pose lookup')
        roles.append(dict(unit_id=3 + i, setup_file_offset=setup,
                          kind_file_offset=kind, part_file_offset=part,
                          table_file_offset=table, model=name(pointer(part + 8)),
                          initial_pose=initial))
    if {r['model'] for r in roles} != {'c_kuribo'}:
        raise ValueError('Unexpected original enemy model dependencies')
    return roles

def tpl_metadata(raw):
    if len(raw) < 12 or struct.unpack_from('>I', raw)[0] != 0x20af30:
        raise ValueError('Invalid original TPL')
    count, table = struct.unpack_from('>II', raw, 4)
    if not count or count > 4096 or table + count * 8 > len(raw):
        raise ValueError('Invalid original TPL directory')
    result = []
    for index in range(count):
        header, palette = struct.unpack_from('>II', raw, table + 8 * index)
        if header + 36 > len(raw):
            raise ValueError('Truncated original TPL header')
        h, w, fmt, data, s, t, minimum, maximum = struct.unpack_from('>HH6I', raw, header)
        result.append(dict(index=index, header=header, palette=palette, width=w,
                           height=h, format=fmt, data=data, wrap_s=s, wrap_t=t,
                           min_filter=minimum, mag_filter=maximum,
                           lod_bytes=raw[header + 28:header + 36].hex()))
    return result

def build(iso=DEFAULT_ISO, work=Path('local-assets/trio-enemy-native'), output=Path('romfs')):
    work, output = Path(work), Path(output)
    work.mkdir(parents=True, exist_ok=True)
    output.mkdir(parents=True, exist_ok=True)
    rel, model, texture = (work / p for p in ('tik.rel', 'c_kuribo', 'c_kuribo.tpl'))
    extract(iso, 'rel/tik.rel', rel)
    roles = source_roles(rel.read_bytes())
    extract(iso, 'a/c_kuribo', model)
    original, bank, names = source_directory(model.read_bytes())
    if (original, bank) != ('c_kuribo', 'c_kuribo'):
        raise ValueError('Unexpected original MODEL identity')
    if any(r['initial_pose'] not in names for r in roles):
        raise ValueError('Source battle initial pose absent from original MODEL')
    extract(iso, 'a/c_kuribo-', texture)
    images = tpl_metadata(texture.read_bytes())
    helpers = ('trio_enemy_pose_assets.py', 'battle_pose_directory.py', 'character_surface_mesh.py',
               'character_mesh.py', 'character_bake.py', 'character_node.py',
               'character_animation_pack.py', 'character_animation_data.py', 'iso_assets.py',
               'rel_event_inventory.py')
    fingerprint = hashlib.sha256(b''.join(Path(__file__).with_name(p).read_bytes() for p in helpers)
                                 + rel.read_bytes() + model.read_bytes() + texture.read_bytes()).hexdigest()
    products = [output / (ASSET + '.' + ext) for ext in
                ('pmanim', 'pose-directory.bin', 'draw-model.bin', 'draw-textures.bin', 't3x', 'bin')]
    manifest = output / (ASSET + '.json')
    atlas = work / (ASSET + '.png')
    if manifest.is_file():
        cached = json.loads(manifest.read_text())
        if (cached.get('fingerprint') == fingerprint and atlas.is_file()
                and digest(atlas) == cached.get('atlas', {}).get('sha256')
                and all(p.is_file() and digest(p) == cached.get('products', {}).get(p.name, {}).get('sha256') for p in products)):
            return cached
    # A failed conversion leaves no success manifest. Never substitute idle clips.
    manifest.unlink(missing_ok=True)
    result = compile_surface_mesh([(model, texture, names)], products[0], atlas)
    write_directory(model, products[1], texture)
    products[5].write_bytes(b'PMSUV1\0\0' + struct.pack('<f', result['uv_scale']))
    subprocess.run([shutil.which('tex3ds') or '/opt/devkitpro/tools/bin/tex3ds',
                    '-f', 'rgba8', '-z', 'auto', '-o', str(products[4]), str(atlas)], check=True)
    result.update(asset=ASSET, model=original, texture=bank, fingerprint=fingerprint,
                  source_descriptor_order=names, source_roles=roles, source_tpl=images,
                  atlas=dict(path=str(atlas), sha256=digest(atlas), width=1024,
                             height=result['atlas_height'], uv_scale=result['uv_scale'],
                             native_format='RGBA8', ownership='Dedicated original c_kuribo resource identity; not underground flattened bank'),
                  assets=[json.loads(p.with_suffix(p.suffix + '.provenance.json').read_text()) for p in (rel, model, texture)],
                  products={p.name:dict(bytes=p.stat().st_size, sha256=digest(p)) for p in products},
                  readiness='Complete CPU descriptors + original draw/TPL metadata and converted atlas. Source pose registration, source buffers, GPU residency and queue admission remain separate.',
                  license='Original Nintendo/Intelligent Systems rights retained. Local extraction grants no redistribution rights.')
    manifest.write_text(json.dumps(result, indent=2) + '\n')
    return result

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--work', type=Path, default=Path('local-assets/trio-enemy-native'))
    parser.add_argument('--output', type=Path, default=Path('romfs'))
    args = parser.parse_args()
    result = build(args.iso, args.work, args.output)
    print('Original enemy resource:', result['model'], len(result['source_descriptor_order']), 'descriptors')
