import hashlib
import tempfile
import json
import struct
import sys
import unittest
from pathlib import Path
from PIL import Image
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from source_icon_assets import padded, build

class OriginalSourceIcons(unittest.TestCase):
    def test_gutters_preserve_original_pixels_and_source_wrap(self):
        image=Image.new('RGBA',(2,2));image.putdata([(255,0,0,255),(0,255,0,255),(0,0,255,255),(1,2,3,4)])
        for wrap in (0,1,2):
            cell=padded(image,wrap,wrap)
            self.assertEqual(cell.size,(8,8))
            self.assertEqual(list(cell.crop((2,2,4,4)).getdata()),list(image.getdata()))
        self.assertEqual(padded(image,0,0).getpixel((1,2)),image.getpixel((0,0)))
        self.assertEqual(padded(image,1,1).getpixel((1,2)),image.getpixel((1,0)))
        self.assertEqual(padded(image,2,2).getpixel((1,2)),image.getpixel((0,0)))
    def test_wrong_disc_is_rejected_before_asset_writes(self):
        with tempfile.TemporaryDirectory() as tmp:
            path=Path(tmp)/'bad.iso';path.write_bytes(b'G8MJ01\0\0')
            with self.assertRaises(ValueError):build(path)
    def test_complete_original_resources_and_cell_identities(self):
        root=Path('romfs/source-icons');meta=json.loads((root/'sources.json').read_text());raw=(root/'icon.tpl').read_bytes()
        self.assertEqual(struct.unpack_from('>I',raw,4)[0],613)
        self.assertEqual(len(meta['images']),613)
        for name,digest in meta['sha256'].items():self.assertEqual(hashlib.sha256((root/name).read_bytes()).hexdigest(),digest)
        for index,row in enumerate(meta['images']):
            self.assertEqual(row['index'],index)
            self.assertEqual(hashlib.sha256((root/f'{index}.t3x').read_bytes()).hexdigest(),row['t3x_sha256'])
            w,h=row['native_size'];self.assertEqual(w&(w-1),0);self.assertEqual(h&(h-1),0)
            self.assertGreaterEqual(w,row['width']+4);self.assertGreaterEqual(h,row['height']+4)
        constants=(root/'constants.bin').read_bytes()
        self.assertEqual(len(constants),188)
        self.assertEqual(constants[116:124],bytes.fromhex('ffffff003c8efa35'))
        self.assertEqual(struct.unpack_from('>3I',constants,124),(0xffffffd0,0xffffff00,0xffffff00))
        self.assertEqual(struct.unpack_from('>5H',constants,164),(65,67,69,68,66))
        self.assertEqual(struct.unpack_from('>5H',constants,176),(70,72,74,73,71))
if __name__=='__main__':unittest.main()
