import json,sys,unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from iso_assets import DEFAULT_ISO
from PIL import Image
class ClassIconTests(unittest.TestCase):
 def test_original_cells_and_unscaled_pixels(self):
  meta=json.loads(Path('romfs/battle-class-icons.json').read_text())
  self.assertEqual(meta['dimensions'],[256,256]);self.assertEqual(meta['texture_bytes'],262144)
  self.assertEqual([r['id'] for r in meta['icons']],list(range(334,344)))
  atlas=Image.open('local-assets/battle-class-icons/atlas.png').convert('RGBA')
  for row in meta['icons']:
   self.assertEqual((row['tpl'],row['width'],row['height'],row['loop'],row['delay']),(row['id']+5,48,48,0,0))
   x=round(row['uv'][0]*256);y=round((1-row['uv'][1])*256)
   source=Image.open(f"local-assets/battle-class-icons/{row['id']}.png").convert('RGBA')
   self.assertEqual(atlas.crop((x,y,x+48,y+48)).tobytes(),source.tobytes())
   self.assertEqual(atlas.getpixel((x-1,y-1)),(0,0,0,0))
