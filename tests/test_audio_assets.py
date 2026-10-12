"""Audio fixtures contain authored DSP data, never original game assets."""
from pathlib import Path
import struct
import sys
import tempfile
import unittest
import wave

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from audio_convert import stm, sample, sample_directory
from extract_audio import verify_pcm


def stream_fixture(enabled=True):
    raw = bytearray(320)
    struct.pack_into('>HHII', raw, 0, 0x200, 32000, 2, 8)
    for channel in range(2):
        header = 64 + channel * 96
        struct.pack_into('>IIIHHIII', raw, header, 14, 16, 32000, enabled, 0, 4, 15, 2)
        raw[256 + channel * 32:264 + channel * 32] = bytes([0, 0x11 * (channel + 1)]) + bytes(6)
    return raw


def bank_fixture(length=10):
    raw = bytearray(108)
    struct.pack_into('>H', raw, 0, 42)
    struct.pack_into('>HIIII', raw, 14, 16000, 14, 2, length, 68)
    raw[32:36] = b'\xff' * 4
    return raw


class AudioAssetTests(unittest.TestCase):
    def test_stereo_loop_and_sample_order(self):
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / 'source.stm'; pcm = Path(directory) / 'out.wav'
            source.write_bytes(stream_fixture())
            record = stm(source, pcm)
            self.assertEqual(record['loop'], (2, 14))
            verify_pcm(pcm, record)
            with wave.open(str(pcm), 'rb') as wav:
                self.assertEqual(struct.unpack('<4h', wav.readframes(2)), (1, 2, 1, 2))
            self.assertEqual(pcm.stat().st_size, 44 + 14 * 2 * 2)
            # The exported interval preserves the intro then repeats only 2..13.
            self.assertEqual(struct.unpack('<II', Path(str(pcm) + '.loop').read_bytes()), (2, 14))

    def test_nonloop_removes_stale_sidecar_and_source_is_protected(self):
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / 'source.stm'; pcm = Path(directory) / 'out.wav'
            source.write_bytes(stream_fixture(False))
            Path(str(pcm) + '.loop').write_bytes(b'stale')
            record = stm(source, pcm)
            self.assertFalse(record['loop_enabled']); self.assertIsNone(record['loop'])
            verify_pcm(pcm, record)
            with self.assertRaises(ValueError): stm(source, source)
            self.assertEqual(source.read_bytes(), stream_fixture(False))

    def test_invalid_loop_flags_bounds_and_truncated_stream(self):
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / 'source.stm'; pcm = Path(directory) / 'out.wav'
            for offset, fmt, value in [(172, 'H', 0), (80, 'I', 40), (76, 'H', 2)]:
                raw = stream_fixture(); struct.pack_into('>' + fmt, raw, offset, value)
                source.write_bytes(raw)
                with self.assertRaises(ValueError): stm(source, pcm)
            for raw in [b'\x02\x00', stream_fixture()[:295]]:
                source.write_bytes(raw)
                with self.assertRaises(ValueError): stm(source, pcm)

    def test_musyx_looped_and_single_sample_nonloop(self):
        with tempfile.TemporaryDirectory() as directory:
            sdir = Path(directory) / 'bank.sdir'; samp = Path(directory) / 'bank.samp'
            pcm = Path(directory) / 'sample.wav'
            samp.write_bytes(bytes([0, 0x11]) + bytes(6))
            sdir.write_bytes(bank_fixture())
            record = sample(sdir, samp, 42, pcm)
            self.assertEqual(record['loop'], [2, 12]); self.assertEqual(record['id'], 42)
            verify_pcm(pcm, record)
            sdir.write_bytes(bank_fixture(1))
            verify_pcm(pcm, sample(sdir, samp, 42, pcm))
            self.assertFalse(Path(str(pcm) + '.loop').exists())
            with self.assertRaises(ValueError): sample(sdir, samp, 41, pcm)
            with self.assertRaises(ValueError): sample(sdir, samp, 42, samp)
            samp.write_bytes(bytes(7))
            with self.assertRaises(ValueError): sample(sdir, samp, 42, pcm)

    def test_bank_bounds_and_pcm_validation(self):
        with tempfile.TemporaryDirectory() as directory:
            sdir = Path(directory) / 'bank.sdir'
            sdir.write_bytes(bank_fixture(13))
            with self.assertRaises(ValueError): sample_directory(sdir)
            source = Path(directory) / 'source.stm'; pcm = Path(directory) / 'out.wav'
            source.write_bytes(stream_fixture()); record = stm(source, pcm)
            Path(str(pcm) + '.loop').write_bytes(struct.pack('<II', 0, 15))
            with self.assertRaises(ValueError): verify_pcm(pcm, record)
            pcm.write_bytes(pcm.read_bytes()[:-2])
            with self.assertRaises(ValueError): verify_pcm(pcm, record)


if __name__ == '__main__':
    unittest.main()
