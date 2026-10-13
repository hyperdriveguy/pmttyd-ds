import tempfile
import unittest
from pathlib import Path
from PIL import Image
from iso_assets import DEFAULT_ISO,decode_gx,extract,convert_tpl


class IntensityAlphaTests(unittest.TestCase):
    def test_intensity_masks_replicate_into_alpha(self):
        # Transparent background, intermediate coverage and opaque peak;
        # neither format has an independent opaque-only alpha channel.
        i4=decode_gx(bytes([0x07,0xbf])+bytes(30),8,8,0)
        self.assertEqual(i4[:4],[(0,0,0,0),(119,119,119,119),(187,187,187,187),(255,255,255,255)])
        i8=decode_gx(bytes([0,31,128,255])+bytes(28),8,4,1)
        self.assertEqual(i8[:4],[(0,0,0,0),(31,31,31,31),(128,128,128,128),(255,255,255,255)])

    def test_original_crystal_masks_keep_transparent_edges(self):
        if not Path(DEFAULT_ISO).is_file():self.skipTest('Original local US ISO required')
        with tempfile.TemporaryDirectory() as directory:
            root=Path(directory);tpl=root/'effect.tpl';extract(DEFAULT_ISO,'e/us/effect_n64.tpl',tpl)
            for index,size in [(109,(64,32)),(110,(64,64))]:
                png=root/f'{index}.png';convert_tpl(tpl,png,index)
                image=Image.open(png).convert('RGBA');self.assertEqual(image.size,size)
                pixels=image.tobytes()
                self.assertTrue(all(pixels[i]==pixels[i+1]==pixels[i+2]==pixels[i+3] for i in range(0,len(pixels),4)))
                self.assertEqual(image.getpixel((0,0)),(0,0,0,0))
                self.assertEqual(image.getextrema()[3],(0,255))


if __name__=='__main__':unittest.main()
