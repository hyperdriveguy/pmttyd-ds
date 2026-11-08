import importlib.util
from pathlib import Path
import tempfile
import unittest
import json
ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('import_harbor_star_point_assets',ROOT/'tools/import_harbor_star_point_assets.py')
module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
class ImportTests(unittest.TestCase):
 def test_original_source(self):
  with tempfile.TemporaryDirectory()as d:
   records=module.build(module.DEFAULT_ISO,Path(d)/'empty')
   self.assertEqual(len(records),11)
   memory=json.loads((Path(d)/'empty/memory.json').read_text())
   self.assertEqual([(q['address'],len(bytes.fromhex(q['bytes'])))for q in memory['segments']],[(0x802fe540,8),(0x8042033c,4),(0x80426f08,56)])
 def test_preserve_edit(self):
  with tempfile.TemporaryDirectory()as d:
   path=Path(d);(path/'edited').write_bytes(b'edited')
   with self.assertRaises(ValueError):module.build(module.DEFAULT_ISO,path)
   self.assertEqual((path/'edited').read_bytes(),b'edited')
if __name__=='__main__':unittest.main()
