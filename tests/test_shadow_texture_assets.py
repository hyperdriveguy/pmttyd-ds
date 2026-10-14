import hashlib
import json
import sys
import unittest
from pathlib import Path
from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT/'tools'))
from shadow_texture_assets import descriptor, PIXELS_SHA, RESOURCE


class ShadowTextureAssets(unittest.TestCase):
    def test_original_resource_pixels_and_sampler(self):
        manifest = json.loads((ROOT/'romfs/shadow-texture44.json').read_text())
        source = (ROOT/'local-assets/shadow/effect_n64.tpl').read_bytes()
        self.assertEqual(manifest['source'], RESOURCE)
        self.assertEqual(manifest['texture'], descriptor(source))
        with Image.open(ROOT/'local-assets/shadow/texture44.png') as image:
            self.assertEqual(image.size, (16, 16))
            self.assertEqual(hashlib.sha256(image.convert('RGBA').tobytes()).hexdigest(), PIXELS_SHA)
        self.assertEqual(manifest['texture']['wrap_s'], 'REPEAT')
        self.assertEqual(manifest['texture']['min_filter'], 'LINEAR')
        self.assertFalse(manifest['texture']['mipmaps'])
        self.assertTrue((ROOT/'romfs/shadow-texture44.t3x').is_file())
        damaged = bytearray(source)
        damaged[66496] ^= 1
        with self.assertRaises(ValueError):
            descriptor(damaged)


if __name__ == '__main__':
    unittest.main()
