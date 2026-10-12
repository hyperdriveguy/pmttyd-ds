import hashlib
from pathlib import Path
import struct
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from switch_package_inventory import inventory


class SwitchPackageInventoryTests(unittest.TestCase):
    def fixture(self, entries, names=b'a\0b\0', payload=b'abcdef'):
        return (struct.pack('<4sIII', b'PFS0', len(entries), len(names), 0)
                + b''.join(struct.pack('<QQII', *e, 0) for e in entries)
                + names + payload)

    def inspect(self, raw):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / 'test.nsp'
            path.write_bytes(raw)
            result = inventory(path)
            self.assertEqual(path.read_bytes(), raw)
            return result

    def test_out_of_order_entries_and_gaps_hash_correctly(self):
        raw = self.fixture([(4, 2, 2), (0, 2, 0)])
        report = self.inspect(raw)
        self.assertEqual(report['sha256'], hashlib.sha256(raw).hexdigest())
        self.assertEqual([e['sha256'] for e in report['entries']],
                         [hashlib.sha256(b).hexdigest() for b in (b'ef', b'ab')])

    def test_rejects_overlapping_or_truncated_members(self):
        for entries in [[(0, 4, 0), (3, 3, 2)], [(0, 7, 0)]]:
            with self.subTest(entries=entries), self.assertRaises(ValueError):
                self.inspect(self.fixture(entries))

    def test_rejects_invalid_names(self):
        for names, entries in [(b'../a\0', [(0, 1, 0)]),
                               (b'a', [(0, 1, 0)]),
                               (b'a\0a\0', [(0, 1, 0), (1, 1, 2)])]:
            with self.subTest(names=names), self.assertRaises(ValueError):
                self.inspect(self.fixture(entries, names))

    def test_rejects_short_header_and_bad_magic(self):
        for raw in [b'PFS0', b'BAD!' + self.fixture([])[4:]]:
            with self.subTest(raw=raw), self.assertRaises(ValueError):
                self.inspect(raw)


if __name__ == '__main__':
    unittest.main()
