"""Original used sky completion and sun local translation regression."""
import struct,tempfile,unittest,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from iso_assets import DEFAULT_ISO,inventory
from map_convert import convert

class OriginalVoyagePoseTests(unittest.TestCase):
 def test_original_completed_sky_rotates_sun_translation_upward(self):
  if not Path(DEFAULT_ISO).is_file():self.skipTest('Local original US ISO required')
  entry=next(f for f in inventory(DEFAULT_ISO)['files'] if f['path']=='m/muj_20/d')
  work=Path('artifacts');work.mkdir(exist_ok=True)
  with tempfile.TemporaryDirectory(dir=work) as directory:
   root=Path(directory);model=root/'muj_20.d'
   with Path(DEFAULT_ISO).open('rb') as source:source.seek(entry['offset']);model.write_bytes(source.read(entry['size']))
   convert(model,root/'base',initial_pose=('yoru_hiru',250),object_bindings=('S_hai03_sun',))
   convert(model,root/'translated',initial_pose=('yoru_hiru',250),local_offsets={'S_hai03_sun':(0,-26,0)},object_bindings=('S_hai03_sun',))
   def points(path):
    raw=path.read_bytes();magic,vertices,objects=struct.unpack_from('<8sII',raw);self.assertEqual(magic,b'PMHOBJ02');self.assertEqual(objects,1)
    tag,count=struct.unpack_from('<II',raw,16);self.assertEqual(tag,0)
    return {row[0]:row[4:] for row in struct.iter_unpack('<I6f',raw[72:72+count*28])}
   base=points(root/'base/map.houseobj');moved=points(root/'translated/map.houseobj');self.assertEqual(base.keys(),moved.keys());self.assertEqual(len(base),12)
   # The original finished night/day parent rotates180deg. Its local -260
   # engine-Y request therefore raises the original sun; a global offset or
   # the rejected initial sky pose would incorrectly lower it below the sea.
   for index,p in base.items():self.assertGreater(moved[index][1]-p[1],2.59)
