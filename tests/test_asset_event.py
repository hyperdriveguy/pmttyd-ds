import hashlib
import json
from pathlib import Path
import struct
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
from asset_event import encode
import asset_pipeline as pipeline
from import_npc_creation_event import import_source

class EventAssetTests(unittest.TestCase):
    def test_original_child_and_scanner_word(self):
        path = ROOT / 'assets/original/events/harbor-npc-creation/event.json'
        source = json.loads(path.read_text())
        child = next(x for x in source['segments'] if x['address'] == 0x90505208)
        raw = bytes.fromhex(child['bytes'])
        self.assertEqual(len(raw), 0xec)  # Complete e8 child plus actual scanner prefetch.
        self.assertEqual(hashlib.sha256(raw[:0xe8]).hexdigest(), 'c18a49a6a80ff6beff980986af1a7658c2b5025522702d264180ff0f297dfd93')
        self.assertEqual(encode(source), (ROOT / 'romfs/field-npc-creation.pmevt').read_bytes())
        zero = next(x for x in source['segments'] if x['address'] == 0x8041fd84)
        self.assertEqual(zero['bytes'], '00000000')

    def test_invalid_extents_and_overlap(self):
        for segments in ([], [dict(address=1, bytes='00000000')], [dict(address=4, bytes='')],
                         [dict(address=0xfffffffc, bytes='00'*8)],
                         [dict(address=4, bytes='00'*8), dict(address=8, bytes='00'*4)]):
            with self.assertRaises(ValueError):
                encode(dict(schema=1, segments=segments))

    def test_build_uses_editable_source(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            source = root / 'event.json'
            source.write_text(json.dumps(dict(schema=1, segments=[dict(address=0x90000000, bytes='00000001')])))
            manifest = root / 'manifest.json'
            manifest.write_text(json.dumps(dict(schema=1, assets=[dict(id='fixture/event', kind='event', source='event.json', output='event.pmevt', license='test fixture') ])))
            output, work = root/'out', root/'work'
            pipeline.build(manifest, root, output, work, 'unused')
            self.assertEqual((output/'event.pmevt').read_bytes()[-4:], bytes.fromhex('00000001'))
            pipeline.prepare(output, work)
            source.write_text(json.dumps(dict(schema=1, segments=[dict(address=0x90000000, bytes='00000002')])))
            pipeline.build(manifest, root, output, work, 'unused')
            self.assertEqual((output/'event.pmevt').read_bytes()[-4:], bytes.fromhex('00000002'))

    def test_import_refuses_existing_edits_before_reading_iso(self):
        with tempfile.TemporaryDirectory() as directory:
            destination = Path(directory)/'edited';destination.mkdir()
            marker = destination/'event.json';marker.write_text('user edit')
            with self.assertRaises(ValueError):
                import_source('/no/iso', '/no/symbols', destination)
            self.assertEqual(marker.read_text(), 'user edit')
