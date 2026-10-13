import collections
import json
import struct
import tempfile
import unittest
from pathlib import Path
from iso_assets import DEFAULT_ISO,extract
from map_convert import convert
from demo_map_visibility_assets import bindings,complete,PERMANENT_HIDDEN,WALL_ROOTS,EVENT_GROUPS


class DemoMapVisibilityTests(unittest.TestCase):
    def test_exact_source_subtrees_partition_without_unrelated_geometry_loss(self):
        if not Path(DEFAULT_ISO).is_file():self.skipTest('Local original US ISO required')
        def rows(path):
            data=path.read_bytes()
            return collections.Counter(data[i:i+36] for i in range(0,len(data),36))
        with tempfile.TemporaryDirectory() as directory:
            root=Path(directory);model=root/'gor_01.d';extract(DEFAULT_ISO,'m/gor_01/d',model)
            bound=bindings(model)
            self.assertEqual(bound['S']['offset'],0x158128)
            self.assertNotIn(bound['S_gai_kabe_k_01']['offset'],bound['S_gai_kabe_k']['descendants'])
            self.assertIn(bound['S_gai_kabe_k_01']['offset'],bound['S']['descendants'])
            self.assertTrue(all(bound[name]['mesh_nodes']>0 for name in (*PERMANENT_HIDDEN,*WALL_ROOTS)))
            tagged,before,after=[root/name for name in ['tagged','before','after']]
            convert(model,tagged,hidden=PERMANENT_HIDDEN,event_groups=EVENT_GROUPS)
            convert(model,before,hidden=(*PERMANENT_HIDDEN,*WALL_ROOTS))
            convert(model,after,hidden=PERMANENT_HIDDEN)
            vertices=(tagged/'map.vertices').read_bytes();kept=collections.Counter();seen=set()
            for start,count,texture,flags,wrap_s,wrap_t in struct.iter_unpack('<IIiIII',(tagged/'map.batches').read_bytes()[8:]):
                group=flags>>26;seen.add(group)
                if group==0:kept.update(vertices[i*36:(i+1)*36] for i in range(start,start+count))
            self.assertEqual(seen,{0,1})
            self.assertEqual(kept,rows(before/'map.vertices'))
            self.assertEqual(rows(tagged/'map.vertices'),rows(after/'map.vertices'))
            for suffix in ['ground','walls','head']:
                self.assertEqual((tagged/('map.'+suffix)).read_bytes(),(after/('map.'+suffix)).read_bytes())

    def test_cache_requires_all_map_companions_and_texture_slots(self):
        with tempfile.TemporaryDirectory() as directory:
            root=Path(directory);prefix='opening-demo-market'
            for suffix in ['vertices','uvanim','meshanim']:(root/(prefix+'.'+suffix)).write_bytes(b'fixture')
            (root/(prefix+'.batches')).write_bytes(struct.pack('<II',0,1))
            self.assertFalse(complete(root));(root/(prefix+'-0.t3x')).write_bytes(b'fixture');self.assertTrue(complete(root))
            (root/(prefix+'.uvanim')).unlink();self.assertFalse(complete(root))


if __name__=='__main__':unittest.main()
