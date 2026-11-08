import importlib.util
import json
from pathlib import Path
import tempfile
import sys
import unittest
import wave

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
SPEC = importlib.util.spec_from_file_location('mario_voice_import', ROOT/'tools/import_mario_action_voice_assets.py')
IMPORTER = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(IMPORTER)

class MarioActionVoiceAssets(unittest.TestCase):
    def test_original_fifteen_and_empty_destination(self):
        with tempfile.TemporaryDirectory(prefix='ttyd-original-voices-') as temporary:
            destination = Path(temporary)/'editable'
            record = IMPORTER.build(IMPORTER.DEFAULT_ISO, destination)
            self.assertEqual(len(record['voices']), 15)
            self.assertEqual({r['sample_id'] for r in record['voices']}, set(range(590,605)))
            self.assertEqual(sum(r['pcm']['samples'] for r in record['voices']),209739)
            self.assertLess(sum(r['pcm']['samples'] for r in record['voices']),256*1024)
            self.assertEqual(len(list(destination.glob('*.wav'))),15)
            self.assertEqual(list(destination.glob('*.loop')),[])
            for voice in record['voices']:
                with wave.open(str(destination/voice['file']), 'rb') as pcm:
                    self.assertEqual((pcm.getnchannels(),pcm.getsampwidth(),pcm.getframerate()),(1,2,22050))
                    self.assertEqual(pcm.getnframes(),voice['pcm']['samples'])
                    self.assertEqual(len(pcm.readframes(pcm.getnframes())),2*voice['pcm']['samples'])
                self.assertEqual(voice['pcm_sha256'],IMPORTER.sha((destination/voice['file']).read_bytes()))
                self.assertGreater(voice['encoded_sample']['iso_offset'],0)
                self.assertIn('no redistribution license',voice['license'])
            # A real editor change must survive refused re-import, including a
            # directory containing otherwise valid original products.
            edited=destination/record['voices'][0]['file'];edited.write_bytes(b'editable changes retained')
            with self.assertRaisesRegex(ValueError,'must be empty'):
                IMPORTER.build(IMPORTER.DEFAULT_ISO,destination)
            self.assertEqual(edited.read_bytes(),b'editable changes retained')
            self.assertEqual(json.loads((destination/'provenance.json').read_text())['license'],IMPORTER.LICENSE)

if __name__ == '__main__':
    unittest.main()
