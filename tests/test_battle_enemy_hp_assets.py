import struct
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch
from PIL import Image

TOOLS = Path(__file__).resolve().parents[1] / 'tools'
sys.path.insert(0, str(TOOLS))
import battle_enemy_hp_assets as hp


class EnemyHpAssetsTests(unittest.TestCase):
    def test_unverified_original_is_rejected(self):
        for raw in (b'', bytes(hp.SOURCE_SIZE), bytes(hp.SOURCE_SIZE + 1)):
            with self.assertRaisesRegex(ValueError, 'Original US common'):
                hp.verify_source(raw)

    def test_pack_preserves_pixels_and_separate_catalog(self):
        # Fixture images test the packer's exact pixel handling, not ISO identity.
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            records = [dict(index=i, width=40, height=16, format=14) for i in (17, 18)]
            originals = {}
            for index in (17, 18):
                image = Image.new('RGBA', (40, 16))
                image.putdata([(x * 5, y * 11, index, (x + y) * 4)
                               for y in range(16) for x in range(40)])
                originals[index] = image

            def extract(_iso, _source, target):
                Path(target).write_bytes(b'explicit test fixture')

            def convert(_tpl, target, index):
                originals[index].save(target)

            with patch.object(hp, 'extract', extract), patch.object(hp, 'convert_tpl', convert), \
                    patch.object(hp, 'verify_source', return_value=records), \
                    patch.object(hp.subprocess, 'run') as run:
                result = hp.build('fixture.iso', root / 'romfs', root / 'work')
            atlas = Image.open(root / 'work/atlas.png').convert('RGBA')
            self.assertEqual(atlas.size, (128, 32))
            self.assertEqual(atlas.getpixel((0, 0)), (0, 0, 0, 0))
            for index, x in ((17, 2), (18, 46)):
                self.assertEqual(atlas.crop((x, 2, x + 40, 18)).tobytes(),
                                 originals[index].tobytes())
            raw = (root / 'romfs/battle-enemy-hp-surfaces.bin').read_bytes()
            self.assertEqual(len(raw), 68)
            self.assertEqual(struct.unpack_from('<8sI', raw), (b'PMBEHP01', 2))
            for ordinal, x in enumerate((2, 46)):
                self.assertEqual(struct.unpack_from('<3I4f', raw, 12 + 28 * ordinal),
                                 (17 + ordinal, 40, 16, x / 128, 30 / 32,
                                  (x + 40) / 128, 14 / 32))
            self.assertEqual(result['texture_bytes'], 16384)
            self.assertIn('no resampling', result['conversion'])
            self.assertIn('rgba8', run.call_args.args[0])
            self.assertFalse((root / 'romfs/battle-class-footers.bin').exists())


if __name__ == '__main__':
    unittest.main()
