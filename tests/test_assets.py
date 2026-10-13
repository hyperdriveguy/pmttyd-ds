import importlib.util
from pathlib import Path
import struct
import tempfile
import unittest
spec=importlib.util.spec_from_file_location('iso_assets',Path(__file__).resolve().parents[1]/'tools/iso_assets.py')
a=importlib.util.module_from_spec(spec);spec.loader.exec_module(a)
class GXTests(unittest.TestCase):
    def test_i4_tiling_and_crop(self):
        p=a.decode_gx(bytes([0xf0]*32),7,5,0)
        self.assertEqual(len(p),35);self.assertEqual(p[0],(255,255,255,255));self.assertEqual(p[1],(0,0,0,0))
    def test_rgba8_planes(self):
        p=a.decode_gx(bytes([128,255]*16+[30,40]*16),4,4,6)
        self.assertEqual(p[0],(255,30,40,128))
    def test_rgb565(self):
        self.assertEqual(a.decode_gx(bytes.fromhex('f800')*16,4,4,4)[0],(255,0,0,255))
    def test_rgb5a3(self):
        self.assertEqual(a.rgb5a3(0xffff),(255,255,255,255));self.assertEqual(a.rgb5a3(0x0fff),(255,255,255,0))
    def test_cmpr_transparent_and_subblocks(self):
        block=struct.pack('>HH',0,0xffff)+bytes([0xff]*4)
        p=a.decode_gx(block*4,8,8,14)
        self.assertTrue(all(x==(0,0,0,0) for x in p))
        block=struct.pack('>HH',0xf800,0)+bytes(4)
        self.assertEqual(a.decode_gx(block*4,8,8,14)[63],(255,0,0,255))
    def test_palette(self):
        self.assertEqual(a.decode_gx(bytes(32),8,8,8,[(1,2,3,4)])[0],(1,2,3,4))
        with self.assertRaises(ValueError):a.decode_gx(bytes(32),8,8,8)
    def test_invalid_texture(self):
        for raw,w,h,fmt in [(b'',4,4,6),(bytes(64),0,4,6),(bytes(64),4,4,77)]:
            with self.assertRaises(ValueError):a.decode_gx(raw,w,h,fmt)
class DiscTests(unittest.TestCase):
    def fixture(self,path,unsafe=False):
        raw=bytearray(0x600);raw[:6]=b'G8ME01';struct.pack_into('>I',raw,0x1c,0xc2339f3d)
        names=b'\0'+(b'../bad\0' if unsafe else b'sample.tpl\0')
        fst=struct.pack('>III',0x01000000,0,2)+struct.pack('>III',1,0x580,4)+names
        struct.pack_into('>II',raw,0x424,0x440,len(fst));raw[0x440:0x440+len(fst)]=fst;raw[0x580:0x584]=b'test';path.write_bytes(raw)
    def test_inventory_extract(self):
        with tempfile.TemporaryDirectory() as d:
            iso=Path(d)/'test.iso';self.fixture(iso);before=iso.read_bytes()
            self.assertEqual(a.inventory(iso)['files'][0]['path'],'sample.tpl')
            out=Path(d)/'extracted';a.extract(iso,'sample.tpl',out)
            self.assertEqual(out.read_bytes(),b'test');self.assertEqual(iso.read_bytes(),before)
            with self.assertRaises(ValueError):a.extract(iso,'sample.tpl',iso)
    def test_bad_names_and_disc(self):
        with tempfile.TemporaryDirectory() as d:
            iso=Path(d)/'test.iso';self.fixture(iso,True)
            with self.assertRaises(ValueError):a.inventory(iso)
            iso.write_bytes(b'bad')
            with self.assertRaises(ValueError):a.inventory(iso)
if __name__=='__main__':unittest.main()
