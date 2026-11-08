"""Original US source authority for the complete retained target display."""
import sys
import unittest
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from verify_battle_target_display import verify

class TargetDisplaySourceTests(unittest.TestCase):
    def test_whole_parent_linked_children_and_constants(self):
        evidence = verify()
        self.assertEqual(evidence['whole_parent_bytes'], 0x5a0)
        self.assertEqual(evidence['pinned_spans'], 26)
