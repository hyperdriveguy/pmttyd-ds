import importlib.util
import json
from pathlib import Path
import sys
import tempfile
import unittest
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
from asset_animation import decode,encode
from asset_pose_extensions import append_animation_clips,auxiliary_sources,compile_pose_directory,pose_directory_outputs
from harbor_tutorial_entry_assets import import_assets

class EntryAssets(unittest.TestCase):
    def test_current_edit_preserved(self):
        base=json.loads((ROOT/'assets/original/animations/harbor-tutorial-crump.json').read_text())
        addition=json.loads((ROOT/'assets/original/tutorial-unit-entry/crump-z1.json').read_text())
        base['clips'][0]['loop']=1-base['clips'][0]['loop'] # represents an existing edit
        result=append_animation_clips(base,[addition]);self.assertEqual(result['clips'][:-1],base['clips'])
        old,new=encode(base),encode(result)
        self.assertEqual(new[12:len(old)],old[12:]);self.assertEqual(result['clips'][-1]['name'],'Z_1')
        result['clips'][0]['name']='changed';self.assertNotEqual(result,base)
        with self.assertRaises(ValueError):append_animation_clips(base,[base])
        with self.assertRaises(ValueError):append_animation_clips(base,[dict(addition,format='PMANIM4')])
    def test_auxiliary_validation(self):
        for args in [dict(kind='texture',source='a',append_sources=['b']),dict(kind='animation',source='a',append_sources=['a']),dict(kind='animation',source='a',append_sources=['b','b']),dict(kind='pose-directory',source='a')]:
            with self.assertRaises(ValueError):auxiliary_sources(args)
        self.assertEqual(pose_directory_outputs('foo.pose-directory.bin'),['foo.pose-directory.bin','foo.draw-model.bin','foo.draw-textures.bin'])
        with self.assertRaises(ValueError):pose_directory_outputs('foo.bin')
    def test_source_companions_and_import_refusal(self):
        source=ROOT/'assets/original/tutorial-unit-entry'
        with tempfile.TemporaryDirectory() as d:
            target=Path(d)/'crump.pose-directory.bin';compile_pose_directory(source/'crump.model',target,source/'crump.tpl')
            self.assertEqual((Path(d)/'crump.draw-model.bin').read_bytes(),(source/'crump.model').read_bytes())
            self.assertEqual((Path(d)/'crump.draw-textures.bin').read_bytes(),(source/'crump.tpl').read_bytes())
            self.assertEqual(target.read_bytes()[:8],b'PMPOSE02')
            with self.assertRaises(ValueError):import_assets('/not/opened.iso',Path(d))
if __name__=='__main__':unittest.main()
