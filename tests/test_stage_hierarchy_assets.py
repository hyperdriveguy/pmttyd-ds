import copy
from pathlib import Path
import json
import sys
import tempfile
import unittest
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
from stage_hierarchy_assets import decode,encode
from import_tutorial_stage_hierarchy import import_hierarchy
class HierarchyAssets(unittest.TestCase):
    def test_original_roundtrip_and_selection(self):
        for path,vertices,nodes,names in [('stg_00_0-hierarchy.json',7995,144,['A1','A2','B','C']),('base/bti_01-hierarchy.json',11829,108,['escape_wall','spotlight'])]:
            source=json.loads((ROOT/'assets/original/tutorial-stage'/path).read_text())
            self.assertEqual(source['vertex_count'],vertices);self.assertEqual(len(source['objects']),nodes)
            encoded=encode(source);self.assertEqual(encode(decode(encoded)),encoded)
            actual=[node['name'] for node in source['objects']]
            for name in names:self.assertEqual(actual.count(name),1)
            edited=copy.deepcopy(source);edited['objects'][0]['matrix'][3]+=1
            self.assertNotEqual(encode(edited),encoded) # current editable transform participates
    def test_rejections_preserve_unknowns(self):
        source=dict(schema=1,format='PMMAP001',vertex_count=1,objects=[dict(parent=-1,name='root',matrix=[1,0,0,0,0,1,0,0,0,0,1,0],bindings=[[0,1,2,3]])])
        raw=encode(source)
        for value in [raw[:-1],raw+b'\0',b'WRONG001'+raw[8:]]:
            with self.assertRaises(ValueError):decode(value)
        for field,value in [('parent',0),('name','x'*64),('matrix',[float('nan')]*12),('bindings',[]),('bindings',[[0,1,2,3],[0,4,5,6]])]:
            bad=copy.deepcopy(source);bad['objects'][0][field]=value
            with self.assertRaises(ValueError):encode(bad)
        with tempfile.TemporaryDirectory() as d:
            (Path(d)/'edits.json').write_text('preserve')
            with self.assertRaises(ValueError):import_hierarchy('/unopened.iso',d,'/unopened.template')
if __name__=='__main__':unittest.main()
