import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
from verify_battle_class_menu_messages import verify


class OriginalClassMenuMessages(unittest.TestCase):
    def test_original_us_labels_and_native_resource(self):
        result = verify(global_resource=ROOT / 'romfs/messages/global.txt')
        self.assertEqual(len(result['labels']), 7)
        self.assertTrue(result['native_copy_checked'])
        self.assertTrue(all(row['size_including_nul'] > 1 for row in result['labels']))


if __name__ == '__main__':
    unittest.main()
