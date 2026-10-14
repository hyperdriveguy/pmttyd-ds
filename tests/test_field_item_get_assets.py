import hashlib,json,struct,unittest,wave
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

    def test_original_key_item_fanfare(self):
        manifest=Path('romfs/field-item-get.json')
        if not manifest.exists():self.skipTest('Original local item-get pack not built')
        data=json.loads(manifest.read_text());row=data['fanfare']
        self.assertEqual(row['provenance']['source_path'],'sound/stream/ff_get2_32k.stm')
        self.assertEqual(row['provenance']['sha256'],'847b529744401aa5590b821ff7502a65b1ac59045306dc8792d6d44b4e05f7dc')
        self.assertEqual((row['original_row'],row['volume'],row['pan']),(217,80,64))
        pcm=Path('romfs')/row['output']
        self.assertEqual(hashlib.sha256(pcm.read_bytes()).hexdigest(),row['pcm_sha256'])
        self.assertFalse(row['loop_enabled']);self.assertIsNone(row['loop'])
        self.assertFalse(Path(str(pcm)+'.loop').exists())
        with wave.open(str(pcm),'rb') as audio:
            self.assertEqual((audio.getframerate(),audio.getnchannels(),audio.getnframes(),audio.getsampwidth()),(32000,2,83888,2))

    def test_original_coin_fanfare(self):
        manifest=Path('romfs/field-item-get.json')
        if not manifest.exists():self.skipTest('Original local item-get pack not built')
        row=json.loads(manifest.read_text())['coin_fanfare']
        self.assertEqual(row['provenance']['source_path'],'sound/stream/ff_get4_32k.stm')
        self.assertEqual(row['provenance']['sha256'],'9abc00c564a50d6b3dadc5a76ff11ab428a0556152a7d1550502bf03451d38de')
        self.assertEqual((row['original_row'],row['volume'],row['pan'],row['bypass_global']),(219,80,64,True))
        pcm=Path('romfs')/row['output'];self.assertEqual(hashlib.sha256(pcm.read_bytes()).hexdigest(),row['pcm_sha256'])
        self.assertFalse(row['loop_enabled']);self.assertIsNone(row['loop']);self.assertFalse(Path(str(pcm)+'.loop').exists())
        with wave.open(str(pcm),'rb') as audio:
            self.assertEqual((audio.getframerate(),audio.getnchannels(),audio.getnframes(),audio.getsampwidth()),(32000,2,65352,2))
