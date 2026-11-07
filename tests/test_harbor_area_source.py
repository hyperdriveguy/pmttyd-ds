import hashlib
import json
import tempfile
import unittest
from pathlib import Path
from import_harbor_area_event import import_source
from asset_event import encode

ROOT=Path(__file__).resolve().parents[1]
class HarborAreaSourceTests(unittest.TestCase):
 def test_original_data_closure(self):
  base=ROOT/'assets/original/events/harbor-area-init'
  source=json.loads((base/'event.json').read_text())
  provenance=json.loads((base/'provenance.json').read_text())
  self.assertEqual(source['root'],0x9050ef7c)
  self.assertTrue(all(o['type']!='function' for o in provenance['closure']['objects']))
  original=next(o for o in provenance['closure']['objects'] if o['name']=='gor_00_init_evt')
  self.assertEqual(original['size'],0x870)
  self.assertTrue(provenance['linked_dol_events'])
  self.assertEqual(encode(source),(ROOT/'romfs/field-harbor-init.pmevt').read_bytes())
 def test_retains_creation_child(self):
  source=json.loads((ROOT/'assets/original/events/harbor-area-init/event.json').read_text())
  memory={s['address']+i:b for s in source['segments'] for i,b in enumerate(bytes.fromhex(s['bytes']))}
  body=bytes(memory[0x90505208+i] for i in range(0xe8))
  self.assertEqual(hashlib.sha256(body).hexdigest(),'c18a49a6a80ff6beff980986af1a7658c2b5025522702d264180ff0f297dfd93')
  self.assertEqual(bytes(memory[0x9050ef7c+i] for i in range(4)),bytes.fromhex('0000006b'))
 def test_import_preserves_existing_edits(self):
  with tempfile.TemporaryDirectory() as temporary:
   root=Path(temporary);marker=root/'event.json';marker.write_text('edited')
   with self.assertRaisesRegex(ValueError,'empty destination'):
    import_source('missing.iso','missing.csv',root)
   self.assertEqual(marker.read_text(),'edited')
