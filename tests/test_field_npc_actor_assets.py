import hashlib,json,struct,sys,unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from field_npc_actor_assets import SPANS
class OriginalNpcActorData(unittest.TestCase):
 def test_original_views_and_unchanged_default_script(self):
  data=Path('romfs/door-data.pmevt').read_bytes();rows=[struct.unpack_from('>III',data,12+12*n) for n in range(struct.unpack_from('>I',data,8)[0])]
  def read(address,size):
   for base,length,offset in rows:
    if base<=address and address+size<=base+length:return data[offset+address-base:offset+address-base+size]
   self.fail('Missing original actor-event view '+hex(address))
  views=json.loads(Path('romfs/door-data.json').read_text())['field_npc_actor_events']['dol_views'];self.assertEqual(len(views),len(SPANS))
  for address,size,digest in SPANS:
   self.assertEqual(hashlib.sha256(read(address,size)).hexdigest(),digest)
   self.assertIn(dict(address=address,size=size,sha256=digest),views)
  self.assertEqual(read(0x80415e18,28),struct.pack('>7I',0x4005b,0x800500b0,1,0x8041fd80,0x600,2,1))
