"""Authored NintendoWare fixtures; no original Switch assets in source history."""
import json
from pathlib import Path
import struct
import sys
import tempfile
import unittest
from unittest import mock
import wave

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from switch_audio import NintendoWare, safe_path, wave_members, decode_job, bind_external_groups


def container(magic, sections):
    raw = bytearray(64)
    struct.pack_into('<4sHHIIHH', raw, 0, magic, 0xfeff, 64, 0x10000, 0, len(sections), 0)
    for index, (kind, name, body) in enumerate(sections):
        offset = len(raw); size = len(body) + 8
        struct.pack_into('<HHII', raw, 20 + index * 12, kind, 0, offset, size)
        raw.extend(struct.pack('<4sI', name, size) + body)
    struct.pack_into('<I', raw, 12, len(raw))
    return raw


def wave_archive(offset=0):
    member = container(b'FWAV', [])
    table = struct.pack('<IHHII', 1, 0x1f00, 0, offset, len(member))
    return container(b'FWAR', [(0x6800, b'INFO', table), (0x6801, b'FILE', member)])


class SwitchAudioTests(unittest.TestCase):
    def test_user_managed_group_keeps_conflicting_file_id_context(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory); groups = root / 'sound/userManagementFiles'
            groups.mkdir(parents=True)
            previous = container(b'FWAV', [])
            alternate = container(b'FWAR', [])
            (root / 'sound/existing.bin').write_bytes(previous)
            table = struct.pack('<IHHI', 1, 0x7900, 0, 12)
            row = struct.pack('<IHHII', 0, 0x1f00, 0, 0, len(alternate))
            group = container(b'FGRP', [(0x7800, b'INFO', table + row), (0x7801, b'FILE', alternate)])
            wrapper = bytearray(64) + group
            struct.pack_into('<III', wrapper, 0, 1, 64, len(group))
            (groups / 'test.bfgrp.bin').write_bytes(wrapper)
            record = {'file_id': 0, 'source': 'sound/existing.bin', 'offset': 0, 'size': len(previous)}
            inventory = {'files': [dict(record)]}
            bind_external_groups(inventory, root)
            self.assertEqual(inventory['files'][0], record)
            variant = inventory['group_variants'][0]
            self.assertEqual(variant['source'], 'sound/userManagementFiles/test.bfgrp.bin')
            self.assertEqual(variant['external_group_index'], 0)
            self.assertEqual(wrapper[variant['offset']:variant['offset'] + variant['size']], alternate)
            inventory = {'files': [{'file_id': 0, 'source': 'sound/sound.bfsar', 'offset': None}]}
            bind_external_groups(inventory, root)
            self.assertEqual(inventory['files'][0]['source'], variant['source'])
            self.assertEqual(inventory['group_variants'], [])

    def test_wave_table_uses_file_body_and_exact_member_span(self):
        raw = wave_archive(); records = wave_members(raw)
        self.assertEqual(len(records), 1)
        self.assertEqual(records[0]['wave_id'], 0)
        self.assertEqual(raw[records[0]['offset']:records[0]['offset'] + 4], b'FWAV')
        self.assertEqual(records[0]['size'], 64)
        with self.assertRaises(ValueError): wave_members(wave_archive(1))
        with self.assertRaises(ValueError): wave_members(wave_archive(0xffffff00))

    def test_truncated_duplicate_wrong_endian_and_declared_size(self):
        raw = wave_archive()
        for bad in [raw[:10], raw[:-1], b'XXXX' + raw[4:]]:
            with self.assertRaises(ValueError): NintendoWare(bad, b'FWAR')
        raw[4:6] = b'\xfe\xff'
        with self.assertRaises(ValueError): NintendoWare(raw, b'FWAR')
        duplicate = container(b'FWAR', [(0x6800, b'INFO', b''), (0x6800, b'INFO', b'')])
        with self.assertRaises(ValueError): NintendoWare(duplicate, b'FWAR')

    def test_path_and_reference_bounds(self):
        self.assertEqual(safe_path('stream/BGM/O/test.opus'), Path('stream/BGM/O/test.opus'))
        for path in ['', '/tmp/test', '../test', 'stream/../test', 'stream\\test']:
            with self.assertRaises(ValueError): safe_path(path)
        ar = NintendoWare(wave_archive(), b'FWAR')
        for offset, size in [(-1, 1), (0, len(ar.raw) + 1)]:
            with self.assertRaises(ValueError): ar.span(offset, size)
        with self.assertRaises(ValueError): ar.ref(0, 0, 0x220a)

    def test_decode_once_preserves_channels_rate_and_archive_loop(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory); source = root / 'test.opus'; source.write_bytes(b'fixture')
            metadata = {'sampleRate': 48000, 'channels': 2, 'numberOfSamples': 8,
                        'encoding': 'Opus', 'loopingInfo': None}
            calls = []

            def run(command, **kwargs):
                calls.append(command)
                if '-m' in command:
                    return mock.Mock(stdout=json.dumps(metadata))
                target = Path(command[command.index('-o') + 1])
                with wave.open(str(target), 'wb') as pcm:
                    pcm.setparams((2, 2, 48000, 8, 'NONE', 'not compressed')); pcm.writeframes(bytes(32))
                return mock.Mock()

            job = (source, Path('stream/test.opus'), {'source': 'sound/stream/test.opus'}, [2, 7])
            with mock.patch('switch_audio.subprocess.run', side_effect=run):
                record = decode_job(job, Path('decoder'), root)
                self.assertEqual(record['loop'], [2, 7])
                self.assertEqual(record['samples'], 8)
                self.assertEqual(record['channels'], 2)
                self.assertIn('-i', calls[-1]); self.assertIn('-W', calls[-1])
                sidecar = root / record['loop_sidecar']
                self.assertEqual(struct.unpack('<II', sidecar.read_bytes()), (2, 7))
                record = decode_job((*job[:3], None), Path('decoder'), root)
                self.assertIsNone(record['loop']); self.assertFalse(sidecar.exists())
                with self.assertRaises(ValueError):
                    decode_job((*job[:3], [0, 9]), Path('decoder'), root)


if __name__ == '__main__':
    unittest.main()
