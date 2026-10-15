import hashlib,json,struct,sys,tempfile,unittest
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
    for suffix in ['bin','t3x']:self.assertIn(r['asset']+'.'+suffix,r['products'])
    magic,scale=struct.unpack('<8sf',(root/'romfs'/(r['asset']+'.bin')).read_bytes());self.assertEqual(magic,b'PMSUV1\0\0');self.assertIn(scale,(1,2,4,8))
    self.assertGreater((root/'romfs'/(r['asset']+'.t3x')).stat().st_size,64)
    raw=(root/'romfs'/(r['asset']+'.draw-model.bin')).read_bytes();self.assertEqual(raw[4:68].split(b'\0')[0].decode(),name)
    self.assertEqual(hashlib.sha256(raw).hexdigest(),r['source'][0]['sha256'])
    for product,info in r['products'].items():self.assertEqual(hashlib.sha256((root/'romfs'/product).read_bytes()).hexdigest(),info['sha256'])
   partner=meta['resources']['c_pkr'];self.assertEqual(partner['converted_pose_names'],partner['source_descriptor_order']);self.assertEqual(len(partner['converted_pose_names']),32)
   self.assertEqual(struct.unpack('<8sI',(root/'romfs/field-player-c_pkr.pmanim').read_bytes()[:12]),(b'PMANIM4\0',32))
   self.assertIn('PKR_D_2',meta['resources']['c_pkr']['converted_pose_names']);self.assertGreater((root/'romfs/field-player-c_pkr.t3x').stat().st_size,64)
   self.assertGreater((root/'romfs/shared-field-frankly.t3x').stat().st_size,64)
