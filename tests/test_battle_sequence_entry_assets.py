import hashlib,json,struct,sys,tempfile,unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from battle_sequence_entry_assets import build
from iso_assets import DEFAULT_ISO
class BattleSequenceAssets(unittest.TestCase):
 def test_original_data_only(self):
  with tempfile.TemporaryDirectory() as out:
   m=build(DEFAULT_ISO,out);raw=Path(out,'battle-sequence-entry.pmevt').read_bytes()
   self.assertEqual(m['sha256'],'7a5295a1050b3672569655c8f637aebd65aa69924acc700db5e18de8d5cf8349')
   self.assertEqual(len(raw),180);self.assertEqual(raw[:8],b'PMEVT001');self.assertEqual(struct.unpack_from('>I',raw,8)[0],2)
   self.assertEqual([x['size'] for x in m['spans']],[112,32]);self.assertIn(b'battle/audience/audience_normal.tpl\0',raw);self.assertIn(b'battle/audience/audience_luigi.tpl\0',raw)
   self.assertEqual(hashlib.sha256(raw).hexdigest(),m['sha256'])
   self.assertEqual(Path(out,'battle-sequence-entry.json').read_text(),json.dumps(m,indent=2)+'\n')
