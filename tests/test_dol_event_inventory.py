"""Synthetic event-stream boundaries; no execution of original scripts."""
from pathlib import Path
import struct
import sys
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from dol_event_inventory import parse_commands


class DolEventTests(unittest.TestCase):
    def test_preserves_raw_operands_and_static_annotation(self):
        data = struct.pack('>5I', 0x0003005b, 0x80010000, 0xfffffffe, 0xfe363c80, 1)
        commands = parse_commands(data, 0x80300000, {0x80010000: 'fixture_callback'})
        self.assertEqual([c['address'] for c in commands], [0x80300000, 0x80300010])
        self.assertEqual(commands[0]['opcode'], 91)
        self.assertEqual(commands[0]['operands'][0]['annotation'], 'fixture_callback')
        self.assertEqual(commands[0]['operands'][2]['raw'], 0xfe363c80)
        self.assertIsNone(commands[0]['operands'][2]['annotation'])

    def test_rejects_truncated_word_or_operand_span(self):
        for data in (b'\0', struct.pack('>I', 0x0001005b)):
            with self.assertRaises(ValueError):
                parse_commands(data, 0, {})

    def test_rejects_count_opcode_outside_dispatch_domain(self):
        for data in (struct.pack('>I', 0x0100005b), struct.pack('>I', 119)):
            with self.assertRaises(ValueError):
                parse_commands(data, 0, {})
