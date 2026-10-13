from pathlib import Path
import struct
import sys
import unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from character_animation_data import animation,evaluate,node_delta


def fixture():
    raw=bytearray(0x800)
    def word(offset,value):struct.pack_into('>I',raw,offset,value)
    def real(offset,value):struct.pack_into('>f',raw,offset,value)
    for offset,value in [(0xf0,1),(0x12c,1),(0x13c,1),(0x140,24),(0x148,1),
                         (0x154,0x600),(0x15c,0x610),(0x190,0x6c0),(0x1a0,0x6f0),(0x1a4,0x650),(0x1ac,0x200)]:word(offset,value)
    raw[0x200:0x208]=b'fixture\0';word(0x23c,0x300)
    for offset,value in [(4,1),(8,2),(12,1),(20,1),(28,1),(0x24,0x44),(0x28,0x50),(0x2c,0x100),(0x34,0x120),(0x3c,0x110)]:word(0x300+offset,value)
    struct.pack_into('>Iff',raw,0x344,0,0,4)
    key=0x350+44;real(key,4)
    for offset in [8,24,40]:word(key+offset,1)
    struct.pack_into('>Bbbb',raw,0x400,0,16,0,0)
    struct.pack_into('>Bbbb',raw,0x410,0,16,0,90)
    struct.pack_into('>BB2xff',raw,0x420,0,255,0,0)
    for index in [3,4,5]:real(0x650+index*4,1)
    raw[0x6c0]=5;struct.pack_into('>5f',raw,0x6c4,0,0,1,1,0);raw[0x6f0]=1
    return raw


class CharacterAnimationTests(unittest.TestCase):
    def test_sparse_position_texture_and_instant_node(self):
        parsed=animation(fixture(),'fixture');partial=evaluate(parsed,1)
        self.assertEqual(partial['positions'][0],[.25,0,0])
        self.assertEqual(partial['nodes'][0],0)
        self.assertEqual(partial['texture_indices'],[5])
        complete=evaluate(parsed,4)
        self.assertEqual(complete['positions'][0],[1,0,0])
        self.assertEqual(complete['nodes'][0],1)
        self.assertEqual(complete['texture_indices'],[4])
        self.assertEqual(evaluate(parsed,100)['nodes'][0],1)
        self.assertEqual(evaluate(parsed,.5)['positions'][0],[.125,0,0])

    def test_loop_boundary_and_negative_time(self):
        parsed=animation(fixture(),'fixture');parsed['loop']=True
        self.assertEqual(evaluate(parsed,4)['positions'][0],[0,0,0])
        self.assertEqual(evaluate(parsed,5)['positions'][0],[.25,0,0])
        self.assertEqual(evaluate(parsed,-1)['selected_time'],0)

    def test_visibility_is_byte_addition_at_passed_keys(self):
        raw=fixture();struct.pack_into('>I',raw,0x300+24,1)
        struct.pack_into('>I',raw,0x300+0x38,0x130)
        struct.pack_into('>I',raw,0x350+44+32,1)
        raw[0x430:0x432]=bytes([0,255]);raw[0x6f0]=1
        parsed=animation(raw,'fixture')
        self.assertEqual(evaluate(parsed,3.999)['visibility'],[1])
        self.assertEqual(evaluate(parsed,4)['visibility'],[0])
        self.assertEqual(evaluate(parsed,1)['visibility'],[1])

    def test_source_tangents_and_strict_out_range(self):
        self.assertEqual(node_delta(16,0,0,.25,4),.15625)
        self.assertAlmostEqual(node_delta(16,45,0,.25,4),.71875,places=5)
        for angle in [-90,-89,89,90]:self.assertEqual(node_delta(16,0,angle,.5,4),0)

    def test_rejects_invalid_sparse_targets_and_partial_in(self):
        raw=fixture();raw[0x400]=1
        with self.assertRaises(ValueError):animation(raw,'fixture')
        raw=fixture();raw[0x413]=0;raw[0x412]=89
        with self.assertRaises(ValueError):animation(raw,'fixture')
        raw[0x412]=167 # Signed−89 is table index0 and is valid.
        self.assertEqual(animation(raw,'fixture')['frames'][1]['node'][0][2],-89)
        with self.assertRaises(ValueError):animation(raw,'missing')
