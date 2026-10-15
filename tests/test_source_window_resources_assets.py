import hashlib
import importlib.util
import json
from pathlib import Path
import sys
import tempfile
import unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from source_window_resources_assets import export,SOURCE_SHA256,SOURCE_SIZE,PRODUCT
from iso_assets import DEFAULT_ISO

class SourceWindowResources(unittest.TestCase):
    def test_actual_original_export_and_exact_reuse(self):
        with tempfile.TemporaryDirectory() as temporary:
            destination=Path(temporary)/'raw'
            metadata=export(DEFAULT_ISO,destination)
            raw=(destination/PRODUCT).read_bytes()
            self.assertEqual(len(raw),SOURCE_SIZE)
            self.assertEqual(hashlib.sha256(raw).hexdigest(),SOURCE_SHA256)
            self.assertEqual(metadata['source_path'],'f/msgWindow.tpl')
            self.assertEqual(metadata['source_offset'],947396944)
            self.assertEqual(metadata['format'],'original-unmodified-TPL')
            self.assertEqual(raw[:12],bytes.fromhex('0020af30000000330000000c'))
            self.assertEqual(export(DEFAULT_ISO,destination),metadata)
            self.assertEqual(json.loads((destination/'source-window.json').read_text()),metadata)

    def test_preserve_edits_metadata_extra_and_truncation(self):
        for mutation in ['payload','metadata','extra','truncated']:
            with self.subTest(mutation=mutation),tempfile.TemporaryDirectory() as temporary:
                destination=Path(temporary)/'raw'
                export(DEFAULT_ISO,destination)
                if mutation=='payload':
                    data=bytearray((destination/PRODUCT).read_bytes());data[8]^=1;(destination/PRODUCT).write_bytes(data)
                elif mutation=='metadata':
                    (destination/'source-window.json').write_text('{}')
                elif mutation=='extra':
                    (destination/'original-edit.txt').write_text('keep')
                else:
                    (destination/PRODUCT).write_bytes(b'keep')
                before={p.name:p.read_bytes() for p in destination.iterdir()}
                with self.assertRaises(ValueError):
                    export(DEFAULT_ISO,destination)
                self.assertEqual(before,{p.name:p.read_bytes() for p in destination.iterdir()})

if __name__=='__main__':
    unittest.main()
