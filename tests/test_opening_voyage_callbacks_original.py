import sys,unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from iso_assets import DEFAULT_ISO
from verify_opening_voyage_callbacks import verify
class OriginalVoyageCallbacks(unittest.TestCase):
    @unittest.skipUnless(Path(DEFAULT_ISO).is_file(),'Original local US ISO unavailable')
    def test_original_callbacks_and_init(self):self.assertEqual(verify(),21)
if __name__=='__main__':unittest.main()
