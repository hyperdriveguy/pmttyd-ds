from pathlib import Path
import sys
import unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from iso_assets import DEFAULT_ISO
from verify_source_battle_voice import verify
class SourceBattleVoiceTests(unittest.TestCase):
    @unittest.skipUnless(Path(DEFAULT_ISO).is_file(),'original local ISO unavailable')
    def test_original_complete_bodies_register_base_and_menu_rows(self):
        result=verify()
        self.assertEqual(result['whole_bodies'],8)
        self.assertEqual(len(result['voice_keys']),15)
        self.assertEqual(result['voice_table_addresses'],['80416ad0','80416ae4'])
        self.assertEqual(result['menu_rows']['2522']['descriptor'],0)
        for key in ['2520','2521','2523']:
            self.assertNotEqual(result['menu_rows'][key]['descriptor'],0)
