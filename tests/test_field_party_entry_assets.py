import hashlib,json,struct,sys,tempfile,unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from field_party_entry_assets import build,SPANS
from iso_assets import DEFAULT_ISO
class PartyEntryAssets(unittest.TestCase):
 @unittest.skipUnless(Path(DEFAULT_ISO).is_file(),'local original US ISO unavailable')
 def test_original_data_and_pack(self):
  with tempfile.TemporaryDirectory() as tmp:
   meta=build(DEFAULT_ISO,tmp);data=(Path(tmp)/'field-party-entry.pmevt').read_bytes()
   self.assertEqual(struct.unpack_from('>8sI',data),(b'PMEVT001',16));self.assertEqual(len(data),616)
   self.assertEqual(hashlib.sha256(data).hexdigest(),meta['pack_sha256'])
   for i,(address,size,digest) in enumerate(SPANS):
    a,n,offset=struct.unpack_from('>III',data,12+12*i);self.assertEqual((a,n),(address,size));self.assertEqual(hashlib.sha256(data[offset:offset+n]).hexdigest(),digest)
   if Path('romfs/field-party-entry.pmevt').exists():
    self.assertEqual(data,Path('romfs/field-party-entry.pmevt').read_bytes());self.assertEqual(meta,json.loads(Path('romfs/field-party-entry.json').read_text()))
if __name__=='__main__':unittest.main()
