from pathlib import Path
import sys
import unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from verify_battle_hit_code_events import DEFAULT_ISO,verify
class OriginalHitMetadata(unittest.TestCase):
 @unittest.skipUnless(Path(DEFAULT_ISO).is_file(),'local original ISO unavailable')
 def test_three_complete_source_bodies(self):
  self.assertEqual(verify()['bodies'],3)
