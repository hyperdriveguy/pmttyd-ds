import hashlib,json,struct,unittest
from pathlib import Path
from PIL import Image
class OriginalItemGetAssets(unittest.TestCase):
    def test_original_pose_and_effect_pack(self):
        manifest=Path('romfs/field-item-get.json')
        if not manifest.exists():self.skipTest('Original local item-get pack not built')
        data=json.loads(manifest.read_text())
        self.assertEqual(data['dimensions'],[1024,512]);self.assertEqual(data['uv_scale'],2)
        self.assertEqual(data['sources'][2]['source_path'],'e/us/effect.tpl')
        self.assertEqual(data['sources'][2]['sha256'],'3b62850fe75afb95eebb9fc240e20204d5ed320fba0fc956729f28405c2c789b')
        self.assertEqual(struct.unpack('<8sf',Path('romfs/field-item-get-mario.bin').read_bytes()),(b'PMIGUV01',2))
        expected={42:[128,128],43:[16,128],44:[16,16]}
        for row in data['effect_images']:
            self.assertEqual(row['dimensions'],expected[row['id']])
            with Image.open('local-assets/field-item-get/effect-%d.png'%row['id']) as image:
                self.assertEqual(list(image.size),row['dimensions'])
                self.assertEqual(hashlib.sha256(image.tobytes()).hexdigest(),row['pixel_sha256'])
                self.assertTrue(all(r==g==b==a for r,g,b,a in image.convert('RGBA').getdata()))
