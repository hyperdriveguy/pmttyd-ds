"""Original used sky completion and sun local translation regression."""
import struct,tempfile,unittest,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from iso_assets import DEFAULT_ISO,inventory
from map_convert import convert
from map_animation import read_animations,sample_values

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

 def test_original_sky_uses_named_blue_point_light_and_s8_normals(self):
  if not Path(DEFAULT_ISO).is_file():self.skipTest('Local original US ISO required')
  entry=next(f for f in inventory(DEFAULT_ISO)['files'] if f['path']=='m/muj_20/d')
  with tempfile.TemporaryDirectory(dir='artifacts') as directory:
   root=Path(directory);model=root/'muj_20.d'
   with Path(DEFAULT_ISO).open('rb') as source:source.seek(entry['offset']);model.write_bytes(source.read(entry['size']))
   animation=next(a for a in read_animations(model,light_colors=True) if a['name']=='yoru_hiru')
   track=next(t for t in animation['light_color_tracks'] if t['light']=='pointLight9')
   values=sample_values(track,250)
   self.assertEqual([int(v*255) for v in values[:3]],[54,104,202])
   self.assertEqual(values[3],1.5)
   convert(model,root/'lit',initial_pose=('yoru_hiru',250),object_bindings=('S_hai01',),named_lighting=('S_hai01',))
   binding=(root/'lit/map.houseobj').read_bytes();count=struct.unpack_from('<I',binding,20)[0]
   indices=[row[0] for row in struct.iter_unpack('<I6f',binding[72:72+count*28])]
   self.assertEqual(count,126)
   vertices=(root/'lit/map.vertices').read_bytes()
   colors=[struct.unpack_from('<9f',vertices,index*36)[5:8] for index in indices]
   # Original completed yoru_hiru pointLight9 RGB54/104/202, coefficient1.5, black ambient.
   # Regardless of surface direction, white and arbitrary palette colors are impossible.
   for color in colors:
    for value,bound in zip(color,(81/255,156/255,1)):
     self.assertGreaterEqual(value,0);self.assertLessEqual(value,bound+1e-6)
    self.assertAlmostEqual(color[1]*54,color[0]*104,places=5)
    self.assertAlmostEqual(color[2],min(1,color[0]*202/54),places=6)
   self.assertGreater(max(c[2] for c in colors),.05)
   self.assertGreater(len({tuple(round(v,5) for v in c) for c in colors}),3)
