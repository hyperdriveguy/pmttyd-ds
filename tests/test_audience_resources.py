import hashlib
import json
import struct
import unittest
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

if __name__ == '__main__':
    unittest.main()
