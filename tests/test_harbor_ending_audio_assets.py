import importlib.util
import json
from pathlib import Path
import struct
import tempfile
import unittest

class EndingAudioAssets(unittest.TestCase):
    def test_original_import_and_no_overwrite(self):
        spec = importlib.util.spec_from_file_location('ending_import', Path(__file__).parents[1]/'tools/import_harbor_ending_audio_assets.py')
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        with tempfile.TemporaryDirectory() as temporary:
            dest = Path(temporary)/'ending'
            result = module.build('/home/carson/games/roms/G8ME01.iso', dest)
            self.assertEqual([(r['row']['table_index'],r['sample_id'],r['pcm']['rate']) for r in result['voices']], [(225,635,22050),(226,636,22050),(2616,785,16000)])
            from asset_pipeline import compile_audio
            for row in result['voices']:
                data = (dest/row['file']).read_bytes()
                self.assertEqual(data[:4],b'RIFF')
                self.assertEqual(struct.unpack_from('<I',data,24)[0],row['pcm']['rate'])
                self.assertEqual(module.sha(data),row['pcm_sha256'])
                self.assertFalse(row['pcm']['loop_enabled'])
                compiled=Path(temporary)/row['file']
                compile_audio(dest/row['file'],{},compiled)
                self.assertEqual(compiled.read_bytes(),data)
                self.assertIn('iso_offset',row['encoded_sample'])
            original = (dest/'mario-voice-relief1.wav').read_bytes()
            with self.assertRaises(ValueError):module.build('/home/carson/games/roms/G8ME01.iso',dest)
            self.assertEqual((dest/'mario-voice-relief1.wav').read_bytes(),original)
    def test_catalog_name_scope(self):
        metadata = Path(__file__).parents[1]/'assets/original/audio/harbor-ending/provenance.json'
        # Source checkout test uses imported tracked provenance; raw sample cache
        # or synthesized tones never supply these identities.
        self.assertTrue(metadata.exists())
        result=json.loads(metadata.read_text())
        self.assertEqual([r['name'] for r in result['voices']],['SFX_VOICE_MARIO_RELIEF1','SFX_VOICE_MARIO_RELIEF2','SFX_STAR_POINT_GET1'])

if __name__=='__main__':unittest.main()
