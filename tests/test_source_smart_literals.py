import sys,tempfile,unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from import_source_smart_literals import import_literals
from iso_assets import DEFAULT_ISO
class SourceSmartLiterals(unittest.TestCase):
 def test_exact_original_spans_and_edit_preservation(self):
  with tempfile.TemporaryDirectory() as temp:
   import_literals(DEFAULT_ISO,temp)
   for name in ('event.json','provenance.json'):
    self.assertEqual((Path(temp)/name).read_bytes(),(Path('assets/original/memory/smart-process')/name).read_bytes())
   edited=Path(temp)/'event.json';edited.write_text('retained edit')
   with self.assertRaises(ValueError):import_literals(DEFAULT_ISO,temp)
   self.assertEqual(edited.read_text(),'retained edit')
