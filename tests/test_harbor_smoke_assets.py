import importlib.util
from pathlib import Path
import tempfile,sys,unittest,json
from PIL import Image
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
spec=importlib.util.spec_from_file_location('harbor_smoke_import',ROOT/'tools/import_harbor_smoke_assets.py')
module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
class OriginalSmokeAssets(unittest.TestCase):
 def test_original_pins_pixels_and_edit_preservation(self):
  with tempfile.TemporaryDirectory() as t:
   dest=Path(t)/'editable';record=module.build(module.DEFAULT_ISO,dest)
   self.assertEqual(record['texture']['image'],40)
   with Image.open(dest/'smoke40.png') as image:self.assertEqual(image.size,(512,32));self.assertEqual(image.mode,'RGBA')
   self.assertEqual(len(record['spans']),len(module.BODIES)+len(module.SPANS))
   self.assertEqual(len(json.loads((dest/'memory.json').read_text())['segments']),len(module.SPANS))
   edited=dest/'smoke40.png';edited.write_bytes(b'original-art editor change')
   with self.assertRaisesRegex(ValueError,'must be empty'):module.build(module.DEFAULT_ISO,dest)
   self.assertEqual(edited.read_bytes(),b'original-art editor change')
if __name__=='__main__':unittest.main()
