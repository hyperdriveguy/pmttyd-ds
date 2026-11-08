import hashlib
import json
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from import_tutorial_stage_files import import_files, SOURCE_HASHES
from iso_assets import DEFAULT_ISO


class TutorialStageFiles(unittest.TestCase):
    def test_original_payload_and_actual_dvd_tail(self):
        tails = {'m/bti_01/d': '00000020af30000000100000000c',
                 'm/stg_00_0/d': '00000020af30000000080000000c0000004c00000000'}
        with tempfile.TemporaryDirectory() as temporary:
            destination = Path(temporary) / 'original'
            records = import_files(DEFAULT_ISO, destination)
            self.assertEqual(len(records), 4)
            for row in records:
                raw = (destination / row['file']).read_bytes()
                logical = row['logical_bytes']
                self.assertEqual(len(raw), (logical + 31) & ~31)
                self.assertEqual(hashlib.sha256(raw[:logical]).hexdigest(), SOURCE_HASHES[row['source']])
                self.assertEqual(hashlib.sha256(raw).hexdigest(), row['transfer_sha256'])
                self.assertEqual(raw[logical:].hex(), tails.get(row['source'], ''))
            self.assertEqual(json.loads((destination / 'provenance.json').read_text())['files'], records)

    def test_refuse_existing_edits_before_opening_iso(self):
        with tempfile.TemporaryDirectory() as temporary:
            destination = Path(temporary)
            edited = destination / 'd.bin'
            edited.write_bytes(b'keep original user edits')
            with self.assertRaises(ValueError):
                import_files('/unopened.iso', destination)
            self.assertEqual(edited.read_bytes(), b'keep original user edits')
            with self.assertRaises(ValueError):
                import_files('/unopened.iso', edited)


if __name__ == '__main__':
    unittest.main()
