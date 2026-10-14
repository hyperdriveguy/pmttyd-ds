import sys,unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from verify_opening_sky_light import verify
from iso_assets import DEFAULT_ISO
@unittest.skipUnless(Path(DEFAULT_ISO).is_file(),'private original ISO unavailable')
class OriginalVoyageSkyLight(unittest.TestCase):
    def test_source_bindings(self):
        result=verify()
        self.assertEqual(result['normal_format'],'S8/64, stride3')
        self.assertEqual(result['point9_angular'],[1.5,0,0])
        self.assertEqual(result['point9_distance'],[1,0,0])
        self.assertEqual(result['bodies'],7)
        self.assertEqual(result['alpha_bodies'],9)
        self.assertEqual(result['alpha_references'],14)

        self.assertEqual(result["day_point9_rgba"],[54,104,202,255])
        self.assertEqual(result["day_point9_intensity"],1.5)
