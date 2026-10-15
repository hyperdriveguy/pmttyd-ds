"""Fingerprint the exact original parent used by the native Turn translation."""
from pathlib import Path
import sys
import unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from iso_assets import DEFAULT_ISO
from verify_battle_turn_controller import verify

class OriginalTurn(unittest.TestCase):
    @unittest.skipUnless(Path(DEFAULT_ISO).is_file(), 'local original ISO unavailable')
    def test_complete_parent_and_established_numeric_base(self):
        result=verify(DEFAULT_ISO)
        self.assertEqual(result['instructions'],1974)
        self.assertEqual(result['dispatch_entries'],78)
        self.assertEqual(result['numeric_base'],'0x80427440')
