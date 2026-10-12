"""Research tooling boundaries: never execute upstream configuration."""
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from decomp_coverage import audit


class CoverageAuditTests(unittest.TestCase):
    def test_declared_status_is_separate_from_source_presence(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            # An executable import would fail. The audit must only parse this.
            (root / 'configure.py').write_text('''
raise RuntimeError("must never run")
config.libs = [
 {"lib": "game", "progress_category": "game", "objects": [
  Object(Matching, "missing.c"), Object(NonMatching, "partial.c")]},
 {"lib": "sdk", "progress_category": "sdk", "objects": [
  Object(Matching, "sdk.c")]},
 Rel("gor", [Object(NonMatching, "area.c")])]
''')
            (root / 'src').mkdir()
            (root / 'src/partial.c').write_text('void partial(void) {}\n')
            (root / 'rels/gor/src').mkdir(parents=True)
            (root / 'rels/gor/src/area.c').write_text('/* placeholder */\n')
            result = audit(root)
            self.assertEqual(result['declarations'], {'Matching': 1, 'NonMatching': 2})
            self.assertEqual((result['present'], result['absent']), (2, 1))
            self.assertEqual(result['objects'][2]['path'], 'rels/gor/src/area.c')
            self.assertIsNone(result['objects'][0]['sha256'])

    def test_path_escape_is_rejected(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / 'configure.py').write_text('''
config.libs = [{"lib": "game", "progress_category": "game",
"objects": [Object(NonMatching, "../../outside.c")]}]
''')
            with self.assertRaisesRegex(ValueError, 'escapes'):
                audit(root)

    def test_unsupported_config_is_not_silent_empty_coverage(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / 'configure.py').write_text('config.libs = make_libs()\n')
            with self.assertRaisesRegex(ValueError, 'Unsupported'):
                audit(root)


if __name__ == '__main__':
    unittest.main()
