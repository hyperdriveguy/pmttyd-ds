import hashlib,json,struct,sys,tempfile,unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from animation_boot_image import build,SPANS
from iso_assets import DEFAULT_ISO
class AnimationBootImage(unittest.TestCase):
 def test_source_data_and_bss_precedence(self):
  with tempfile.TemporaryDirectory() as folder:
   meta=build(DEFAULT_ISO,folder);raw=(Path(folder)/'memory.pmevt').read_bytes();self.assertEqual(raw[:8],b'PMEVT001')
   count=struct.unpack_from('>I',raw,8)[0];self.assertEqual(count,len(SPANS));spans={}
   for i in range(count):
    a,n,o=struct.unpack_from('>III',raw,12+12*i);spans[a]=raw[o:o+n]
   for name,a,n,digest in SPANS:self.assertEqual(len(spans[a]),n);self.assertEqual(hashlib.sha256(spans[a]).hexdigest(),digest)
   self.assertEqual(spans[0x80415dc0],bytes.fromhex('803d9470'))
   self.assertEqual(spans[0x80415da0],bytes.fromhex('0000000000000002ffffffffffffffff'))
   self.assertEqual(spans[0x8041e428],bytes.fromhex('ffffffff00000000ffffffff'))
   self.assertTrue(any(spans[0x80305900])) # It is initialized DATA, never fabricated BSS zeros.
   self.assertEqual(spans[0x803d9470],bytes(0x110));self.assertEqual(spans[0x803d9580],bytes(712))
   self.assertEqual(meta['pack_sha256'],hashlib.sha256(raw).hexdigest())
   self.assertEqual(spans[0x80429358],bytes.fromhex('3f80000000000000'))
   self.assertEqual(spans[0x8041e458],bytes.fromhex('000000003f800000'))
   self.assertEqual(struct.unpack_from('>I',spans[0x80306698])[0],0x8003e5b8)
   self.assertEqual(struct.unpack_from('>I',spans[0x803066d8],28)[0],0x8003ec38)
   self.assertEqual(spans[0x80415dc8],bytes.fromhex('00000002000000010000000300000000'))
   self.assertEqual(spans[0x802c103c],bytes(12))
   self.assertEqual(spans[0x802c1078].split(b'\0')[0].count(b'%'),1)
   self.assertNotIn(0x803e6e78,spans) # Shared filename belongs to the real file cache.

   self.assertEqual(struct.unpack('>8I',spans[0x80306678]),(0x8003e178,0x8003e198,0x8003e1a0,0x8003e1a8,0x8003e1c0,0x8003e1c8,0x8003e1e0,0x8003e1f8))
   self.assertEqual(spans[0x802c1060],bytes.fromhex('4330000000000000'))
   self.assertEqual(spans[0x8041fb44],bytes.fromhex('3f000000'))
   self.assertEqual(spans[0x8041fb68],bytes.fromhex('bf000000'))
   self.assertEqual(len(spans[0x802c10c4]),220)
