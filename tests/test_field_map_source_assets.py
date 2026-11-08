import json
import sys
import tempfile
import unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from iso_assets import DEFAULT_ISO
from import_harbor_map_source import import_source
from verify_field_map_source import verify
class FieldMapSourceAssets(unittest.TestCase):
 def test_complete_original_source_spans(self):self.assertEqual(verify(),1070)
 def test_original_payload_fixups_and_scene_reproduce_editable_sources(self):
  with tempfile.TemporaryDirectory() as temporary:
   out=Path(temporary)/'new';import_source(DEFAULT_ISO,out)
   for name in ('event.json','provenance.json'):
    self.assertEqual(json.loads((out/name).read_text()),json.loads((Path('assets/original/maps/harbor-source')/name).read_text()))
   meta=json.loads((out/'provenance.json').read_text());self.assertEqual(meta['payload_size'],975520);self.assertEqual(len(meta['bindings']),15974);self.assertEqual(len(meta['scene_nodes']),540)
 def test_reimport_preserves_existing_edits(self):
  with tempfile.TemporaryDirectory() as temporary:
   out=Path(temporary);edited=out/'event.json';edited.write_text('retained edit')
   with self.assertRaises(ValueError):import_source(DEFAULT_ISO,out)
   self.assertEqual(edited.read_text(),'retained edit');self.assertEqual(list(out.iterdir()),[edited])
if __name__=='__main__':unittest.main()
