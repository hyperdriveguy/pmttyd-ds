import sys
import unittest
from pathlib import Path
ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
from verify_battle_class_native_frame import verify


class OriginalClassNativeFrame(unittest.TestCase):
    def test_original_queued_wrappers_and_reused_ring(self):
        result = verify()
        self.assertEqual([row['address'] for row in result['queued_bodies']],
                         ['0x8012210c', '0x80116cb8'])
        self.assertEqual(result['reused_ring']['bodies'], 4)


if __name__ == '__main__':
    unittest.main()
