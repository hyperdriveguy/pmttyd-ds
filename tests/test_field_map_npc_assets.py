import hashlib,json,struct,sys,unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from field_map_npc_assets import parsed_records

@unittest.skipUnless(Path('romfs/field-npc-map-gor_02.bin').is_file(),'original parsed map inputs unavailable')
class FieldMapNpcAssets(unittest.TestCase):
 def test_exact_original_order_fields_and_words(self):
  source=Path('local-assets/field-map-npc/gor_02.s').read_bytes();records=parsed_records(source);packed=Path('romfs/field-npc-map-gor_02.bin').read_bytes();self.assertEqual(struct.unpack_from('<8sII',packed),(b'PMFMNPC1',15,392));self.assertEqual(len(packed),16+15*392)
  meta=json.loads(Path('romfs/field-npc-map-gor_02.json').read_text());self.assertEqual(meta['source']['sha256'],hashlib.sha256(source).hexdigest());self.assertEqual(meta['sha256'],hashlib.sha256(packed).hexdigest())
  expected=['パワーアップ屋','クリハカセ','情報屋完璧','盗賊団１','盗賊団２','盗賊団親分','サンダース','クリボー４','チューさん３','ロテン３','ボロドー１','ボロドー２','クラガリさん','ルイージ','盗賊団３']
  for n,(ordinal,strings,numbers) in enumerate(records):
   raw=packed[16+392*n:16+392*(n+1)];self.assertEqual(raw[:72],struct.pack('<I17f',ordinal,*numbers));self.assertEqual(strings[0].decode('shift_jis'),expected[n]);self.assertEqual(raw[72:],b''.join(s.ljust(64,b'\0') for s in strings));self.assertEqual(strings[2].decode('shift_jis'),'なし');self.assertEqual(strings[4],b'BTLNO_INVALID')
 def test_duplicate_tags_retain_input_order(self):
  source=Path('local-assets/field-map-npc/gor_02.s').read_bytes();first=source.split(b'<Npc>')[1];records=parsed_records(b'<Npc>'+first+b'<Npc>'+first)
  self.assertEqual(len(records),2);self.assertEqual([r[0] for r in records],[0,1]);self.assertEqual(records[0][1:],records[1][1:]);self.assertEqual(parsed_records(source+b'<Other><name>\"outside\"</></>'),parsed_records(source))
 def test_unchanged_original_registration_views(self):
  raw=Path('romfs/door-data.pmevt').read_bytes();meta=json.loads(Path('romfs/door-data.json').read_text())['field_map_npc']
  def span(a,n):
   for j in range(struct.unpack_from('>I',raw,8)[0]):
    base,size,offset=struct.unpack_from('>III',raw,12+12*j)
    if base<=a and a+n<=base+size:return raw[offset+a-base:offset+a-base+n]
   self.fail('Missing original data span')
  for j in range(struct.unpack_from('>I',raw,8)[0]):
   address,size,_=struct.unpack_from('>III',raw,12+12*j);self.assertEqual((address&3,size&3),(0,0))
  for view in meta['views']:self.assertEqual(hashlib.sha256(span(view['address'],view['size'])).hexdigest(),view['sha256'])
  self.assertEqual(span(0x802c3b10,len(b'BTLNO_INVALID\0')),b'BTLNO_INVALID\0');self.assertEqual(span(0x804207b4,4),bytes(4));self.assertEqual(span(0x80338274,32),bytes(32));self.assertEqual([t['address'] for t in meta['tables']],[0x803316d0,0x80338270,0x80416580])
