#!/usr/bin/env python3
"""Import three original battle-ending primary samples into an EMPTY directory.

Source ISO is opened read-only. Existing editable imports are never overwritten.
The primary-sample presentation policy is not a MusyX macro interpreter.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import tempfile
from audio_convert import sample_directory, convert_sample
from audio_map import gamecube_tables, project_effects, macro_samples
from iso_assets import DEFAULT_ISO, inventory

LICENSE = 'Nintendo/Intelligent Systems original game asset; no redistribution license granted'
# Original btlseqEnd literal keys; the Mario names are RELIEF1/2.
ENDING_SAMPLES = ((0x802feafc, 'SFX_VOICE_MARIO_RELIEF1', 225, 635, 'mario-voice-relief1.wav'), (0x802feb14, 'SFX_VOICE_MARIO_RELIEF2', 226, 636, 'mario-voice-relief2.wav'), (0x802feaa8, 'SFX_STAR_POINT_GET1', 2616, 785, 'star-point-get1.wav'))
PROJECT_SHA = 'f00a06f50bd9da5769f07fa751c2e4ae3731f3e0850236d62cd901c7f8eccdeb'
POOL_SHA = 'd0b12b497cee5718a5433285bdec8fcfe9b2dd25b1c1c0daa77ddb804cb7d1a0'
SDIR_SHA = '9529535398c5f7a839dd18f00c8e831957b26b5aab6a7bf83e5ee6acb5cb8496'
SAMP_SHA = '062f7542e2f13f867ee30a447904b2b8af0ae8e4334ca39b7e1f4286d90833ea'

def sha(data):
    return hashlib.sha256(data).hexdigest()

def build(iso, destination):
    iso, destination = Path(iso), Path(destination)
    if destination.exists() and (not destination.is_dir() or any(destination.iterdir())):
        raise ValueError('Import destination must be empty; editable assets are never overwritten')
    info = inventory(iso)
    files = {r['path']: r for r in info['files']}
    rows, table_spans, _ = gamecube_tables(iso)
    names = {r['name']: r for r in rows if r['table'] == 'sfx'}
    banks = {}; provenance = {}
    with iso.open('rb') as disc:
        if disc.read(8) != b'G8ME01\0\0':
            raise ValueError('Original US disc0 revision0 required')
        disc.seek(0x420); dol, = struct.unpack('>I', disc.read(4)); disc.seek(dol); header = disc.read(256)
        sections = [tuple(struct.unpack_from('>I', header, b+4*i)[0] for b in (0, 0x48, 0x90)) for i in range(18)]
        def dol_read(address, size):
            matches = [dol+offset+address-base for offset, base, length in sections if base <= address and address+size <= base+length]
            if len(matches) != 1:
                raise ValueError('Original voice table mapping differs')
            disc.seek(matches[0]); data = disc.read(size)
            if len(data) != size:
                raise ValueError('Truncated original voice table')
            return data, matches[0]
        for extension in ('proj', 'pool', 'sdir', 'samp'):
            path = 'sound/proj/pmario.'+extension; entry = files[path]
            disc.seek(entry['offset']); data = disc.read(entry['size'])
            if len(data) != entry['size']:
                raise ValueError('Truncated original sound bank')
            banks[extension] = data
            provenance[extension] = dict(source_path=path, iso_offset=entry['offset'], size=len(data), sha256=sha(data))
        if sha(banks['proj']) != PROJECT_SHA or sha(banks['pool']) != POOL_SHA or sha(banks['sdir']) != SDIR_SHA or sha(banks['samp']) != SAMP_SHA:
            raise ValueError('Original effect/macro bank differs')
        effects = project_effects(banks['proj']); macros = macro_samples(banks['pool'])
        with tempfile.TemporaryDirectory(prefix='ttyd-voice-directory-') as temporary:
            sdir = Path(temporary)/'pmario.sdir'; sdir.write_bytes(banks['sdir'])
            directory = {r['id']: r for r in sample_directory(sdir)}
        records = []; staged = []
        for pointer, name, expected_row, sample_id, filename in ENDING_SAMPLES:
            row = names[name]
            if sum(r['name']==name and r['table']=='sfx' for r in rows) != 1:
                raise ValueError('Ending sound lookup must be unique')
            actual_name, name_offset = dol_read(pointer, len(name)+1)
            objects = effects.get(row['fx_id'], [])
            if actual_name != name.encode()+b'\0' or row['table_index'] != expected_row or len(objects) != 1 or macros[objects[0]['object_id']]['sample_ids'] != [sample_id]:
                raise ValueError('Original ending name/object/sample identity differs: '+name)
            descriptor = directory[sample_id]
            encoded_size = ((descriptor['samples']+13)//14)*8
            sample_bytes = banks['samp'][descriptor['offset']:descriptor['offset']+encoded_size]
            if len(sample_bytes) != encoded_size:
                raise ValueError('Original sample exceeds bank')
            staged.append((filename, descriptor))
            records.append(dict(file=filename, name=name, name_pointer=hex(pointer), name_iso_offset=name_offset, row={**{k:v for k,v in row.items() if k != 'parameters'}, 'parameters':dict(zip(('volume','pan','flags','mode','priority','delay','variation_high','variation_low'),row['parameters']))}, effect=objects[0], macro={k:v for k,v in macros[objects[0]['object_id']].items() if k != 'opcodes'}, sample_id=sample_id, encoded_sample=dict(iso_offset=provenance['samp']['iso_offset']+descriptor['offset'], size=encoded_size, sha256=sha(sample_bytes)), license=LICENSE))
    # Commit the new import only after all original identity checks succeeded.
    destination.mkdir(parents=True, exist_ok=True)
    for (filename, descriptor), record in zip(staged, records):
        pcm = convert_sample(descriptor, banks['samp'], destination/filename)
        if pcm['loop_enabled']:
            raise ValueError('Original one-shot action voice unexpectedly loops')
        record['pcm'] = pcm; record['pcm_sha256'] = sha((destination/filename).read_bytes())
    decoder = Path(__import__('audio_convert').__file__)
    result = dict(schema=1, source='US G8ME01 disc0 revision0, read-only', bank_provenance=provenance, original_sound_tables=table_spans, decoder_sha256=sha(decoder.read_bytes()), importer_sha256=sha(Path(__file__).read_bytes()), voices=records, conversion='Original-rate mono PCM16 decode of the single primary DSP ADPCM sample per original voice effect. No normalization/resampling/tone generation. MusyX pitch/envelope/delay/spatial macro behavior is not reproduced.', license=LICENSE)
    (destination/'provenance.json').write_text(json.dumps(result, indent=2)+'\n')
    (destination/'LICENSE.txt').write_text(LICENSE+'\nOriginal source samples and derived PCM remain copyrighted.\nLocal source tracking grants no new redistribution license.\n')
    return result

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    parser.add_argument('--destination', required=True)
    args = parser.parse_args()
    result = build(args.iso, args.destination)
    print('Original ending sounds:', len(result['voices']), 'PCM frames:', sum(r['pcm']['samples'] for r in result['voices']))
