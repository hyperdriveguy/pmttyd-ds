from pathlib import Path
import sys
import unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from iso_assets import DEFAULT_ISO
from verify_battle_damage_orchestration import verify
class DamageOrchestrationSourceTests(unittest.TestCase):
    @unittest.skipUnless(Path(DEFAULT_ISO).is_file(),'original local ISO unavailable')
    def test_original_controller_helper_and_presentation_pins(self):
        result=verify(DEFAULT_ISO)
        self.assertEqual([s['size'] for s in result['spans']],[2560,424,20])
        self.assertEqual(result['anchors_checked'],13)
        self.assertEqual(result['constants'],(60,45,10,.75,1))
        self.assertFalse(result['executed_original'])
if __name__=='__main__':unittest.main()
