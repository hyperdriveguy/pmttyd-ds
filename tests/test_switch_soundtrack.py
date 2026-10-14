import hashlib
import json
import re
from pathlib import Path
import struct
import sys
import tempfile
import unittest
import wave

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from switch_soundtrack import package


class SwitchSoundtrackTests(unittest.TestCase):
    def test_config_matches_native_track_id_order(self):
        root = Path(__file__).resolve().parents[1]
        config = json.loads((root / 'config/switch_soundtrack.json').read_text())
        paths = re.findall(r'"romfs:/soundtrack-switch/([^"\n]+)\.wav"',
                           (root / 'include/audio/audio_policy.hpp').read_text())
        self.assertEqual(paths, [alias for alias, _ in config['tracks']])

    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.library = self.root / 'library'
        self.output = self.root / 'romfs'
        self.output.mkdir()
        self.config = self.root / 'config.json'
        rows = []
        bindings = []
        for i in range(18):
            path = self.library / f'pcm/stream/{i}.wav'
            path.parent.mkdir(parents=True, exist_ok=True)
            with wave.open(str(path), 'wb') as w:
                w.setparams((2, 2, 48000, 0, 'NONE', 'not compressed'))
                w.writeframes(struct.pack('<8h', 1, 2, 3, 4, 5, 6, 7, 8))
            loop = [1, 4] if i % 2 else None
            if loop:
                Path(str(path) + '.loop').write_bytes(struct.pack('<II', *loop))
            rows.append(dict(output=f'pcm/stream/{i}.wav', sound_names=[f'BGM_{i}'],
                rate=48000, channels=2, samples=4, loop=loop, loop_enabled=bool(loop),
                pcm_sha256=hashlib.sha256(path.read_bytes()).hexdigest()))
            bindings.append([f'track-{i}', f'BGM_{i}'])
        (self.library / 'manifest.json').write_text(json.dumps(dict(audio=rows)))
        self.config.write_text(json.dumps(dict(schema=1, tracks=bindings, selection_scope='test adapter')))

    def run_pack(self):
        return package(self.library, self.output, self.config)

    def test_copy_loops_and_repair_cached_products(self):
        self.assertEqual(len(self.run_pack()), 18)
        self.assertEqual((self.output / 'soundtrack-switch.bin').read_bytes(), b'SWM1')
        target = self.output / 'soundtrack-switch/track-0.wav'
        self.assertEqual(target.read_bytes(), (self.library / 'pcm/stream/0.wav').read_bytes())
        self.assertEqual(Path(str(target.with_name('track-1.wav')) + '.loop').read_bytes(), struct.pack('<II', 1, 4))
        target.write_bytes(b'corrupt')
        Path(str(target) + '.loop').write_bytes(b'stale')
        self.run_pack()
        self.assertFalse(Path(str(target) + '.loop').exists())
        self.assertEqual(target.read_bytes(), (self.library / 'pcm/stream/0.wav').read_bytes())

    def test_reject_corrupt_source_before_publication(self):
        (self.library / 'pcm/stream/17.wav').write_bytes(b'corrupt')
        with self.assertRaisesRegex(ValueError, 'hash mismatch'):
            self.run_pack()
        self.assertFalse((self.output / 'soundtrack-switch.bin').exists())
        self.assertFalse((self.output / 'soundtrack-switch').exists())

    def test_reject_invalid_loops_and_unknown_names(self):
        sidecar = self.library / 'pcm/stream/1.wav.loop'
        sidecar.write_bytes(struct.pack('<II', 4, 1))
        with self.assertRaisesRegex(ValueError, 'sidecar mismatch'):
            self.run_pack()
        sidecar.write_bytes(struct.pack('<II', 1, 4))
        config = json.loads(self.config.read_text())
        config['tracks'][0][1] = 'MISSING'
        self.config.write_text(json.dumps(config))
        with self.assertRaisesRegex(ValueError, 'Unknown'):
            self.run_pack()

    def test_gamecube_removes_switch_only_and_preserves_sfx(self):
        self.run_pack()
        (self.output / 'jump.wav').write_bytes(b'original sfx')
        (self.output / 'rogueport.wav').write_bytes(b'original music')
        package(self.library, self.output, self.config, 'gamecube')
        self.assertEqual((self.output / 'soundtrack-switch.bin').read_bytes(), b'GCM1')
        self.assertFalse((self.output / 'soundtrack-switch').exists())
        self.assertEqual((self.output / 'jump.wav').read_bytes(), b'original sfx')
        self.assertEqual((self.output / 'rogueport.wav').read_bytes(), b'original music')
        self.run_pack()
        self.assertEqual((self.output / 'soundtrack-switch.bin').read_bytes(), b'SWM1')
        self.assertEqual(len(list((self.output / 'soundtrack-switch').glob('*.wav'))), 18)
