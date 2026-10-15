from pathlib import Path
import sys
import tempfile
import unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from battle_window_panel_assets import atlas,build
from build_battle_window_panel_assets import stage
from iso_assets import DEFAULT_ISO,inventory

class WindowPanel(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        e=next(e for e in inventory(Path(DEFAULT_ISO))['files'] if e['path']=='f/msgWindow.tpl')
        with Path(DEFAULT_ISO).open('rb') as f:f.seek(e['offset']);cls.raw=f.read(e['size'])
    def test_pixels_and_all_padding(self):
        source,image=atlas(self.raw)
        self.assertEqual(image.crop((0,0,72,72)).tobytes(),source.tobytes())
        for y in range(128):
            for x in range(128):self.assertEqual(image.getpixel((x,y)),source.getpixel((min(x,71),min(y,71))))
        # Independent bilinear sampling: complete padding is equivalent to
        # original clamp for coordinates within and beyond source0..1.
        def sample(im,u,v,scale=1):
            import math
            x=u*scale*im.width-.5;y=v*scale*im.height-.5
            x0=math.floor(x);y0=math.floor(y);a=x-x0;b=y-y0
            p=lambda xx,yy:im.getpixel((max(0,min(im.width-1,xx)),max(0,min(im.height-1,yy))))
            return tuple(sum(p(x0+dx,y0+dy)[i]*(a if dx else 1-a)*(b if dy else 1-b) for dx in (0,1) for dy in (0,1)) for i in range(4))
        for u in (-4,-.01,0,.0001,1/3,.9999,1,1.01,4):
            for v in (-4,-.01,0,1/3,.9999,1,1.01,4):
                for a,b in zip(sample(source,u,v),sample(image,u,v,72/128)):self.assertAlmostEqual(a,b,places=9)
    def test_staging_uses_fresh_export(self):
        with tempfile.TemporaryDirectory() as d:
            stage(DEFAULT_ISO,d)
            self.assertEqual({p.name for p in Path(d).iterdir()},{"battle-window-panel.t3x","battle-window-panel.json"})

    def test_identity_and_empty_destination(self):
        changed=bytearray(self.raw);changed[-1]^=1
        with self.assertRaises(ValueError):atlas(changed)
        with tempfile.TemporaryDirectory() as d:
            Path(d,'edited').write_text('preserve')
            with self.assertRaises(ValueError):build(DEFAULT_ISO,d)
