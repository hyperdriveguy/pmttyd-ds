from pathlib import Path
import sys
import unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from iso_assets import DEFAULT_ISO
from verify_harbor_tutorial_registry import verify
from harbor_tutorial_dol_assets import segments
class HarborRegistrySourceTests(unittest.TestCase):
    @unittest.skipUnless(Path(DEFAULT_ISO).is_file(),'original local ISO unavailable')
    def test_source_owner_and_real_final_scan_word(self):
        pins=verify(DEFAULT_ISO)
        self.assertEqual((pins['kind'],pins['hp'],pins['fp']),(220,999,0))
        self.assertEqual(pins['lookahead_word'],0x100)
        self.assertEqual(len(pins['spans']),5)
        imported=segments(DEFAULT_ISO)
        hooks=[body for address,body in imported if address==0x80419ef8]
        self.assertEqual(len(hooks),1)
        self.assertEqual(len(hooks[0]),28)
        self.assertEqual(hooks[0][-4:],b'\0\0\1\0')
        self.assertFalse(pins['executed_original'])
if __name__=='__main__':unittest.main()
