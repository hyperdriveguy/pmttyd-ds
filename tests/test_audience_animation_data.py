import hashlib
import struct
import unittest
from pathlib import Path

class AudienceAnimationData(unittest.TestCase):
    def test_original_script_owner_imported_without_native_global_tail(self):
        for filename in ['trio-attack-events.pmevt', 'harbor-tutorial-events.pmevt']:
            raw = Path('romfs', filename).read_bytes()
            self.assertEqual(raw[:8], b'PMEVT001')
            count, = struct.unpack_from('>I', raw, 8)
            spans = [struct.unpack_from('>III', raw, 12 + i*12) for i in range(count)]
            matched = [(size, offset) for address, size, offset in spans if address == 0x8041c638]
            self.assertEqual(len(matched), 1, filename)
            size, offset = matched[0]
            self.assertEqual(size, 0x14e8)
            self.assertEqual(hashlib.sha256(raw[offset:offset+size]).hexdigest(),
                             '27efa23577efb6deb3204baf406fdda61546d5700ec4c25f04e8b59fad724cc4')
            for native in [0x8041eb90, 0x8041eb00, 0x8041f070]:
                self.assertFalse(any(address <= native < address+size for address,size,_ in spans))

if __name__ == '__main__':
    unittest.main()
