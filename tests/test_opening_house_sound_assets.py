import hashlib,json,unittest,wave
from pathlib import Path

class OriginalHouseSounds(unittest.TestCase):
    @unittest.skipUnless(Path('romfs/opening-house-sounds.json').exists(),'local original house sound pack not built')
    def test_actual_original_samples_and_positions(self):
        data=json.loads(Path('romfs/opening-house-sounds.json').read_text())
        expected=[('flight',972,1278,1435,1176,4614),('postbox',973,527,620,479,8174),('door-open',568,931,1062,840,12764),('door-shut',589,942,1073,851,12365)]
        for cue,(clip,index,fx,obj,sid,frames) in zip(data['cues'],expected):
            self.assertEqual((cue['clip'],cue['table']['table_index'],cue['table']['fx_id'],cue['object']['object_id'],cue['pcm']['id'],cue['pcm']['samples']),(clip,index,fx,obj,sid,frames))
            path=Path('romfs')/('opening-house-'+clip+'.wav')
            with wave.open(str(path)) as w:
                self.assertEqual((w.getnchannels(),w.getsampwidth(),w.getframerate(),w.getnframes()),(1,2,16000,frames))
                self.assertTrue(any(w.readframes(frames)))
            self.assertEqual(hashlib.sha256(path.read_bytes()).hexdigest(),data['products'][path.name])
            self.assertFalse(cue['pcm']['loop_enabled'])
        self.assertEqual([p['name'] for p in data['objects']],['S_ie_door','S_posuto'])
        self.assertTrue(all(p['leaves']>0 and len(p['world'])==12 and len(p['localMean'])==3 for p in data['objects']))
        self.assertEqual(data['environment']['index'],1)
        self.assertEqual(len(data['environment']['rows']),5)
        self.assertEqual(Path('romfs/opening-house-sounds.bin').stat().st_size,404)

if __name__=='__main__':unittest.main()
