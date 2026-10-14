import struct
import unittest
from pathlib import Path


class AudienceActorControlData(unittest.TestCase):
    def test_original_dispatch_and_motion_constants(self):
        raw = Path('romfs/trio-attack-events.pmevt').read_bytes()
        count, = struct.unpack_from('>I', raw, 8)
        spans = [struct.unpack_from('>III', raw, 12+i*12) for i in range(count)]

        def word(address):
            for start, size, offset in spans:
                if start <= address and address+4 <= start+size:
                    return struct.unpack_from('>I', raw, offset+address-start)[0]
            self.fail(f'Missing original data {address:08x}')

        self.assertEqual([word(0x80397b20+4*i) for i in range(21)],
                         [0x80192a58+12*i for i in range(20)]+[0x80192b48])
        for address, bits in {
            0x804244e8: 0, 0x804244e4: 0x42b40000, 0x804244e0: 0x43870000,
            0x80424508: 0x3d4ccccd, 0x804245d0: 0x40c90fd8,
            0x80424510: 0x40a00000, 0x804244fc: 0xbf800000,
            0x8042457c: 0x4059999a, 0x80424578: 0xbe4ccccd,
            0x80424580: 0x3fc00000, 0x80424584: 0x41400000,
        }.items():
            self.assertEqual(word(address), bits, f'{address:08x}')


if __name__ == '__main__':
    unittest.main()
