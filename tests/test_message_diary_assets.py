from pathlib import Path
import json,unittest
from PIL import Image

class OriginalDiary(unittest.TestCase):
    @unittest.skipUnless(Path('romfs/message-diary.json').is_file(),'original local diary unavailable')
    def test_original_pixels_and_clamp_footprint(self):
        meta=json.loads(Path('romfs/message-diary.json').read_text())
        self.assertEqual(meta['indices'],[6]);self.assertEqual(meta['content'],[560,176]);self.assertEqual(meta['origin'],[1,1])
        self.assertEqual(meta['source_sha256'],'2ef0cdc818d2402de5bfb95394f67d78017e74bc1b41c38b79780cb0b870dc67')
        root=Path('local-assets/message-diary');source=Image.open(root/'6.png').convert('RGBA');atlas=Image.open(root/'atlas.png').convert('RGBA')
        self.assertEqual(atlas.crop((1,1,561,177)).tobytes(),source.tobytes())
        for x,y,sx,sy in [(0,0,0,0),(561,0,559,0),(0,177,0,175),(561,177,559,175)]:self.assertEqual(atlas.getpixel((x,y)),source.getpixel((sx,sy)))
        self.assertEqual(atlas.crop((0,1,1,177)).tobytes(),source.crop((0,0,1,176)).tobytes())
        self.assertEqual(atlas.crop((561,1,562,177)).tobytes(),source.crop((559,0,560,176)).tobytes())
        self.assertEqual(atlas.crop((1,0,561,1)).tobytes(),source.crop((0,0,560,1)).tobytes())
        self.assertEqual(atlas.crop((1,177,561,178)).tobytes(),source.crop((0,175,560,176)).tobytes())
        self.assertEqual(atlas.getpixel((562,0)),(0,0,0,0));self.assertEqual(atlas.getpixel((0,178)),(0,0,0,0))
