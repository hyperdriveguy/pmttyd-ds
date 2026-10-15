import copy
import hashlib
import struct
import sys
import tempfile
import unittest
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from recovery_effect_assets import SPANS, encode, export, read_original


class RecoveryAssets(unittest.TestCase):
    def synthetic_iso(self, path):
        raw = bytearray(0x1000)
        raw[:8] = b'G8ME01\0\0'
        struct.pack_into('>I', raw, 0x420, 0x500)
        payload = bytearray()
        for i, (_, address, size, _) in enumerate(SPANS):
            struct.pack_into('>I', raw, 0x500 + i * 4, 0x100 + len(payload))
            struct.pack_into('>I', raw, 0x548 + i * 4, address)
            struct.pack_into('>I', raw, 0x590 + i * 4, size)
            payload.extend(bytes((j+i) % 256 for j in range(size)))
        raw[0x600:0x600+len(payload)] = payload
        path.write_bytes(raw)
        return bytes(payload)

    def test_lossless_original_address_roundtrip(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            iso = root/'test.iso'
            expected = self.synthetic_iso(iso)
            before = iso.read_bytes()
            source = export(iso, root/'import')
            packed = encode(source)
            self.assertEqual(packed[:8], b'PMREC001')
            self.assertEqual(struct.unpack_from('<I', packed, 8)[0], len(SPANS))
            # Packing sorts spans by address, while preserving every original byte.
            chunks = []
            at = 12 + len(SPANS)*8
            for i in range(len(SPANS)):
                address, size = struct.unpack_from('<II', packed, 12+8*i)
                chunks.append((address, packed[at:at+size]))
                at += size
            self.assertEqual(b''.join(data for _, data in sorted(chunks)), expected)
            self.assertEqual(iso.read_bytes(), before)
            self.assertEqual((root/'import/recovery-effect.bin').read_bytes(), packed)
            for row, (_, data) in zip(source['spans'], chunks):
                self.assertEqual(row['original_sha256'], hashlib.sha256(data).hexdigest())
            with self.assertRaises(ValueError):
                export(iso, root/'import')
            self.assertEqual(iso.read_bytes(), before)

    def test_fail_closed(self):
        with tempfile.TemporaryDirectory() as tmp:
            iso = Path(tmp)/'test.iso'
            self.synthetic_iso(iso)
            source = read_original(iso)
            cases = []
            bad = copy.deepcopy(source);bad['spans'][0]['values'][0] = -1;cases.append(bad)
            bad = copy.deepcopy(source);bad['spans'][0]['values'][0] = True;cases.append(bad)
            bad = copy.deepcopy(source);bad['spans'][0]['encoding'] = 'f32';cases.append(bad)
            bad = copy.deepcopy(source);bad['spans'][0]['values'] = [];cases.append(bad)
            bad = copy.deepcopy(source);bad['spans'][1]['address'] = bad['spans'][0]['address'];cases.append(bad)
            bad = copy.deepcopy(source);bad['spans'][0]['address'] = 'fffffffc';cases.append(bad)
            bad = copy.deepcopy(source);bad['license'] = '';cases.append(bad)
            for bad in cases:
                with self.assertRaises(ValueError):
                    encode(bad)
            raw = bytearray(iso.read_bytes());raw[0] = 0;iso.write_bytes(raw)
            with self.assertRaises(ValueError):read_original(iso)
            self.synthetic_iso(iso);raw = bytearray(iso.read_bytes());struct.pack_into('>I', raw, 0x590, 1);iso.write_bytes(raw)
            with self.assertRaises(ValueError):read_original(iso)
            self.synthetic_iso(iso);iso.write_bytes(iso.read_bytes()[:0x605])
            with self.assertRaises(ValueError):read_original(iso)


if __name__ == '__main__':
    unittest.main()
