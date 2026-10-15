import hashlib
import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import source_font_resources_assets as pack

class SourceFontResourcesTests(unittest.TestCase):
    def test_exact_unmodified_pack_and_refuse_changed_existing_destination(self):
        with tempfile.TemporaryDirectory() as directory:
            output = Path(directory) / "font"
            metadata = pack.export(pack.DEFAULT_ISO, output)
            raw = (output / pack.PRODUCT).read_bytes()
            self.assertEqual(len(raw), 66560)
            self.assertEqual(hashlib.sha256(raw).hexdigest(), pack.SOURCE_SHA256)
            self.assertEqual(raw[77], 1)  # Source runtime, not converter, writes7 then9.
            self.assertEqual(metadata["source_path"], "f/papermarioset_US.bfn")
            self.assertEqual(json.loads((output / "source-font.json").read_text()), metadata)
            self.assertEqual(pack.export(pack.DEFAULT_ISO, output), metadata)
            changed = bytearray(raw)
            changed[77] = 9
            (output / pack.PRODUCT).write_bytes(changed)
            with self.assertRaisesRegex(ValueError, "empty destination"):
                pack.export(pack.DEFAULT_ISO, output)
            self.assertEqual((output / pack.PRODUCT).read_bytes(), changed)

    def test_extra_existing_file_is_preserved(self):
        with tempfile.TemporaryDirectory() as directory:
            output = Path(directory)
            (output / "editable.txt").write_text("keep")
            with self.assertRaisesRegex(ValueError, "empty destination"):
                pack.export(pack.DEFAULT_ISO, output)
            self.assertEqual((output / "editable.txt").read_text(), "keep")
            self.assertFalse((output / pack.PRODUCT).exists())

    def test_wrong_extent_or_identity_never_publishes(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            with patch.object(pack, "inventory", return_value={"disc_id": "WRONG", "files": []}):
                with self.assertRaises(ValueError):
                    pack.export(pack.DEFAULT_ISO, root / "wrong")
            self.assertFalse((root / "wrong").exists())
            with patch.object(pack, "SOURCE_SHA256", "0" * 64):
                with self.assertRaisesRegex(ValueError, "identity"):
                    pack.export(pack.DEFAULT_ISO, root / "mismatch")
            self.assertFalse((root / "mismatch").exists())
