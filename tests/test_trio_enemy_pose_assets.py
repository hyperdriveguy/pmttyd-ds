"""Original enemy resource dependency evidence; no runtime/GPU admission claim."""
import hashlib
import json
import os
from pathlib import Path
import struct
import sys
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from battle_pose_directory import source_directory
from trio_enemy_pose_assets import source_roles, tpl_metadata, ASSET

PACK = Path(os.environ.get('TRIO_ENEMY_POSE_PACK', 'local-assets/trio-enemy-native/pack'))

class EnemyPoseValidation(unittest.TestCase):
    def test_wrong_or_truncated_rel_rejects(self):
        for raw in (b'', bytes(0x100), b'G8ME01'):
            with self.assertRaises(ValueError):
                source_roles(raw)

    def test_bad_tpl_directory_rejects(self):
        for raw in (b'', struct.pack('>III', 0x20af30, 1, 12),
                    struct.pack('>III', 0x20af30, 4097, 12),
                    struct.pack('>III', 0x20af30, 0, 12)):
            with self.assertRaises(ValueError):
                tpl_metadata(raw)

@unittest.skipUnless((PACK / (ASSET + '.json')).is_file(), 'private original enemy pack unavailable')
class OriginalEnemyPoseBank(unittest.TestCase):
    def test_complete_source_bank_and_unchanged_companions(self):
        m = json.loads((PACK / (ASSET + '.json')).read_text())
        model = (PACK / (ASSET + '.draw-model.bin')).read_bytes()
        texture = (PACK / (ASSET + '.draw-textures.bin')).read_bytes()
        original, bank, names = source_directory(model)
        self.assertEqual((original, bank, len(names), names[0]), ('c_kuribo', 'c_kuribo', 45, 'KUR_Z_2'))
        self.assertEqual(names, m['source_descriptor_order'])
        self.assertEqual(struct.unpack('<8sI', (PACK / (ASSET + '.pmanim')).read_bytes()[:12]), (b'PMANIM4\0', 45))
        directory = (PACK / (ASSET + '.pose-directory.bin')).read_bytes()
        self.assertEqual(directory[:8], b'PMPOSE02')
        self.assertEqual(struct.unpack_from('<I', directory, 136)[0], 45)
        self.assertEqual([directory[140+i*64:140+(i+1)*64].split(b'\0')[0].decode('ascii') for i in range(45)], names)
        for data, record in ((model, m['assets'][1]), (texture, m['assets'][2])):
            self.assertEqual(len(data), record['size'])
            self.assertEqual(hashlib.sha256(data).hexdigest(), record['sha256'])
        for product, info in m['products'].items():
            data = (PACK / product).read_bytes()
            self.assertEqual((len(data), hashlib.sha256(data).hexdigest()), (info['bytes'], info['sha256']))
        self.assertEqual(tpl_metadata(texture), m['source_tpl'])
        self.assertEqual(len(m['source_tpl']), 14)
        for image in m['source_tpl']:
            self.assertEqual((image['format'], image['palette'], image['wrap_s'], image['wrap_t'], image['min_filter'], image['mag_filter'], image['lod_bytes']), (14, 0, 0, 0, 1, 1, '0000000000000000'))
        self.assertEqual((m['atlas']['width'], m['atlas']['height'], m['atlas']['uv_scale']), (1024, 256, 4))
        self.assertEqual((PACK / (ASSET+'.bin')).read_bytes(), b'PMSUV1\0\0'+struct.pack('<f', 4))
        self.assertEqual(hashlib.sha256(Path(m['atlas']['path']).read_bytes()).hexdigest(), m['atlas']['sha256'])

    def test_original_roles_come_from_verified_rel(self):
        m = json.loads((PACK / (ASSET + '.json')).read_text())
        rel = Path('local-assets/trio-enemy-native/tik.rel').read_bytes()
        roles = source_roles(rel)
        self.assertEqual(roles, m['source_roles'])
        self.assertEqual([(r['unit_id'],r['model'],r['initial_pose']) for r in roles], [(3,'c_kuribo','KUR_S_1'),(4,'c_kuribo','TGK_S_1'),(5,'c_kuribo','PTK_S_1')])
        self.assertEqual([r['table_file_offset'] for r in roles], [0x2a89c, 0x30e28, 0x2f770])
