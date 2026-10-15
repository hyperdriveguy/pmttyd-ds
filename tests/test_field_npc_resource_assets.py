"""Real original map dependencies, full CPU directories and immutable companions."""
import hashlib,json,struct,sys,unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from battle_pose_directory import source_directory
from field_npc_resource_assets import source_tribes
from iso_assets import DEFAULT_ISO

@unittest.skipUnless(Path('romfs/field-npc-resources.json').is_file(),'original local field resources unavailable')
class FieldNpcResources(unittest.TestCase):
 def test_full_original_banks_and_companions(self):
  catalog=json.loads(Path('romfs/field-npc-resources.json').read_text())
  expected={'c_deale':13,'c_kuri_h':17,'c_joho_b':5,'c_monban':16,'c_binbo':5,'c_bomt_n':41,'c_kuribo_n':5,'c_tyusan':6,'c_roten_g':5,'c_borodo_n':5,'c_f_mura':13,'c_luigi':36,'hoshi':6}
  self.assertEqual(set(catalog['resources']),set(expected));self.assertEqual(sum(expected.values()),173)
  for name,count in expected.items():
   asset=catalog['resources'][name]['asset'];meta=json.loads(Path('romfs',asset+'.json').read_text());model=Path('romfs',asset+'.draw-model.bin').read_bytes();texture=Path('romfs',asset+'.draw-textures.bin').read_bytes()
   original,bank,names=source_directory(model);self.assertEqual(original,name);self.assertEqual(names,meta['source_descriptor_order']);self.assertEqual(len(names),count);self.assertEqual(names,catalog['resources'][name]['descriptors'])
   self.assertEqual(struct.unpack('<8sI',Path('romfs',asset+'.pmanim').read_bytes()[:12]),(b'PMANIM4\0',count))
   self.assertEqual(hashlib.sha256(model).hexdigest(),meta['assets'][0]['sha256']);self.assertEqual(hashlib.sha256(texture).hexdigest(),meta['assets'][1]['sha256'])
   for product,info in meta['products'].items():
    raw=Path('romfs',product).read_bytes();self.assertEqual(len(raw),info['bytes']);self.assertEqual(hashlib.sha256(raw).hexdigest(),info['sha256'])
   atlas=Path(meta['atlas_evidence']['path']);self.assertEqual(atlas.stat().st_size,meta['atlas_evidence']['bytes']);self.assertEqual(hashlib.sha256(atlas.read_bytes()).hexdigest(),meta['atlas_evidence']['sha256'])
   self.assertEqual(meta['original_index0'],'HOS_Z_1' if name=='hoshi' else 'CHU_Z_1' if name=='c_tyusan' else 'Z_1')

 @unittest.skipUnless(Path(DEFAULT_ISO).is_file(),'read-only source ISO unavailable')
 def test_source_tribes_and_map_role_requests(self):
  catalog=json.loads(Path('romfs/field-npc-resources.json').read_text());tribes=source_tribes(DEFAULT_ISO);area=catalog['areas'][0];self.assertEqual(area['area'],'gor_02');self.assertEqual(len(area['actors']),15)
  misses=[]
  for actor in area['actors']:
   source=tribes[actor['tribe']]
   for field in ('address','model','poses','sha256'):self.assertEqual(actor[field],source[field])
   names=catalog['resources'][actor['model']]['descriptors']
   for role,request in actor['poses'].items():
    if request and request not in names:misses.append((actor['model'],role,request))
  self.assertEqual(misses,[('c_kuri_h','pose_20','d_1'),('c_kuri_h','pose_24','d_1')])
