import hashlib,json,struct,sys,tempfile,unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from field_party_definition_assets import build,RECORDS
from iso_assets import DEFAULT_ISO
class FieldPartyDefinitions(unittest.TestCase):
 @unittest.skipUnless(Path(DEFAULT_ISO).is_file(),'local original US ISO unavailable')
 def test_unchanged_original_records_and_pack(self):
  with tempfile.TemporaryDirectory() as tmp:
   meta=build(DEFAULT_ISO,tmp);data=(Path(tmp)/'field-party-definitions.pmevt').read_bytes()
   self.assertEqual(len(data),220);self.assertEqual(struct.unpack_from('>8sI',data),(b'PMEVT001',4));self.assertEqual(hashlib.sha256(data).hexdigest(),meta['pack_sha256'])
   self.assertEqual([r['kind'] for r in meta['records']],[1,8,11,13])
   for i,(kind,address,digest) in enumerate(RECORDS):
    a,n,off=struct.unpack_from('>III',data,12+12*i);self.assertEqual((a,n,off),(address,40,60+40*i));payload=data[off:off+n]
    self.assertEqual(hashlib.sha256(payload).hexdigest(),digest);words=struct.unpack('>10I',payload);self.assertEqual(words[0],0x802cb47c if kind==8 else 0x802cb4bc if kind==11 else 0x804213e0);self.assertEqual(words[1],0x800c4fa0);self.assertEqual(words[9],0)
    self.assertEqual(list(words),meta['records'][i]['words'])
   if Path('romfs/field-party-definitions.pmevt').exists():
    self.assertEqual(data,Path('romfs/field-party-definitions.pmevt').read_bytes());self.assertEqual(meta,json.loads(Path('romfs/field-party-definitions.json').read_text()))
if __name__=='__main__':unittest.main()
