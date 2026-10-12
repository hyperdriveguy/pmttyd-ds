import csv
import hashlib
import json
from pathlib import Path
import struct
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from pack_event_scripts import MAGIC, build_pack, write_pack


class EventPackTests(unittest.TestCase):
    def fixture(self, directory):
        address = 0x80350000
        first = struct.pack('>IIII', 0x00020032, 0xfe363c80, 0x12345678, 1)
        second = struct.pack('>III', 0x00010009, 2, 1)
        data = first + second + struct.pack('>I', 0xdeadbeef)
        disc = bytearray(0x900)
        disc[:8] = b'G8ME01\0\0'
        struct.pack_into('>I', disc, 0x420, 0x500)
        for offset, value in [(0, 0x100), (0x48, address), (0x90, len(data))]:
            struct.pack_into('>I', disc, 0x500 + offset, value)
        disc[0x600:0x600+len(data)] = data
        iso = directory / 'original.iso'
        iso.write_bytes(disc)
        symbols = directory / 'symbols.csv'
        fields = ['name', 'namespace', 'area', 'type', 'ram_addr', 'size', 'file_addr']
        with symbols.open('w') as handle:
            writer = csv.DictWriter(handle, fields)
            writer.writeheader()
            for name, start, file, size in [('first', address, 0x100, len(first)),
                                             ('second', address+len(first), 0x100+len(first), len(second))]:
                writer.writerow(dict(name=name, namespace='fixture.o', area='_main', type='evt',
                                     ram_addr=f'{start:08x}', size=f'{size:08x}', file_addr=f'{file:08x}'))
        return iso, symbols, address, data

    def test_original_bytes_addresses_overlap_and_padding(self):
        with tempfile.TemporaryDirectory() as tmp:
            iso, symbols, address, data = self.fixture(Path(tmp))
            before = iso.read_bytes()
            packed, manifest = build_pack(iso, symbols, ['second', 'first'])
            self.assertEqual(packed[:12], MAGIC + struct.pack('>I', 1))
            self.assertEqual(struct.unpack_from('>III', packed, 12), (address, len(data), 24))
            self.assertEqual(packed[24:], data)
            self.assertEqual(manifest['segments'][0]['sha256'], hashlib.sha256(data).hexdigest())
            self.assertEqual(manifest['events'][1]['commands'][0]['operands'][0]['raw'], 0xfe363c80)
            self.assertEqual(iso.read_bytes(), before)

    def test_write_manifest_and_protect_inputs(self):
        with tempfile.TemporaryDirectory() as tmp:
            folder = Path(tmp)
            iso, symbols, _, _ = self.fixture(folder)
            for output in [iso, symbols]:
                with self.assertRaises(ValueError):
                    write_pack(iso, symbols, ['first'], output)
            output = folder / 'ignored/events.pmevt'
            manifest = write_pack(iso, symbols, ['first'], output)
            self.assertEqual(json.loads(Path(str(output)+'.json').read_text()), manifest)
            self.assertEqual(hashlib.sha256(output.read_bytes()).hexdigest(), manifest['sha256'])
            self.assertEqual(manifest['source']['revision'], 'US G8ME01 disc0 revision0')
            self.assertIn('no redistribution', manifest['licensing'])

    def test_identity_mapping_and_missing_trailing_word(self):
        with tempfile.TemporaryDirectory() as tmp:
            iso, symbols, _, data = self.fixture(Path(tmp))
            with self.assertRaises(ValueError):
                build_pack(iso, symbols, [])
            with self.assertRaises(ValueError):
                build_pack(iso, symbols, ['missing'])
            original = iso.read_bytes()
            wrong = bytearray(original)
            wrong[7] = 1
            iso.write_bytes(wrong)
            with self.assertRaises(ValueError):
                build_pack(iso, symbols, ['first'])
            wrong = bytearray(original)
            struct.pack_into('>I', wrong, 0x500+0x90, len(data)-4)
            iso.write_bytes(wrong)
            with self.assertRaises(ValueError):
                build_pack(iso, symbols, ['second'])

    def test_duplicate_selection_deduplicates_identical_address(self):
        with tempfile.TemporaryDirectory() as tmp:
            iso, symbols, _, _ = self.fixture(Path(tmp))
            packed, manifest = build_pack(iso, symbols, ['first', 'first'])
            self.assertEqual(len(manifest['events']), 2)
            self.assertEqual(len(manifest['segments']), 1)
            self.assertEqual(struct.unpack_from('>I', packed, 8)[0], 1)

    def test_named_original_dependency_must_match_pinned_source(self):
        with tempfile.TemporaryDirectory() as tmp:
            iso, symbols, _, _ = self.fixture(Path(tmp))
            symbols.write_text(symbols.read_text().replace('first,', 'btldefaultevt_SuitoruBadgeEffect,'))
            with self.assertRaises(ValueError):
                build_pack(iso, symbols, ['btldefaultevt_SuitoruBadgeEffect'])


if __name__ == '__main__':
    unittest.main()
