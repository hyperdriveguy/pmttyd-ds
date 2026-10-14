"""Bounded field-side setup views and original source/relocation parity."""
import hashlib,json,struct,sys,unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from rel_data_pack import bounded_segments
from test_rel_event_inventory import fixture

class BoundedRelViews(unittest.TestCase):
 def test_explicit_view_retains_identity_without_dependency_closure(self):
  raw=fixture();segments,meta=bounded_segments(raw,[(0x204,12)])
  self.assertEqual(segments,[(0x90100004,struct.pack('>3I',0x80010000,0x90100080,0x90200004))])
  self.assertEqual(raw[0x204:0x210],bytes(12));self.assertEqual(meta[0]['size'],12)
  self.assertEqual(meta[0]['source_sha256'],hashlib.sha256(bytes(12)).hexdigest())
 def test_rejects_executable_or_cross_section_or_partial_relocations(self):
  raw=fixture();struct.pack_into('>I',raw,0x88,0x201)
  with self.assertRaisesRegex(ValueError,'nonexecuting'):bounded_segments(raw,[(0x204,12)])
  with self.assertRaisesRegex(ValueError,'nonexecuting'):bounded_segments(fixture(),[(0x2fc,8)])
  raw=fixture();struct.pack_into('>H',raw,0xe8,6)
  with self.assertRaisesRegex(ValueError,'Partial'):bounded_segments(raw,[(0x208,4)])
 def test_foreign_and_unaligned_views_are_not_silently_exported(self):
  raw=fixture();struct.pack_into('>I',raw,0xa8,9)
  with self.assertRaisesRegex(ValueError,'foreign'):bounded_segments(raw,[(0x204,12)])
  for view in [(0x205,4),(0x204,3),(0x204,0)]:
   with self.assertRaisesRegex(ValueError,'aligned'):bounded_segments(fixture(),[view])

@unittest.skipUnless(Path('romfs/door-data.pmevt').is_file(),'original local area pack unavailable')
class OriginalFieldSetup(unittest.TestCase):
 def test_original_gor_setup_zero_views_and_read_domain(self):
  meta=json.loads(Path('romfs/door-data.json').read_text())['field_battle_setup'];raw=Path('local-assets/door-data/gor.rel').read_bytes();packed=Path('romfs/door-data.pmevt').read_bytes()
  def span(address,size):
   for i in range(struct.unpack_from('>I',packed,8)[0]):
    base,length,offset=struct.unpack_from('>III',packed,12+12*i)
    if base<=address and address+size<=base+length:return packed[offset+address-base:offset+address-base+size]
   return None
  self.assertEqual(meta['numeric_indices'],[0]);self.assertEqual(meta['configuration'],0x90540da8)
  self.assertEqual([(r['file_offset'],r['size']) for r in meta['rel_views']],[(0x60e30,0x44),(0x60da0,0x18),(0x60d80,0x20),(0x60d50,0x30),(0x61c40,4),(0x1dbc8,0x10),(0x1dbd8,0x14)])
  for row in meta['rel_views']:
   data=span(row['address'],row['size']);self.assertIsNotNone(data);self.assertEqual(hashlib.sha256(data).hexdigest(),row['sha256']);self.assertEqual(hashlib.sha256(raw[row['file_offset']:row['file_offset']+row['size']]).hexdigest(),row['source_sha256'])
  config=span(0x90540da8,0x44);self.assertEqual(config[0x20:0x40],bytes([1,1])*16);self.assertEqual(struct.unpack_from('>I',config,0x18)[0],0)
  self.assertEqual(span(0x80417880,8),bytes(8));self.assertEqual(span(0x90541bba,1),b'\0')
  # This is a field-reader view, not an accidental full-table/stage import.
  self.assertIsNone(span(0x90540da8+0x44,4));self.assertIsNone(span(0x90540870,4))
