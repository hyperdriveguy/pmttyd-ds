"""Native raster sharing keeps all logical source states within the64-cell ABI."""
import sys,tempfile,unittest,struct
from pathlib import Path
from unittest.mock import patch
from PIL import Image
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from character_mesh import compile_mesh
class AtlasSharing(unittest.TestCase):
    def build(self,unique):
        directory=tempfile.TemporaryDirectory();self.addCleanup(directory.cleanup);root=Path(directory.name)
        triangles=[(i,[((0,0,0),(0,0)),((1,0,0),(1,0)),((0,1,0),(0,1))]) for i in range(65)]
        def convert(source,path,index):Image.new('RGBA',(8,8),(index%unique,23,47,255)).save(path)
        with patch('character_mesh.geometry',return_value=triangles),patch('character_mesh.convert_tpl',side_effect=convert):
            result=compile_mesh([('model','source',['pose'])],root/'mesh',root/'atlas.png',deduplicate=True)
        return root,result
    def test_aliases_preserve_every_triangle_and_source_raster(self):
        root,result=self.build(57)
        self.assertEqual(result['textures'],57);self.assertEqual(len(result['texture_cells']),65)
        self.assertEqual(struct.unpack_from('<I',(root/'mesh').read_bytes(),44)[0],195)
        atlas=Image.open(root/'atlas.png')
        for row in result['texture_cells']:
            cell=row['cell'];self.assertEqual(atlas.getpixel((cell%8*128+64,cell//8*128+64)),(row['tpl_index']%57,23,47,255))
        self.assertEqual(result['texture_cells'][57]['cell'],result['texture_cells'][0]['cell'])
    def test_more_than64_distinct_native_rasters_remains_an_error(self):
        with self.assertRaisesRegex(ValueError,'capacity exceeded'):self.build(65)
if __name__=='__main__':unittest.main()
