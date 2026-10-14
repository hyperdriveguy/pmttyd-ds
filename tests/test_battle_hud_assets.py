import json
from pathlib import Path
import sys
import unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from battle_hud_assets import source_art
from iso_assets import DEFAULT_ISO

class BattleHudAssetsTests(unittest.TestCase):
    @unittest.skipUnless(Path(DEFAULT_ISO).is_file(),'original local ISO unavailable')
    def test_verified_original_status_and_numeral_bindings(self):
        art=source_art(DEFAULT_ISO)
        self.assertEqual(len(art['rows']),13)
        self.assertEqual(art['rows'][0],dict(x=108,y=40,icon=472,scale=1))
        self.assertEqual(art['rows'][1]['icon'],427)
        self.assertEqual(art['rows'][4]['icon'],441)
        self.assertEqual(art['large_digits'],list(range(481,501,2)))
        self.assertEqual(art['small_digits'],list(range(482,502,2)))

    @unittest.skipUnless(Path('romfs/message-icons.json').is_file(),'local icon pack unavailable')
    def test_hud_frames_have_original_pixels_and_nonoverlapping_uvs(self):
        from PIL import Image
        meta=json.loads(Path('romfs/message-icons.json').read_text())
        atlas=Image.open('local-assets/message-icons/atlas.png').convert('RGBA')
        art=meta['battle_hud']
        wanted={r['icon'] for r in art['rows']}|set(art['large_digits'])|set(art['small_digits'])
        icons={i['id']:i for i in meta['icons']}
        self.assertTrue(wanted<=icons.keys())
        rectangles=[]
        for icon in meta['icons']:
            for frame in icon['frames']:
                x=round(frame['uv'][0]*atlas.width);y=round((1-frame['uv'][1])*atlas.height)
                w,h=frame['width'],frame['height']
                self.assertTrue(0<=x<x+w<=atlas.width and 0<=y<y+h<=atlas.height)
                for px,py,pw,ph in rectangles:
                    self.assertFalse(x<px+pw and px<x+w and y<py+ph and py<y+h)
                rectangles.append((x,y,w,h))
                if icon['id'] not in wanted:continue
                original=Image.open(f"local-assets/message-icons/{frame['tpl']}.png").convert('RGBA')
                self.assertEqual(atlas.crop((x,y,x+w,y+h)).tobytes(),original.tobytes())

if __name__=='__main__':unittest.main()
