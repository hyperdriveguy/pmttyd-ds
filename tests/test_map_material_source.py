import importlib.util
import json
from pathlib import Path
import sys
import tempfile
import unittest
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
from compile_map_material import generate,instruction
from import_map_material_assets import import_source
from iso_assets import DEFAULT_ISO
class MapMaterialSourceTests(unittest.TestCase):
 def test_complete_original_graph(self):
  graph=generate()
  self.assertEqual(graph,(ROOT/'source/display/source_map_material_program.inc').read_text())
  self.assertEqual(graph.count('case 0x'),1206)
  self.assertEqual(len(json.loads((ROOT/'tools/map_material_bodies.json').read_text())['bodies']),11)
 def test_original_import_and_no_overwrite(self):
  expected=ROOT/'assets/original/maps/material-source'
  with tempfile.TemporaryDirectory() as d:
   destination=Path(d)/'material'
   import_source(DEFAULT_ISO,destination)
   for name in ('event.json','provenance.json'):
    self.assertEqual((destination/name).read_bytes(),(expected/name).read_bytes())
   edited=destination/'event.json';edited.write_text('editable sentinel')
   with self.assertRaises(ValueError):import_source(DEFAULT_ISO,destination)
   self.assertEqual(edited.read_text(),'editable sentinel')
 def test_binary64_conversion_is_retained(self):
  # The exact original LOD load cannot round to f32 before its subtraction.
  code=instruction(0x800213e4,0xc8010150,set())
  self.assertIn('uint64_t(c.transfer)<<32|low',code)
  self.assertNotIn('float(',code)
if __name__=='__main__':unittest.main()
