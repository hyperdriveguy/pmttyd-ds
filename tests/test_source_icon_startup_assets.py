import hashlib
import json
import sys
import tempfile
import unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from source_icon_startup_assets import read,build
from iso_assets import DEFAULT_ISO

class IconStartup(unittest.TestCase):
 def test_actual_original_span_and_rounded_dvd_read(self):
  blob,meta=read(DEFAULT_ISO)
  self.assertEqual(len(blob),50)
  self.assertEqual(blob[:8],b'PMICST01')
  self.assertEqual(meta['format_sha256'],'b20463192bd4764d579f6b85f8f8119ebf8892396b7ccd6a9b28e05fec6ec59e')
  self.assertEqual(meta['dvd_tail_offset'],948147838)
  self.assertEqual(meta['dvd_tail_size'],18)
  self.assertEqual(meta['dvd_tail_sha256'],'385fd512ffe41b128bd5ea66415eb4176f7123635c958b7e25b89d45fa40b360')
  with Path(DEFAULT_ISO).open('rb') as f:
   f.seek(meta['source_offset'])
   actual=f.read(meta['rounded_read_size'])
  raw=Path('romfs/source-icons/icon.bin').read_bytes()
  self.assertEqual(actual,raw+blob[32:])
  self.assertEqual(hashlib.sha256(blob).hexdigest(),meta['sha256'])
 def test_empty_destination_and_provenance(self):
  with tempfile.TemporaryDirectory() as tmp:
   out=Path(tmp)/'new'
   meta=build(DEFAULT_ISO,out)
   self.assertEqual((out/'startup.bin').stat().st_size,50)
   self.assertEqual(json.loads((out/'sources.json').read_text()),meta)
   old=(out/'startup.bin').read_bytes()
   with self.assertRaises(ValueError):build(DEFAULT_ISO,out)
   self.assertEqual((out/'startup.bin').read_bytes(),old)

if __name__=='__main__':unittest.main()
