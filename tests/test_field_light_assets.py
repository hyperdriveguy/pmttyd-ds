import json
import sys
import tempfile
import unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from iso_assets import DEFAULT_ISO
from import_light_registry_assets import import_source as registry
from import_harbor_light_inputs import import_source as harbor

class FieldLightAssets(unittest.TestCase):
 def test_original_inputs_reproduce_tracked_editable_sources(self):
  for importer,name in ((registry,'registry-definition'),(harbor,'harbor-map-input')):
   with self.subTest(name=name),tempfile.TemporaryDirectory() as temporary:
    destination=Path(temporary)/'new'
    importer(DEFAULT_ISO,destination)
    for filename in ('event.json','provenance.json'):
     expected=Path('assets/original/lights')/name/filename
     self.assertEqual(json.loads((destination/filename).read_text()),json.loads(expected.read_text()))
 def test_existing_edits_survive_reimport(self):
  for importer in (registry,harbor):
   with self.subTest(importer=importer.__module__),tempfile.TemporaryDirectory() as temporary:
    destination=Path(temporary)
    edited=destination/'event.json';edited.write_text('retained editable source')
    with self.assertRaises(ValueError):importer(DEFAULT_ISO,destination)
    self.assertEqual(edited.read_text(),'retained editable source')
    self.assertEqual(list(destination.iterdir()),[edited])

if __name__=='__main__':unittest.main()
