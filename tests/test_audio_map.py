"""Authored fixtures for sound-ID indirection; no original game bytes."""
import struct
import sys
from pathlib import Path
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from audio_map import category, project_effects, macro_samples

class AudioMapTests(unittest.TestCase):
    def test_fx_is_not_sample_id_and_groups_retain_variants(self):
        data=bytearray(112)
        for pos,group in [(0,3),(54,6)]:
            struct.pack_into('>IHH',data,pos,pos+54,group,1)
            struct.pack_into('>I',data,pos+28,pos+40)
            struct.pack_into('>HH',data,pos+40,1,0)
            struct.pack_into('>HH6B',data,pos+44,42,77,8,10,127,64,60,0)
        struct.pack_into('>I',data,108,0xffffffff)
        result=project_effects(data)
        self.assertEqual([r['group_id'] for r in result[42]],[3,6])
        self.assertEqual(result[42][0]['object_id'],77)
        data[0:4]=bytes(4)
        with self.assertRaises(ValueError):project_effects(data)

    def test_macro_start_sample_not_incidental_operand_byte(self):
        data=bytearray(44)
        struct.pack_into('>II',data,0,16,44)
        struct.pack_into('>IH',data,16,24,77)
        data[24:32]=bytes([0,0,99,0x10,0,0,0,0])
        data[32:40]=bytes([0x10,0,1,0,0,0,0,0])
        struct.pack_into('>I',data,40,0xffffffff)
        self.assertEqual(macro_samples(data)[77]['sample_ids'],[99])
        struct.pack_into('>I',data,16,100)
        with self.assertRaises(ValueError):macro_samples(data)

    def test_categories_keep_dialogue_and_environment_separate(self):
        for name,expected in [('SE_SYS_MSG_KUR','dialogue'),('SFX_WINDOW_NORMAL_OPEN1','dialogue'),
                              ('SE_ENV_gor_Wave','ambience'),('BGM_O_stg_gor1','overworld'),
                              ('SE_BTL_SYS_Guard','battle'),('SFX_EVT_MAP','event'),
                              ('SFX_CURSOR_OK1','interface'),('unknown','unclassified')]:
            self.assertEqual(category(name),expected)

if __name__=='__main__':unittest.main()
