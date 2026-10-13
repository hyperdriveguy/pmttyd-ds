import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1]/'tools'))
import message_assets


class MessageAssetsTests(unittest.TestCase):
    def test_exact_bytes_and_repeat_build(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            iso = root/'source.iso'
            payload = b'b\0<dynamic 3>Original\x80<k><p>\0a\0Other\0\0\0'
            iso.write_bytes(b'header'+payload)
            info = dict(disc_id='G8ME01', files=[dict(path='msg/US/room.txt', offset=6, size=len(payload))])
            with patch.object(message_assets, 'inventory', return_value=info):
                result = message_assets.build(iso, root/'output')
                target = root/'output/room.txt'
                before = target.stat().st_mtime_ns
                message_assets.build(iso, root/'output')
                self.assertEqual(before, target.stat().st_mtime_ns)
            self.assertEqual(target.read_bytes(), payload)
            self.assertEqual(iso.read_bytes(), b'header'+payload)
            self.assertEqual(result['resources'][0]['pair_count'], 2)
            self.assertEqual(json.loads((root/'output/manifest.json').read_text()), result)

    def test_reject_invalid_source(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            iso = root/'source.iso'
            iso.write_bytes(b'key\0value\0')
            info = dict(disc_id='G8ME01', files=[dict(path='msg/US/room.txt', offset=0, size=10)])
            with patch.object(message_assets, 'inventory', return_value=info):
                with self.assertRaises(ValueError):
                    message_assets.build(iso, root/'output')
            with self.assertRaises(ValueError):
                message_assets.build(iso, iso)
            with self.assertRaises(ValueError):
                message_assets.build(iso, root)
            with patch.object(message_assets, 'inventory', return_value=dict(disc_id='G8MJ01', files=[])):
                with self.assertRaises(ValueError):
                    message_assets.build(iso, root/'output')


if __name__ == '__main__':
    unittest.main()
