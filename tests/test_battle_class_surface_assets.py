import hashlib,json,struct,sys,tempfile,unittest
from pathlib import Path
from PIL import Image
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from battle_class_surface_assets import COMMON_OUTPUT,SOURCE_HASH,SOURCE_SIZE,package_common_tpl
class ClassSurfaceAssets(unittest.TestCase):
 def test_exact_original_common_tpl_and_corruption_rejection(self):
  root=Path(__file__).resolve().parents[1]
  original=root/'local-assets/battle-class-ui/battle_common.tpl'
  raw=original.read_bytes()
  self.assertEqual(len(raw),SOURCE_SIZE)
  self.assertEqual(hashlib.sha256(raw).hexdigest(),SOURCE_HASH)
  self.assertEqual(struct.unpack_from('>3I',raw),(0x0020af30,97,12))
  with tempfile.TemporaryDirectory() as temporary:
   destination=Path(temporary)/'pack'
   record=package_common_tpl(original,destination)
   packaged=destination/COMMON_OUTPUT
   self.assertEqual(packaged.read_bytes(),raw)
   self.assertEqual(record['size'],SOURCE_SIZE)
   self.assertEqual(record['sha256'],SOURCE_HASH)
   self.assertEqual(record['path'],COMMON_OUTPUT)
   corrupted=Path(temporary)/'corrupted.tpl'
   changed=bytearray(raw);changed[-1]^=1;corrupted.write_bytes(changed)
   with self.assertRaisesRegex(ValueError,'Original US common battle texture changed'):
    package_common_tpl(corrupted,destination)
   self.assertEqual(packaged.read_bytes(),raw)
   corrupted.write_bytes(raw[:-1])
   with self.assertRaises(ValueError):package_common_tpl(corrupted,destination)
   self.assertEqual(packaged.read_bytes(),raw)

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
   source=(root/'local-assets/battle-class-ui/battle_common.tpl').read_bytes()
   table=struct.unpack_from('>I',source,8)[0];header=struct.unpack_from('>I',source,table+index*8)[0]
   self.assertEqual(struct.unpack_from('>4I',source,header+12),(0,0,1,1))
   for dx in range(-2,w+2):
    for dy in range(-2,h+2):
     self.assertEqual(atlas.getpixel((x+dx,y+dy)),original.getpixel((min(w-1,max(0,dx)),min(h-1,max(0,dy)))))
if __name__=='__main__':unittest.main()
