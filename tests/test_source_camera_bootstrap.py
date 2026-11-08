import sys,unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from verify_source_camera_bootstrap import verify
class CameraSource(unittest.TestCase):
 def test_original_complete_bodies_and_selected_fragment(self):self.assertEqual(verify(),996)
if __name__=='__main__':unittest.main()
