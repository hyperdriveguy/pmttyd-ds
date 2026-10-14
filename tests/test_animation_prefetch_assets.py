import hashlib,json,sys,unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from animation_prefetch_assets import SOURCES
@unittest.skipUnless(Path('romfs/field-prefetch/sources.json').exists(),'original private prefetch assets unavailable')
class OriginalAnimationPrefetch(unittest.TestCase):
 def test_original_sources_and_shared_texture(self):
  root=Path('romfs/field-prefetch')
  for name,(size,digest) in SOURCES.items():
   data=(root/name).read_bytes();self.assertEqual(len(data),size);self.assertEqual(hashlib.sha256(data).hexdigest(),digest)
  table=(root/'a/ag2tg.bin').read_bytes();rows=[table[n:n+32].split(b'\0')[0] for n in range(0,len(table),32)]
  self.assertEqual(rows[rows.index(b'p_dokan_x')+1],b'p_slit');self.assertEqual(rows[rows.index(b'p_dokan_y')+1],b'p_slit')
  self.assertEqual(len(json.loads((root/'sources.json').read_text())['sources']),4)
