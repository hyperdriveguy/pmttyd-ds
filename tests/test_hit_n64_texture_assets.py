"""Original private HitN64 GPU prerequisites; no normal effect admission."""
import hashlib
import json
from pathlib import Path
import struct
import unittest
from PIL import Image

PACK=Path('local-assets/hit-n64/pack')
@unittest.skipUnless((PACK/'hit-n64-texture74.json').is_file(),'private original HitN64 texture pack unavailable')
class HitN64TextureAssets(unittest.TestCase):
    def test_exact_original_bank_descriptor_vertices_and_source_views(self):
        m=json.loads((PACK/'hit-n64-texture74.json').read_text());bank=(PACK/'hit-n64-bank.tpl').read_bytes();meta=(PACK/'hit-n64-texture74.bin').read_bytes()
        self.assertEqual(hashlib.sha256(bank).hexdigest(),'3202ac8f437a2a7664d791ec5d9ee1988504f51b912c1211429708fac0a268ef')
        self.assertEqual(len(meta),476);self.assertEqual(struct.unpack_from('<8sIII',meta),(b'PMHITX01',74,32,128));self.assertEqual(meta[20:56],bank[0xf2c:0xf2c+36])
        self.assertEqual(hashlib.sha256(meta[56:336]).hexdigest(),'8778157212c6fd114b2746aefd029272202764db44511a746d3d168530feef9b')
        for span,data in zip(m['immutable_spans'],(meta[336:384],meta[384:436],meta[436:])):
            self.assertEqual((len(data),hashlib.sha256(data).hexdigest()),(span['bytes'],span['sha256']))
        self.assertEqual([s['address'] for s in m['immutable_spans']],[0x802fb0a0,0x80419888,0x80425458])
        for name,record in m['products'].items():
            data=(PACK/name).read_bytes();self.assertEqual((len(data),hashlib.sha256(data).hexdigest()),(record['bytes'],record['sha256']))

    def test_unscaled_i4_pixels_sampler_and_index16_source_rows(self):
        m=json.loads((PACK/'hit-n64-texture74.json').read_text());image=Image.open(m['texture']['decoded_png']).convert('RGBA');self.assertEqual(image.size,(32,128))
        self.assertEqual(hashlib.sha256(image.tobytes()).hexdigest(),m['rgba_sha256'])
        rgba=image.tobytes();self.assertTrue(all(rgba[i]==rgba[i+1]==rgba[i+2]==rgba[i+3] and rgba[i]%17==0 for i in range(0,len(rgba),4)))
        meta=(PACK/'hit-n64-texture74.bin').read_bytes();self.assertEqual(struct.unpack_from('>HH6I',meta,20),(128,32,0,110720,1,1,1,1));self.assertEqual(meta[48:56],bytes(8))
        for frame in range(5):
            vertices=meta[56+frame*56:56+(frame+1)*56];self.assertEqual(len(vertices),56)
            # Raw rows are original position S16x3 / UV S16x2 / RGBA8, stride14.
            rows=[struct.unpack_from('>5h4B',vertices,i*14) for i in range(4)]
            self.assertEqual([(r[0],r[1],r[2]) for r in rows],[(-16,-16,0),(16,-16,0),(16,16,0),(-16,16,0)])
