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
 def test_complete_original_source_spans(self):self.assertEqual(verify(),5116)
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

class HarborMapFiles(unittest.TestCase):
 def test_raw_files_and_disc_transfer_tail_reproduce_sources(self):
  from import_harbor_map_files import import_files
  import hashlib
  with tempfile.TemporaryDirectory() as temporary:
   out=Path(temporary)/'new';records=import_files(DEFAULT_ISO,out)
   for name in ('d.bin','t.bin','literals.json','provenance.json'):
    self.assertEqual((out/name).read_bytes(),(Path('assets/original/maps/harbor-files')/name).read_bytes())
   self.assertEqual([r['logical_bytes'] for r in records],[1039618,525792])
   self.assertEqual([r['transfer_bytes'] for r in records],[1039648,525792])
   for row in records:self.assertEqual(hashlib.sha256((out/row['file']).read_bytes()).hexdigest(),row['transfer_sha256'])
 def test_refuse_import_over_existing_edits(self):
  from import_harbor_map_files import import_files
  with tempfile.TemporaryDirectory() as temporary:
   edited=Path(temporary)/'d.bin';edited.write_bytes(b'editable-original')
   with self.assertRaises(ValueError):import_files(DEFAULT_ISO,temporary)
   self.assertEqual(edited.read_bytes(),b'editable-original');self.assertEqual(list(Path(temporary).iterdir()),[edited])


class HitLiterals(unittest.TestCase):
 def test_original_constants_and_import_guard(self):
  from import_field_hit_literals import import_literals
  with tempfile.TemporaryDirectory() as temporary:
   out=Path(temporary)/'new';import_literals(DEFAULT_ISO,out)
   for name in ('event.json','provenance.json'):self.assertEqual((out/name).read_bytes(),(Path('assets/original/maps/hit-literals')/name).read_bytes())
   edited=out/'event.json';edited.write_text('retained edit')
   with self.assertRaises(ValueError):import_literals(DEFAULT_ISO,out)
   self.assertEqual(edited.read_text(),'retained edit')

if __name__=='__main__':unittest.main()


class HarborCameraRoad(unittest.TestCase):
 def test_original_camera_and_initialized_literals_reproduce_sources(self):
  from import_harbor_camera_road import import_camera
  with tempfile.TemporaryDirectory() as temporary:
   import_camera(DEFAULT_ISO,temporary)
   for name in ('c.bin','literals.json','provenance.json'):
    self.assertEqual((Path(temporary)/name).read_bytes(),(Path('assets/original/maps/harbor-camera-road')/name).read_bytes())
 def test_existing_edits_are_retained(self):
  from import_harbor_camera_road import import_camera
  with tempfile.TemporaryDirectory() as temporary:
   file=Path(temporary)/'c.bin';file.write_bytes(b'edited')
   with self.assertRaises(ValueError):import_camera(DEFAULT_ISO,temporary)
   self.assertEqual(file.read_bytes(),b'edited')
