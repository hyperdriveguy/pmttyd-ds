import sys
from pathlib import Path
import unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from verify_battle_stage_floor import verify,DEFAULT_ISO
class StageFloorSource(unittest.TestCase):
    @unittest.skipUnless(Path(DEFAULT_ISO).is_file(),'local original ISO unavailable')
    def test_original_source(self):
        report=verify()
        self.assertEqual(report['bodies'],4)
        self.assertEqual(report['constants'],2)
