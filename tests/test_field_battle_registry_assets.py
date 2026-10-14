"""Original registry is unmodified source data, not preinitialized bindings."""
import hashlib,json,struct,unittest
from pathlib import Path

@unittest.skipUnless(Path('romfs/door-data.pmevt').is_file(),'original local area pack unavailable')
class OriginalFieldBattleRegistry(unittest.TestCase):
 def test_registry_source_hashes_zero_bindings_and_module_numbers(self):
  packed=Path('romfs/door-data.pmevt').read_bytes();meta=json.loads(Path('romfs/door-data.json').read_text())['field_battle_registry']
  def span(a,n):
   for i in range(struct.unpack_from('>I',packed,8)[0]):
    base,size,offset=struct.unpack_from('>III',packed,12+12*i)
    if base<=a and a+n<=base+size:return packed[offset+a-base:offset+a-base+n]
   self.fail('Missing registry view')
  for row in meta['dol_views']:self.assertEqual(hashlib.sha256(span(row['address'],row['size'])).hexdigest(),row['sha256'])
  self.assertEqual(sum(r['size'] for r in meta['dol_views']),2272);self.assertEqual(len(meta['records']),33)
  for i,row in enumerate(meta['records']):
   self.assertEqual(struct.unpack('>I',span(0x803057ec+4*i,4))[0],row['address']);self.assertEqual(span(row['address']+8,8),bytes(8));self.assertEqual(span(row['name_address'],4),row['name'].encode()+b'\0')
  self.assertEqual(span(0x80305870,4),bytes(4));self.assertEqual(next(r['address'] for r in meta['records'] if r['name']=='gor'),0x80305364)
  names=['BTLNO_GOR_TEST1','BTLNO_GOR_TEST2','BTLNO_GOR_00_01','BTLNO_GOR_02_ACLEC','BTLNO_GOR_02_SACLEC','BTLNO_GOR_02_01']
  for i,name in enumerate(names):
   address,value=struct.unpack('>2I',span(0x90540f84+8*i,8));self.assertEqual(value,i);self.assertEqual(span(address,len(name)+1),name.encode()+b'\0')
  self.assertEqual(span(0x90540f84+48,8),bytes(8));self.assertEqual(struct.unpack('<8sI6I',Path('romfs/field-area-battle.bin').read_bytes()),(b'PMFABT01',1,2,0x90400260,0x90540da8,0x90540f84,0,1))
