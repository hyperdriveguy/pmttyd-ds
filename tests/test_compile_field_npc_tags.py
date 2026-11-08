import hashlib
import struct
import sys
import unittest
from pathlib import Path
ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
from compile_field_npc_tags import encode_field_npc_tags

class FieldNpcTags(unittest.TestCase):
    def test_original_harbor(self):
        raw = (ROOT / 'assets/original/harbor-npc-tags/gor_00.s').read_bytes()
        self.assertEqual(hashlib.sha256(raw).hexdigest(), '361cc4c7e2f85bc4b632fcc6daab9283fa39028676b2d27bb9c74a0989dcf32a')
        data = encode_field_npc_tags(raw)
        self.assertEqual(struct.unpack_from('<8sII', data), (b'PMFMNPC1', 17, 392))
        for ordinal, name in [(2, 'ガラの悪い水夫'), (12, '水夫ボム兵Ｂ')]:
            row = data[16+ordinal*392:16+(ordinal+1)*392]
            self.assertEqual(struct.unpack_from('<I', row)[0], ordinal)
            self.assertEqual(row[72:136].split(b'\0')[0], name.encode('shift_jis'))
            self.assertEqual(row[328:392].split(b'\0')[0], b'BTLNO_INVALID')
    def test_edit_is_compiled_without_import(self):
        raw = (ROOT / 'assets/original/harbor-npc-tags/gor_00.s').read_bytes()
        start = raw.index(b'<position>') + len(b'<position>')
        end = raw.index(b'</>', start)
        changed = raw[:start] + b' 1 2 3 ' + raw[end:]
        data = encode_field_npc_tags(changed)
        self.assertEqual(struct.unpack_from('<3f', data, 20), (1, 2, 3))
    def test_nonfinite_rejected(self):
        raw = (ROOT / 'assets/original/harbor-npc-tags/gor_00.s').read_bytes()
        start = raw.index(b'<position>') + len(b'<position>')
        end = raw.index(b'</>', start)
        with self.assertRaises(ValueError):
            encode_field_npc_tags(raw[:start] + b' nan 2 3 ' + raw[end:])
if __name__ == '__main__':
    unittest.main()
