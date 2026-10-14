import struct
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from battle_pose_directory import source_directory, write_directory


class BattlePoseDirectoryTests(unittest.TestCase):
    def model(self):
        raw = bytearray(0x400)
        raw[4:11] = b'model\0\0'
        raw[0x44:0x4c] = b'texture\0'
        struct.pack_into('>I', raw, 0x148, 3)
        struct.pack_into('>I', raw, 0x1ac, 0x1b0)
        struct.pack_into('>I', raw, 0xc4, 9)
        for i, name in enumerate((b'default', b'unconverted', b'walking')):
            start = 0x1b0 + i * 64
            raw[start:start + len(name)] = name
            animation = 0x270 + i * 0x80
            struct.pack_into('>I', raw, start + 60, animation)
            struct.pack_into('>I', raw, animation + 0x20, 1)
            struct.pack_into('>I', raw, animation + 0x40, 0x50)
            struct.pack_into('>fI', raw, animation + 0x50, float(i), 0x12345678 + i)
        return raw

    def test_draw_companion_preserves_original_bytes(self):
        with tempfile.TemporaryDirectory() as folder:
            source = Path(folder) / 'model'
            raw = self.model()
            source.write_bytes(raw)
            write_directory(source, Path(folder) / 'actor.pose-directory.bin')
            self.assertEqual((Path(folder) / 'actor.draw-model.bin').read_bytes(), raw)

    def test_preserves_source_order_and_resource_names(self):
        raw = self.model()
        self.assertEqual(source_directory(raw),
                         ('model', 'texture', ['default', 'unconverted', 'walking']))
        with tempfile.TemporaryDirectory() as folder:
            source, output = Path(folder) / 'model', Path(folder) / 'directory'
            source.write_bytes(raw)
            write_directory(source, output)
            result = output.read_bytes()
            self.assertEqual(result[:8], b'PMPOSE02')
            self.assertEqual(struct.unpack_from('<I', result, 136)[0], 3)
            self.assertEqual(result[204:268].rstrip(b'\0'), b'unconverted')
            self.assertEqual(struct.unpack_from('<I', result, 332)[0], 9)
            self.assertEqual(struct.unpack_from('<IfI', result, 336), (1, 0., 0x12345678))
            self.assertEqual(struct.unpack_from('<IfI', result, 360), (1, 2., 0x1234567a))

    def test_rejects_invalid_timeline_extent(self):
        for field in (0x20, 0x40):
            raw = self.model()
            struct.pack_into('>I', raw, 0x270 + field, 0xffffffff)
            with tempfile.TemporaryDirectory() as folder:
                source = Path(folder) / 'model'
                source.write_bytes(raw)
                with self.assertRaises(ValueError):
                    write_directory(source, Path(folder) / 'directory')

    def test_rejects_invalid_source_extents_and_names(self):
        for change in ('header', 'extent', 'name', 'empty'):
            raw = self.model()
            if change == 'header':
                raw = raw[:16]
            elif change == 'extent':
                struct.pack_into('>I', raw, 0x148, 0xffffffff)
            elif change == 'name':
                raw[0x1b0:0x1b0 + 60] = b'A' * 60
            else:
                raw[4:68] = b'\0' * 64
            with self.subTest(change=change), self.assertRaises(ValueError):
                source_directory(raw)

    def test_local_original_directories(self):
        root = Path(__file__).resolve().parents[1] / 'local-assets/trio-battle'
        if not all((root / name).exists() for name in ('b_mario', 'c_pkr')):
            self.skipTest('Private original model extracts are unavailable')
        mario = source_directory((root / 'b_mario').read_bytes())
        partner = source_directory((root / 'c_pkr').read_bytes())
        self.assertEqual(mario[:2], ('b_mario', 'a_mario'))
        self.assertEqual(partner[:2], ('c_pkr', 'c_pkr'))
        self.assertEqual((len(mario[2]), mario[2][0]), (85, 'M_Z_1'))
        self.assertEqual((len(partner[2]), partner[2][0]), (32, 'PKR_Z_1'))
        self.assertNotIn('KRN_H_1', mario[2])

    def test_original_frankly_complete_bank(self):
        root = Path(__file__).resolve().parents[1]
        source = root / 'local-assets/professor/c_kuri_h'
        if not source.exists():
            self.skipTest('Private original Frankly model is unavailable')
        model, texture, names = source_directory(source.read_bytes())
        self.assertEqual((model, texture, len(names), names[0], names[-1]),
                         ('c_kuri_h', 'c_kuri_h', 17, 'Z_1', 'D_2'))
        directory = root / 'romfs/professor.pose-directory.bin'
        if directory.exists():
            data = directory.read_bytes()
            self.assertEqual(data[:8], b'PMPOSE02')
            self.assertEqual(struct.unpack_from('<I', data, 136)[0], 17)
            actual = [data[140+i*64:204+i*64].rstrip(b'\0').decode('ascii') for i in range(17)]
            self.assertEqual(actual, names)


if __name__ == '__main__':
    unittest.main()
