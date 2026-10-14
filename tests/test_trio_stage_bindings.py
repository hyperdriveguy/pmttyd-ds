from pathlib import Path
import struct
import unittest
import math
ROOT=Path(__file__).resolve().parents[1]
class TrioStageBindings(unittest.TestCase):
    def read(self,room):
        raw=(ROOT/'romfs'/f'{room}.houseobj').read_bytes()
        magic,vertices,count=struct.unpack_from('<8sII',raw)
        self.assertEqual(magic,b'PMHOBJ02')
        offset=16;objects=[]
        for tag in range(count):
            actual,size,*world=struct.unpack_from('<II12f',raw,offset);offset+=56
            self.assertEqual(actual,tag)
            bindings=[]
            for _ in range(size):
                row=struct.unpack_from('<I6f',raw,offset);offset+=28
                self.assertLess(row[0],vertices)
                self.assertTrue(all(math.isfinite(v) for v in row[1:]))
                bindings.append(row)
            objects.append((world,bindings))
        self.assertEqual(offset,len(raw))
        return vertices,objects
    @unittest.skipUnless((ROOT/'romfs/stg_00_1.houseobj').is_file(),'original private stage bindings unavailable')
    def test_original_selected_roots_cover_each_vertex_once(self):
        vertices,objects=self.read('stg_00_1');self.assertEqual(len(objects),5)
        raw=(ROOT/'romfs/stg_00_1.vertices').read_bytes();self.assertEqual(len(raw),vertices*36)
        seen=set()
        for matrix,bindings in objects:
            self.assertTrue(bindings)
            for index,x,y,z,ox,oy,oz in bindings:
                self.assertNotIn(index,seen);seen.add(index)
                original=struct.unpack_from('<3f',raw,index*36)
                for actual,want in zip(original,(ox,oy,oz)):self.assertEqual(actual,want)
                transformed=[sum(matrix[row*4+i]*v for i,v in enumerate((x,y,z)))+matrix[row*4+3] for row in range(3)]
                for actual,want in zip(transformed,original):self.assertAlmostEqual(actual,want,places=5)
        self.assertEqual(seen,set(range(vertices)))
        # Authored B/C translations stay inside their object matrices.
        self.assertAlmostEqual(objects[3][0][11],-.8,places=6)
        self.assertAlmostEqual(objects[4][0][11],-1,places=6)
    @unittest.skipUnless((ROOT/'romfs/bti_01.houseobj').is_file(),'original private background bindings unavailable')
    def test_hidden_builtin_owners_retained_without_draw_vertices(self):
        _,objects=self.read('bti_01');self.assertEqual(len(objects),2)
        self.assertTrue(all(not bindings for _,bindings in objects))
if __name__=='__main__':unittest.main()
