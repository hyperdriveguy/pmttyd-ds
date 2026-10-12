"""Authored fixtures for sound-ID indirection; no original game bytes."""
import json
import tempfile
import struct
import sys
from pathlib import Path
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from audio_map import category, project_effects, macro_samples, switch_dependencies
from test_switch_audio import container

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

    def test_switch_wsd_indirection_and_sound_zero_stream(self):
        # FWSD wave table -> archive 7 -> file 91 -> member 9. The sound
        # uses note zero; it does not use sound ID or FWSD file ID as a wave ID.
        body=bytearray(92)
        struct.pack_into('<HHi',body,0,0x100,0,16)
        struct.pack_into('<HHi',body,8,0x100,0,28)
        struct.pack_into('<III',body,16,1,0x05000007,9)
        struct.pack_into('<IHHi',body,28,1,0x4900,0,12)
        struct.pack_into('<HHi',body,40+16,0x100,0,24)
        struct.pack_into('<IHHi',body,64,1,0x4902,0,12)
        struct.pack_into('<I',body,76,0)
        with tempfile.TemporaryDirectory() as directory:
            root=Path(directory)
            (root/'file.fwsd').write_bytes(container(b'FWSD',[(0x4800,b'INFO',body)]))
            inventory={'sounds':[{'sound_id':0,'file_id':11,'kind':0x2201,'name':'BGM_TEST'},
                {'sound_id':1,'file_id':33,'kind':0x2202,'name':'SE_SYS_MSG_KUR','wave_sound_index':0}],
                'wave_archives':[{'wave_archive_id':7,'file_id':91}], 'group_variants':[{'file_id':91,'variant_id':0}]}
            manifest={'archive_files':[{'file_id':33,'output':'file.fwsd'}], 'audio':[
                {'sound_ids':[0],'output':'stream.wav'}, {'file_id':91,'wave_id':9,'output':'wave.wav'}]}
            (root/'archive-inventory.json').write_text(json.dumps(inventory))
            (root/'manifest.json').write_text(json.dumps(manifest))
            rows=switch_dependencies(root)
            self.assertEqual(rows[0]['pcm'],'stream.wav')
            self.assertEqual(rows[1]['wave_dependencies'][0]['pcm'],'wave.wav')
            self.assertEqual(rows[1]['group_variants'],[{'file_id':91,'variant_id':0}])

    def test_categories_keep_dialogue_and_environment_separate(self):
        for name,expected in [('SE_SYS_MSG_KUR','dialogue'),('SFX_WINDOW_NORMAL_OPEN1','dialogue'),
                              ('SE_ENV_gor_Wave','ambience'),('BGM_O_stg_gor1','overworld'),
                              ('SE_BTL_SYS_Guard','battle'),('SFX_EVT_MAP','event'),
                              ('SFX_CURSOR_OK1','interface'),('unknown','unclassified')]:
            self.assertEqual(category(name),expected)

if __name__=='__main__':unittest.main()
