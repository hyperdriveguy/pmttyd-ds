from pathlib import Path
import json
import unittest
from PIL import Image

class OriginalKanban(unittest.TestCase):
    @unittest.skipUnless(Path('romfs/message-kanban.json').is_file(),'original local sign pack unavailable')
    def test_original_cells_and_clamped_middle(self):
        meta=json.loads(Path('romfs/message-kanban.json').read_text())
        self.assertEqual(meta['indices'],[29,31,30,32])
        self.assertEqual(meta['dimensions'],[1024,256])
        self.assertEqual(meta['source_sha256'],'2ef0cdc818d2402de5bfb95394f67d78017e74bc1b41c38b79780cb0b870dc67')
        root=Path('local-assets/message-kanban');atlas=Image.open(root/'atlas.png').convert('RGBA')
        for index,x,y,w,h in [(29,0,0,72,176),(31,72,0,416,40),(30,488,0,72,176),(32,72,136,416,40)]:
            source=Image.open(root/f'{index}.png').convert('RGBA')
            self.assertEqual(atlas.crop((x,y,x+w,y+h)).tobytes(),source.tobytes())
        edge=Image.open(root/'31.png').convert('RGBA').crop((0,39,416,40)).tobytes()
        for y in range(40,136):self.assertEqual(atlas.crop((72,y,488,y+1)).tobytes(),edge)
        self.assertEqual(atlas.getpixel((560,0)),(0,0,0,0))
        self.assertEqual(atlas.getpixel((0,176)),(0,0,0,0))
if __name__=='__main__':unittest.main()
