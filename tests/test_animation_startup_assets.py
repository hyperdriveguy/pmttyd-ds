import hashlib,sys,tempfile,unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from animation_startup_assets import build,SOURCES
from iso_assets import DEFAULT_ISO
class AnimationStartupAssets(unittest.TestCase):
 def test_exact_original_auxiliary_payloads(self):
  with tempfile.TemporaryDirectory() as folder:
   result=build(DEFAULT_ISO,folder);self.assertEqual([r['source_path'] for r in result['sources']],list(SOURCES))
   for name,(size,digest) in SOURCES.items():
    data=(Path(folder)/name).read_bytes();self.assertEqual(len(data),size);self.assertEqual(hashlib.sha256(data).hexdigest(),digest)
   self.assertEqual(len((Path(folder)/'a/vivian.bin').read_bytes()[0xc00:]),16)
