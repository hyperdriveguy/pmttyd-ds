"""Unchanged US DOL constructor constants in the original shared area pack."""
import hashlib,json,struct,unittest
from pathlib import Path
@unittest.skipUnless(Path('romfs/door-data.pmevt').is_file(),'original local area pack unavailable')
class OriginalNpcConstructor(unittest.TestCase):
 def test_source_constants_and_strings(self):
  data=Path('romfs/door-data.pmevt').read_bytes();meta=json.loads(Path('romfs/door-data.json').read_text())['field_npc_constructor']
  def span(a,n):
   for i in range(struct.unpack_from('>I',data,8)[0]):
    base,size,offset=struct.unpack_from('>III',data,12+12*i)
    if base<=a and a+n<=base+size:return data[offset+a-base:offset+a-base+n]
   self.fail('Missing source constructor bytes')
  self.assertEqual(sum(r['size'] for r in meta['dol_views']),96)
  for row in meta['dol_views']:self.assertEqual(hashlib.sha256(span(row['address'],row['size'])).hexdigest(),row['sha256'])
  self.assertEqual(span(0x802c11f8,24),struct.pack('>6f',0,0,0,1,1,1));self.assertEqual(span(0x8041fb64,4),struct.pack('>f',2))
  self.assertEqual(span(0x8041fc6c,4),b'\xff'*4);self.assertEqual(span(0x8041fc7c,4),bytes(4));self.assertEqual(span(0x8041fc94,4),struct.pack('>f',1))
  self.assertEqual(span(0x8041fd6c,6),b'hoshi\0');self.assertEqual(span(0x8041fd74,6),b'M_I_2\0');self.assertEqual(span(0x802c1468,8),b'c_luigi\0')
