import hashlib,json,struct,sys,unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from material_light_assets import decode
from battle_hit_assets import SOURCES

class OriginalMaterialLight(unittest.TestCase):
 def test_original_scene_bytes(self):
  raw=Path('romfs/material-light.bin').read_bytes();self.assertEqual(len(raw),64);self.assertEqual(raw[:8],b'PMLTSC01')
  for i,(name,digest) in enumerate(SOURCES.items()):
   source=Path('local-assets/material-light',name+'.d').read_bytes();payload,meta=decode(source,digest)
   self.assertEqual(payload,raw[8+24*i:32+24*i]);self.assertEqual(meta['light_count'],0)
  self.assertEqual(struct.unpack_from('>6I',raw,8),(0,0,0,0,0x447a0000,0xff))
  self.assertEqual(raw[56:],bytes.fromhex('707070ff707070ff'))
 def test_wrong_revision_rejected(self):
  with self.assertRaises(ValueError):decode(b'bad','')
