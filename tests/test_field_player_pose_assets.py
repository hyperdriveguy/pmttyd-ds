import hashlib,json,sys,tempfile,unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from field_player_pose_assets import build
from iso_assets import DEFAULT_ISO
class FieldPlayerPoseAssets(unittest.TestCase):
 def test_original_models_defaults_and_local_gpu_packages(self):
  with tempfile.TemporaryDirectory() as d:
   root=Path(d);meta=build(DEFAULT_ISO,root/'romfs',root/'work')
   for name,default in [('a_mario','M_Z_1'),('a_mario_r','M_Z_1R'),('e_mario','M_S_5'),('c_pkr','PKR_Z_1')]:
    r=meta['resources'][name];self.assertEqual(r['original_index0'],default);self.assertEqual(r['converted_pose_names'][0],default)
    raw=(root/'romfs'/(r['asset']+'.draw-model.bin')).read_bytes();self.assertEqual(raw[4:68].split(b'\0')[0].decode(),name)
    self.assertEqual(hashlib.sha256(raw).hexdigest(),r['source'][0]['sha256'])
    for product,info in r['products'].items():self.assertEqual(hashlib.sha256((root/'romfs'/product).read_bytes()).hexdigest(),info['sha256'])
   self.assertIn('PKR_D_2',meta['resources']['c_pkr']['converted_pose_names']);self.assertGreater((root/'romfs/field-player-c_pkr.t3x').stat().st_size,64)
   self.assertGreater((root/'romfs/shared-field-frankly.t3x').stat().st_size,64)
