"""Validate imported harbor resources against the actual read-only US source."""
import hashlib
import json
import struct
import sys
import unittest
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from battle_pose_directory import source_directory
from field_npc_resource_assets import source_tribes
from iso_assets import DEFAULT_ISO


class HarborNativeResources(unittest.TestCase):
    def test_original_banks_and_sailor_identities(self):
        catalog = json.loads(Path('romfs/harbor-native-npc-resources.json').read_text())
        source = source_tribes(DEFAULT_ISO)
        self.assertEqual({name: row['model'] for name, row in catalog['tribes'].items()}, {
            'クリスチーヌ': 'c_pkr', '第三勢力幹部': 'c_kanbu_ibt',
            '第三勢力部下': 'c_zako_n', 'ガラの悪い水夫': 'c_gara',
            '水夫ボム兵': 'c_suifu_b'})
        for name, row in catalog['tribes'].items():
            self.assertEqual(row, source[name])
            bank = catalog['resources'][row['model']]
            meta = json.loads(Path('romfs', bank['asset'] + '.json').read_text())
            raw = Path('romfs', bank['asset'] + '.draw-model.bin').read_bytes()
            model, texture, descriptors = source_directory(raw)
            self.assertEqual(model, row['model'])
            self.assertEqual(texture, bank['texture'])
            self.assertEqual(descriptors, bank['descriptors'])
            self.assertEqual(descriptors, meta['source_descriptor_order'])
            self.assertEqual(struct.unpack('<8sI', Path('romfs', bank['asset'] + '.pmanim').read_bytes()[:12]),
                             (b'PMANIM4\0', len(descriptors)))
            for suffix, original in [('draw-model.bin', 0), ('draw-textures.bin', 1)]:
                product = Path('romfs', bank['asset'] + '.' + suffix).read_bytes()
                self.assertEqual(hashlib.sha256(product).hexdigest(), meta['assets'][original]['sha256'])
            for product, info in meta['products'].items():
                raw = Path('romfs', product).read_bytes()
                self.assertEqual(len(raw), info['bytes'])
                self.assertEqual(hashlib.sha256(raw).hexdigest(), info['sha256'])
        self.assertEqual(catalog['resources']['c_kanbu_ibt']['texture'], 'c_kanbu')
        # Both entry paths must expose the same complete Goombella directory.
        party = json.loads(Path('romfs/field-player-c_pkr.json').read_text())
        self.assertEqual(party['converted_pose_names'], catalog['resources']['c_pkr']['descriptors'])
        self.assertEqual(len(party['converted_pose_names']), 32)
