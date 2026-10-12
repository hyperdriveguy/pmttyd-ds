"""Synthetic reference-audit cases; these do not execute original game code."""
import csv
from pathlib import Path
import struct
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from us_usage_audit import audit


class UsageAuditTests(unittest.TestCase):
    def fixture(self, root, revision=0, sentinel=True):
        raw = bytearray(0x2000)
        raw[:8] = b'G8ME01\x00' + bytes([revision])
        struct.pack_into('>I', raw, 0x420, 0x500)
        for index, off, address, size in [(0, 0x100, 0x80010000, 16),
                                          (7, 0x120, 0x80364348, 0x1a4)]:
            for base, value in [(0, off), (0x48, address), (0x90, size)]:
                struct.pack_into('>I', raw, 0x500 + base + index * 4, value)
        # Call and tail branch to the same annotated function; distinct evidence.
        struct.pack_into('>4I', raw, 0x600, 0x48000009, 0x48000004, 0x4e800020, 0x4e800020)
        struct.pack_into('>5I', raw, 0x620, 1, 0x80010008, 0, 0, 0)
        if not sentinel:
            for off in range(20, 420, 20):
                struct.pack_into('>I', raw, 0x620 + off, 1)
        rel = bytearray(0x60)
        struct.pack_into('>II', rel, 0x28, 0x38, 8)
        struct.pack_into('>II', rel, 0x38, 0, 0x40)
        for off, values in [(0x40, (0, 202, 1, 0)),
                            (0x48, (4, 1, 0, 0x80010008)),
                            (0x50, (0, 203, 0, 0))]:
            struct.pack_into('>HBBI', rel, off, *values)
        raw[0x1000:0x1060] = rel
        iso = root / 'source.iso'
        iso.write_bytes(raw)
        symbols = root / 'symbols.csv'
        with symbols.open('w') as f:
            w = csv.DictWriter(f, fieldnames=['area', 'sec_type', 'ram_addr', 'size', 'name', 'namespace'])
            w.writeheader()
            w.writerow(dict(area='_main', sec_type='text', ram_addr='80010008',
                            size='00000004', name='target', namespace='fixture.o'))
        inv = dict(disc_id='G8ME01', disc_size=len(raw),
                   files=[dict(path='rel/fixture.rel', offset=0x1000, size=len(rel))])
        return iso, symbols, inv

    def test_separates_calls_tail_branches_data_and_rel_imports(self):
        with tempfile.TemporaryDirectory() as tmp:
            iso, symbols, inv = self.fixture(Path(tmp))
            original = iso.read_bytes()
            with patch('us_usage_audit.inventory', return_value=inv):
                result = audit(iso, symbols)
            entry = result['functions'][0]
            self.assertEqual((entry['direct_link_sites'], entry['direct_nonlink_sites'],
                              entry['data_word_candidates']), (1, 1, 1))
            self.assertEqual(entry['rel_imports'], [dict(module='rel/fixture.rel',
                                                       section=1, offset=4, relocation_type=1)])
            self.assertEqual(result['action_command_table'][0]['callbacks']['main']['annotation'], 'target')
            self.assertEqual(iso.read_bytes(), original)

    def test_rejects_other_revision(self):
        with tempfile.TemporaryDirectory() as tmp:
            iso, symbols, inv = self.fixture(Path(tmp), revision=1)
            with patch('us_usage_audit.inventory', return_value=inv):
                with self.assertRaisesRegex(ValueError, 'revision 0'):
                    audit(iso, symbols)

    def test_rejects_unterminated_action_table(self):
        with tempfile.TemporaryDirectory() as tmp:
            iso, symbols, inv = self.fixture(Path(tmp), sentinel=False)
            with patch('us_usage_audit.inventory', return_value=inv):
                with self.assertRaisesRegex(ValueError, 'sentinel'):
                    audit(iso, symbols)
