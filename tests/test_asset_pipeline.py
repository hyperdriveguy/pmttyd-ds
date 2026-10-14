import importlib.util
import json
from pathlib import Path
import struct
import sys
import tempfile
import unittest
import wave

from PIL import Image

TOOLS = Path(__file__).resolve().parents[1] / 'tools'
sys.path.insert(0, str(TOOLS))
import asset_pipeline as pipeline
from asset_animation import decode, encode


class AssetPipelineTests(unittest.TestCase):
    def test_manifest_rejects_escape_duplicate_and_missing_rights(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory); (root / 'a.bin').write_bytes(b'x')
            path = root / 'manifest.json'
            entry = dict(id='one', kind='pack', source='a.bin', output='a.bin', license='test')
            for rows in ([dict(entry, source='../a.bin')], [dict(entry, output='x//a.bin')], [dict(entry, output='../outside')],
                         [entry, dict(entry, id='two')], [dict(entry, license='')],
                         [dict(entry, inherit_original_visibility=True)]):
                path.write_text(json.dumps(dict(schema=1, assets=rows)))
                with self.assertRaises(ValueError): pipeline.read_manifest(path, root)

    def test_model_roundtrip_and_bounds(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory); prefix = root / 'model'
            doc = dict(schema=1, texture_count=1, vertices=[[0,0,0,0,0,1,1,1,1]]*3,
                       batches=[[0,3,0,0,0,0]])
            pipeline.compile_model(doc, prefix)
            pipeline.export_model(prefix, root / 'model.json')
            self.assertEqual(json.loads((root / 'model.json').read_text()), doc)
            for batches in ([[0,6,0,0,0,0]], [[0,3,1,0,0,0]], [[0,3,0,0,4,0]], [[1,2,0,0,0,0]]):
                with self.assertRaises(ValueError): pipeline.compile_model(dict(doc,batches=batches),prefix)

    def test_original_visibility_survives_reordered_edited_model(self):
        with tempfile.TemporaryDirectory() as directory:
            prefix = Path(directory) / 'gor_02'
            a = [[0,0,0,0,0,1,1,1,1], [1,0,0,0,0,1,1,1,1], [0,1,0,0,0,1,1,1,1]]
            b = [[v[0]+2,*v[1:]] for v in a]
            original = dict(schema=1, texture_count=1, vertices=a+b,
                            batches=[[0,3,0,13<<26,0,0], [3,3,0,7<<26,0,0]])
            pipeline.compile_model(original, prefix)
            edited = dict(original, vertices=b+a, batches=[[0,6,0,1<<24,1,2]])
            edited['vertices'][0] = [*b[0][:3], .5,.75,.2,.3,.4,1]
            result = pipeline.inherit_model_visibility(edited, prefix)
            self.assertEqual(result['vertices'], edited['vertices'])
            self.assertEqual(result['batches'], [[0,3,0,(7<<26)|(1<<24),1,2],
                                                [3,3,0,(13<<26)|(1<<24),1,2]])
            pipeline.compile_model(result, prefix)
            moved = dict(edited, vertices=[[v[0]+10,*v[1:]] for v in edited['vertices']])
            self.assertEqual(pipeline.inherit_model_visibility(moved, prefix)['batches'], edited['batches'])
            for invalid in ([0,1,0,0,0,0], [0,3,0,-1,0,0], [0,3,0,1<<32,0,0]):
                with self.assertRaises(ValueError):
                    pipeline.inherit_model_visibility(dict(edited,batches=[invalid]), prefix)
            ambiguous = dict(original, vertices=a+a)
            pipeline.compile_model(ambiguous, prefix)
            with self.assertRaisesRegex(ValueError, 'Ambiguous'):
                pipeline.inherit_model_visibility(edited, prefix)

    def test_obj_negative_indices_uv_and_materials(self):
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / 'model.obj'
            source.write_text('v 0 0 0\nv 1 0 0\nv 0 1 0\nvt 0 0\nvt 1 0\nvt 0 1\nusemtl paper\nf -3/-3 -2/-2 -1/-1\n')
            doc = pipeline.model_from_obj(source, dict(texture_count=1, scale=.1, materials={'paper':{'texture':0}}))
            self.assertEqual(doc['vertices'][1], [.1,0,0,1,0,1,1,1,1])
            self.assertEqual(doc['batches'], [[0,3,0,0,0,0]])
            with self.assertRaises(ValueError): pipeline.model_from_obj(source,dict(texture_count=1))

    def test_actor_roundtrip_and_uvs(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory); target=root/'pose.pmmesh'
            doc=dict(schema=1,poses=[dict(name='idle',vertices=[[0,0,0,0,1]]*3)])
            pipeline.compile_actor(doc,target);pipeline.export_actor(target,root/'pose.json')
            self.assertEqual(json.loads((root/'pose.json').read_text()),doc)
            doc['poses'][0]['vertices'][0][3]=2
            with self.assertRaises(ValueError):pipeline.compile_actor(doc,target)

    def test_atlas_edge_extrusion_and_overlap(self):
        with tempfile.TemporaryDirectory() as directory:
            root=Path(directory);Image.new('RGBA',(2,2),(20,30,40,255)).save(root/'sprite.png')
            doc=dict(size=[8,8],padding=2,frames=[dict(name='a',source='sprite.png',position=[2,2])])
            source=root/'atlas.json';source.write_text(json.dumps(doc))
            image, metadata=pipeline.make_atlas(source,root,root)
            with Image.open(image) as atlas:
                self.assertEqual(atlas.getpixel((0,0)),(20,30,40,255))
                self.assertEqual(atlas.getpixel((5,5)),(20,30,40,255))
                self.assertEqual(atlas.getpixel((6,6)),(0,0,0,0))
            self.assertEqual(metadata['frames']['a']['uv'],[.25,.5,.5,.75])
            doc['frames'].append(dict(name='b',source='sprite.png',position=[3,3]))
            source.write_text(json.dumps(doc))
            with self.assertRaises(ValueError):pipeline.make_atlas(source,root,root)

    def test_audio_canonical_header_loop_and_one_shot(self):
        with tempfile.TemporaryDirectory() as directory:
            root=Path(directory);source=root/'source.wav';target=root/'target.wav'
            with wave.open(str(source),'wb') as out:
                out.setparams((2,2,32000,8,'NONE','not compressed'));out.writeframes(bytes(32))
            pipeline.compile_audio(source,dict(loop=[2,8]),target)
            raw=target.read_bytes()
            self.assertEqual(raw[36:40],b'data');self.assertEqual(len(raw),44+32)
            self.assertEqual(Path(str(target)+'.loop').read_bytes(),struct.pack('<II',2,8))
            with self.assertRaises(ValueError):pipeline.compile_audio(source,dict(loop=[2,9]),target)

    def test_overlay_removal_restores_original_and_deletes_addition(self):
        with tempfile.TemporaryDirectory() as directory:
            root=Path(directory);output=root/'romfs';output.mkdir();work=root/'work'
            (root/'source.bin').write_bytes(b'edited');(output/'original.bin').write_bytes(b'original')
            manifest=root/'manifest.json'
            entries=[dict(id=name,kind='pack',source='source.bin',output=name,license='test') for name in ['original.bin','new.bin']]
            manifest.write_text(json.dumps(dict(schema=1,assets=entries)))
            pipeline.build(manifest,root,output,work,'unused')
            self.assertEqual((output/'original.bin').read_bytes(),b'edited')
            pipeline.prepare(output,work)
            self.assertEqual((output/'original.bin').read_bytes(),b'original');self.assertFalse((output/'new.bin').exists())
            pipeline.build(manifest,root,output,work,'unused')
            self.assertTrue(all(row['cached'] for row in json.loads((work/'build.json').read_text())['assets']))
            (output/'new.bin').write_bytes(b'external')
            with self.assertRaises(ValueError):pipeline.prepare(output,work)
            self.assertEqual((output/'new.bin').read_bytes(),b'external')

    def test_invalid_second_asset_does_not_install_first(self):
        with tempfile.TemporaryDirectory() as directory:
            root=Path(directory);output=root/'romfs';output.mkdir();(output/'a').write_bytes(b'base')
            (root/'a.bin').write_bytes(b'changed');(root/'bad.json').write_text('{}')
            entries=[dict(id='a',kind='pack',source='a.bin',output='a',license='test'),dict(id='bad',kind='model',source='bad.json',output='bad',license='test')]
            manifest=root/'manifest.json';manifest.write_text(json.dumps(dict(schema=1,assets=entries)))
            with self.assertRaises(ValueError):pipeline.build(manifest,root,output,root/'work','unused')
            self.assertEqual((output/'a').read_bytes(),b'base');self.assertFalse((root/'work/installed.json').exists())

    def test_source_edit_invalidates_cache_and_removing_loop_clears_sidecar(self):
        with tempfile.TemporaryDirectory() as directory:
            root=Path(directory); output=root/'romfs'; work=root/'work'; source=root/'sound.wav'
            with wave.open(str(source),'wb') as out:
                out.setparams((1,2,22050,8,'NONE','not compressed'));out.writeframes(bytes(16))
            entry=dict(id='sound',kind='sfx',source='sound.wav',output='sound.wav',license='test',loop=[0,8])
            manifest=root/'manifest.json'; manifest.write_text(json.dumps(dict(schema=1,assets=[entry])))
            pipeline.build(manifest,root,output,work,'unused'); pipeline.prepare(output,work)
            entry.pop('loop'); manifest.write_text(json.dumps(dict(schema=1,assets=[entry])))
            pipeline.build(manifest,root,output,work,'unused')
            self.assertFalse((output/'sound.wav.loop').exists())
            self.assertFalse(json.loads((work/'build.json').read_text())['assets'][0]['cached'])
            pipeline.prepare(output,work)
            with wave.open(str(source),'wb') as out:
                out.setparams((1,2,22050,8,'NONE','not compressed'));out.writeframes(b'\1\0'*8)
            pipeline.build(manifest,root,output,work,'unused')
            self.assertFalse(json.loads((work/'build.json').read_text())['assets'][0]['cached'])
            with wave.open(str(output/'sound.wav'),'rb') as pcm:self.assertEqual(pcm.readframes(8),b'\1\0'*8)

    def test_nested_asset_edit_updates_root_package_stamp(self):
        with tempfile.TemporaryDirectory() as directory:
            root=Path(directory); output=root/'romfs'; work=root/'work'
            source=root/'a.bin'; source.write_bytes(b'first')
            manifest=root/'manifest.json'; manifest.write_text(json.dumps(dict(schema=1,assets=[
                dict(id='a',kind='pack',source='a.bin',output='nested/a.bin',license='test')])))
            pipeline.build(manifest,root,output,work,'unused')
            first=(output/'asset-pipeline-stamp.json').read_bytes()
            pipeline.prepare(output,work);source.write_bytes(b'second')
            pipeline.build(manifest,root,output,work,'unused')
            self.assertNotEqual((output/'asset-pipeline-stamp.json').read_bytes(),first)
            pipeline.prepare(output,work)
            second=(output/'asset-pipeline-stamp.json').read_bytes()
            pipeline.build(manifest,root,output,work,'unused')
            self.assertEqual((output/'asset-pipeline-stamp.json').read_bytes(),second)

    def test_import_refuses_to_overwrite_authored_sources(self):
        from import_editable_assets import import_assets
        with tempfile.TemporaryDirectory() as directory:
            root=Path(directory); destination=root/'original'; destination.mkdir()
            (destination/'authored.png').write_bytes(b'edited')
            with self.assertRaises(ValueError):import_assets(root,{},destination)
            self.assertEqual((destination/'authored.png').read_bytes(),b'edited')

    def test_corrupt_baseline_is_preserved_and_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            root=Path(directory); output=root/'romfs';output.mkdir();work=root/'work'
            (output/'a.bin').write_bytes(b'original');(root/'source.bin').write_bytes(b'edited')
            manifest=root/'manifest.json';manifest.write_text(json.dumps(dict(schema=1,assets=[
                dict(id='a',kind='pack',source='source.bin',output='a.bin',license='test')])))
            pipeline.build(manifest,root,output,work,'unused')
            (work/'baseline/a.bin').write_bytes(b'corrupted')
            with self.assertRaises(ValueError):pipeline.prepare(output,work)
            self.assertEqual((output/'a.bin').read_bytes(),b'edited')

    def test_animation_lossless_sparse_keys_and_rejected_binding(self):
        clip=dict(name='idle',loop=1,start=0,end=10,positions=[[0,0,0]],normals=[],nodes=[0],visibility=[],
                  groups=[[-1,-1,0,0,0]],vertices=[[0,0,0,0,0]]*3,palettes=[[0]+[65535]*256],
                  keys=[dict(time=1,position=[[0,1,-1,0,0]],normal=[],node=[],texture=[[0,0]],visibility=[])])
        for fmt in ('PMANIM3','PMANIM4'):
            if fmt=='PMANIM4':
                clip.update(textures=[[0,0,1,1,0,0,0]],colors=[[255]*4]*3,surfaces=[[2,3]])
                clip['keys'][0]['uv']=[[0,0]]
            doc=dict(schema=1,format=fmt,clips=[clip]);raw=encode(doc)
            self.assertEqual(encode(decode(raw)),raw)
        clip['keys'][0]['position'][0][0]=5
        with self.assertRaises(ValueError):encode(doc)


if __name__=='__main__':unittest.main()
