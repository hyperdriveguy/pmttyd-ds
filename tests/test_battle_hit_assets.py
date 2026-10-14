import hashlib
import json
import struct
import sys
import unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
from battle_hit_assets import decode, SOURCES


class BattleHitAssets(unittest.TestCase):
    def test_source_hierarchy_engine_units_and_strip_order(self):
        manifest=json.loads((ROOT/'romfs/battle-hit.json').read_text())
        pack=(ROOT/'romfs/battle-hit.bin').read_bytes()
        self.assertEqual(struct.unpack_from('<8sI',pack),(b'PMBHIT01',2))
        self.assertEqual(hashlib.sha256(pack).hexdigest(),manifest['sha256'])
        data=(ROOT/'local-assets/battle-hit/bti_01.d').read_bytes()
        root,objects=decode(data,SOURCES['bti_01'])
        self.assertEqual(root,'bti_A')
        self.assertEqual([o['name'] for o in objects],['bti_A','A_bti_1_00','A_bti_1_01'])
        self.assertEqual([len(o['triangles']) for o in objects],[0,4,2])
        self.assertEqual(objects[2]['center'],[0.,-50.,350.])
        self.assertTrue(all(point[1]==0 for t in objects[1]['triangles'] for point in t))
        self.assertEqual([o['flags'] for o in objects],[0,0,0])
        second=(ROOT/'local-assets/battle-hit/stg_00_1.d').read_bytes()
        root,objects=decode(second,SOURCES['stg_00_1'])
        self.assertEqual(root,'A');self.assertEqual(len(objects[1]['triangles']),160)
        self.assertTrue(all(point[1]==-20 for t in objects[1]['triangles'] for point in t))
        damaged=bytearray(data);damaged[0]^=1
        with self.assertRaises(ValueError): decode(damaged,SOURCES['bti_01'])


if __name__=='__main__':unittest.main()
