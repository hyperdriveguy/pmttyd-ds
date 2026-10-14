import json,struct,unittest
from pathlib import Path
from PIL import Image
class ClassSurfaceAssets(unittest.TestCase):
 def test_original_pixels_and_native_descriptors(self):
  root=Path(__file__).resolve().parents[1]
  manifest=json.loads((root/'romfs/battle-class-surfaces.json').read_text())
  self.assertEqual(manifest['source'],'battle/common/battle_common.tpl')
  self.assertEqual(manifest['dimensions'],[256,128])
  raw=(root/'romfs/battle-class-surfaces.bin').read_bytes()
  self.assertEqual(struct.unpack_from('<8sI',raw),(b'PMBCLS01',2));self.assertEqual(len(raw),68)
  atlas=Image.open(root/'local-assets/battle-class-ui/atlas.png').convert('RGBA')
  for index,record in enumerate(manifest['images']):
   original=Image.open(root/f'local-assets/battle-class-ui/common-{index}.png').convert('RGBA')
   x,y=record['x'],record['y'];w,h=record['width'],record['height']
   self.assertEqual(original.size,[(40,40),(128,80)][index]);self.assertEqual(atlas.crop((x,y,x+w,y+h)).tobytes(),original.tobytes())
   self.assertEqual(struct.unpack_from('<3I',raw,12+28*index),(index,w,h))
   self.assertEqual(struct.unpack_from('<4f',raw,24+28*index),(x/256,1-y/128,(x+w)/256,1-(y+h)/128))
   self.assertEqual(atlas.getpixel((x-1,y)),(0,0,0,0));self.assertEqual(atlas.getpixel((x+w,y)),(0,0,0,0))
if __name__=='__main__':unittest.main()
