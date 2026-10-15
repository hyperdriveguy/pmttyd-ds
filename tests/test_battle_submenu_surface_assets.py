import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import sys
import tempfile
import unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from battle_submenu_surface_assets import build
from iso_assets import DEFAULT_ISO

class OriginalSubmenuSurfaceAssets(unittest.TestCase):
    def test_original_pixels_catalog_and_empty_destination(self):
        if not Path(DEFAULT_ISO).is_file():self.skipTest('Original read-only ISO unavailable')
        with tempfile.TemporaryDirectory() as d:
            out=Path(d)/'output';cache=Path(d)/'cache';meta=build(output=out,work=cache)
            self.assertEqual(meta['dimensions'],[32,32]);self.assertEqual(meta['texture_bytes'],4096)
            self.assertEqual([(r['index'],r['width'],r['height']) for r in meta['images']],[(2,16,24),(3,8,24)])
            raw=(out/'battle-submenu-surfaces.bin').read_bytes();self.assertEqual(struct.unpack_from('<8sI',raw),(b'PMBSUB01',2));self.assertEqual(len(raw),68)
            from PIL import Image
            atlas=Image.open(cache/'atlas.png').convert('RGBA')
            for r in meta['images']:
                original=Image.open(cache/f"common-{r['index']}.png").convert('RGBA')
                cropped=atlas.crop((r['x'],r['y'],r['x']+r['width'],r['y']+r['height']))
                self.assertEqual(cropped.tobytes(),original.tobytes());self.assertEqual(hashlib.sha256(original.tobytes()).hexdigest(),r['pixel_sha256'])
                for dx in range(-2,r['width']+2):
                    for dy in range(-2,r['height']+2):
                        self.assertEqual(atlas.getpixel((r['x']+dx,r['y']+dy)),original.getpixel((min(r['width']-1,max(0,dx)),min(r['height']-1,max(0,dy)))))
            before={p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in out.iterdir()}
            with self.assertRaises(ValueError):build(output=out,work=Path(d)/'another-cache')
            self.assertEqual(before,{p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in out.iterdir()})
            with self.assertRaises(ValueError):build(output=Path(d)/'another-output',work=cache)
if __name__=='__main__':unittest.main()
