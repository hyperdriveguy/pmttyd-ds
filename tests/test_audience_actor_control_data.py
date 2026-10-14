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
            0x80424554: 0x46fffe00, 0x8042453c: 0x41a00000,
            0x804244ec: 0x40c00000, 0x80424500: 0xc2c80000,
            0x80424504: 0x3dcccccd, 0x80424558: 0xbf000000,
            0x80424560: 0x42f00000, 0x80424568: 0x44160000,
        }.items():
            self.assertEqual(word(address), bits, f'{address:08x}')

        self.assertEqual([word(0x80397af8+4*i) for i in range(10)], [
            0x8019276c, 0x8019276c, 0x8019281c, 0x8019276c, 0x801923dc,
            0x8019276c, 0x80192550, 0x80192604, 0x8019276c, 0x801926b8,
        ])
        for address, bits in {
            0x804244f8: 0x3f800000, 0x80424524: 0x41200000,
            0x80424540: 0x42200000, 0x80424548: 0x41700000,
            0x8042454c: 0x3b23d70a, 0x8042455c: 0x40000000,
            0x80424564: 0x41000000, 0x80424570: 0xbe199999,
            0x80424574: 0x40800000, 0x80424588: 0x3f400000,
            0x8042458c: 0x3f000000, 0x80424590: 0x3e800000,
            0x80424594: 0x3fe00000, 0x80424598: 0x3fa00000,
            0x8042459c: 0x3ba3d70a, 0x804245a0: 0x433c0000,
            0x804245a4: 0x41800000, 0x804245a8: 0x41c00000,
            0x804245ac: 0x42000000, 0x804245b0: 0x42100000,
            0x804245b4: 0x41880000, 0x804245b8: 0x3ae56042,
            0x804245bc: 0x41b00000, 0x804245c0: 0x3b75c28f,
            0x804245c4: 0x42800000, 0x804245c8: 0x41f80000,
            0x804245cc: 0x3aa3d70a,
        }.items():
            self.assertEqual(word(address), bits, f'{address:08x}')


if __name__ == '__main__':
    unittest.main()
