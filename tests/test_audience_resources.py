import hashlib
import json
import struct
import unittest
import sys
from PIL import Image
from pathlib import Path

class OriginalAudienceResources(unittest.TestCase):
    def test_original_tpl_payloads_and_source_metadata(self):
        manifest = json.loads(Path('romfs/audience-resources.json').read_text())
        originals = {
            'audience-normal.tpl': (205, 428896, '16f8cdda26bb3727aeb9c2d7b494d66c1822d2ac94705dbad07b09d319e5e4f2'),
            'audience-luigi.tpl': (19, 39776, '3f556a8d9bd7cbfbba33ce3ab1a31531f174273451fe620f54182523533c48ef'),
        }
        for bank in manifest['banks']:
            path = Path(bank['path'])
            count, size, digest = originals[path.name]
            raw = path.read_bytes()
            self.assertEqual(len(raw), size)
            self.assertEqual(hashlib.sha256(raw).hexdigest(), digest)
            self.assertEqual(bank['sha256'], digest)
            self.assertEqual(bank['source'], 'battle/audience/' + path.name.replace('-', '_'))
            magic, images, table = struct.unpack_from('>III', raw)
            self.assertEqual((magic, images), (0x20af30, count))
            for i in range(images):
                image, palette = struct.unpack_from('>II', raw, table + 8*i)
                height, width, fmt, pixels = struct.unpack_from('>HHII', raw, image)
                self.assertEqual((width, height, fmt, palette), (64, 64, 14, 0))
                self.assertLessEqual(pixels + 2048, size)

    def test_native_catalog_preserves_every_bank_frame_and_pixel(self):
        sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
        from iso_assets import convert_tpl
        import tempfile
        manifest = json.loads(Path('romfs/audience-resources.json').read_text())
        catalog = Path(manifest['catalog']).read_bytes()
        self.assertEqual(struct.unpack_from('<8sIII', catalog),
                         (b'PMAUD001', 1024, 1024, 224))
        self.assertEqual(len(catalog), 20 + 224 * 32)
        self.assertEqual(manifest['texture_bytes'], 4 * 1024 * 1024)
        self.assertTrue(Path(manifest['texture']).stat().st_size > 0)
        atlas = Image.open(manifest['atlas']).convert('RGBA')
        self.assertEqual(atlas.size, (1024, 1024))
        expected = [(bank, index) for bank, count in [(1, 205), (2, 19)]
                    for index in range(count)]
        self.assertEqual([(f['bank'], f['index']) for f in manifest['frames']], expected)
        with tempfile.TemporaryDirectory() as work:
            image = Path(work) / 'original.png'
            for slot, record in enumerate(manifest['frames']):
                bank, index = expected[slot]
                source = Path(manifest['banks'][bank - 1]['path'])
                convert_tpl(source, image, index)
                original = Image.open(image).convert('RGBA')
                x, y = record['x'], record['y']
                self.assertEqual(original.size, (64, 64))
                actual = atlas.crop((x, y, x + 64, y + 64))
                self.assertEqual(actual.tobytes(), original.tobytes())
                self.assertEqual(hashlib.sha256(actual.tobytes()).hexdigest(),
                                 record['pixel_sha256'])
                self.assertEqual(struct.unpack_from('<4I4f', catalog, 20 + slot * 32),
                                 (bank, index, 64, 64, x/1024, 1-y/1024,
                                  (x+64)/1024, 1-(y+64)/1024))
                for edge in [atlas.crop((x-2, y-2, x+66, y)),
                             atlas.crop((x-2, y+64, x+66, y+66)),
                             atlas.crop((x-2, y, x, y+64)),
                             atlas.crop((x+64, y, x+66, y+64))]:
                    self.assertEqual(edge.tobytes(), bytes(len(edge.tobytes())))
        # Last cell and trailing atlas margins have no fabricated imagery.
        self.assertIsNone(atlas.crop((952, 954, 1024, 1024)).getbbox())

if __name__ == '__main__':
    unittest.main()
