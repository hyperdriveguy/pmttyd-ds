import hashlib,json,struct,unittest
from pathlib import Path
from PIL import Image

class FooterAssets(unittest.TestCase):
 def test_original_footer_pixels_and_regions(self):
  root=Path(__file__).resolve().parents[1]
  output=root/'romfs';work=root/'local-assets/battle-class-footers'
  if not (output/'battle-class-footers.json').exists():
   self.skipTest('private original footer pack unavailable')
  manifest=json.loads((output/'battle-class-footers.json').read_text())
  self.assertEqual(manifest['source'],'battle/common/battle_common.tpl')
  self.assertEqual(manifest['source_sha256'],'8eb685317056f42b3ef73138eaad85d16fa4943d830b44a149f09971a5959bb1')
  self.assertEqual(manifest['danger'],[6,7,8,7,9,10])
  self.assertEqual(manifest['critical'],list(range(11,17)))
  raw=(output/'battle-class-footers.bin').read_bytes()
  self.assertEqual(struct.unpack_from('<8sI',raw),(b'PMBCLF01',13))
  self.assertEqual(len(raw),376)
  atlas=Image.open(work/'atlas.png').convert('RGBA')
  self.assertEqual(atlas.size,(512,256))
  for ordinal,record in enumerate(manifest['images']):
   index=ordinal+4;self.assertEqual(record['index'],index)
   original=Image.open(work/f'common-{index}.png').convert('RGBA')
   x,y,w,h=(record[k] for k in ['x','y','width','height'])
   self.assertEqual(original.size,(80,32) if index<6 else (112,48 if index<11 else 56))
   self.assertEqual(atlas.crop((x,y,x+w,y+h)).tobytes(),original.tobytes())
   self.assertEqual(hashlib.sha256(original.tobytes()).hexdigest(),record['pixel_sha256'])
   self.assertEqual(struct.unpack_from('<3I4f',raw,12+ordinal*28),(index,w,h,x/512,1-y/256,(x+w)/512,1-(y+h)/256))
   self.assertEqual(atlas.getpixel((x-1,y)),(0,0,0,0))
   self.assertEqual(atlas.getpixel((x+w,y)),(0,0,0,0))
