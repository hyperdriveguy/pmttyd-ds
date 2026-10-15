import hashlib,importlib.util,json,struct,sys,tempfile,unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
import source_ui_startup_assets as tool
class StartupAssets(unittest.TestCase):
 def test_original_image_and_reuse(self):
  pack,meta=tool.read_source(tool.DEFAULT_ISO)
  self.assertEqual(hashlib.sha256(pack).hexdigest(),'7577bb75184fd6e6fe81d5e291fccd76919ef25b62a454b21523232a5053a9fe')
  self.assertEqual(meta['US_GP_instruction_hex'],'806d90b038800001380000009083016c')
  self.assertEqual(meta['original_font_directory_files'],[])
  spans={a:pack[o:o+n]for a,n,o in(struct.unpack_from('>III',pack,12+12*i)for i in range(10))}
  self.assertEqual(spans[0x8041e978],bytes(68))
  self.assertEqual(spans[0x8041f494],b'.\0\0\0')
  self.assertEqual(spans[0x80416010][:4],bytes.fromhex('1e1414ff'))
  self.assertEqual(spans[0x80429440][:4],bytes.fromhex('41800000'))
  for name in ['font-global-object-state','source-fog','source-selected-color']:
   origin=next(s['origins']for s in meta['spans']if s['name']==name)
   self.assertTrue(all(x==['source-loader-BSS',None]for x in origin))
  with tempfile.TemporaryDirectory()as d:
   dest=Path(d)/'new';self.assertEqual(tool.build(tool.DEFAULT_ISO,dest),meta)
   before=(dest/'memory.pmevt').stat().st_mtime_ns
   self.assertEqual(tool.build(tool.DEFAULT_ISO,dest),meta)
   self.assertEqual((dest/'memory.pmevt').stat().st_mtime_ns,before)
   bad=bytearray(pack);bad[-1]^=1;(dest/'memory.pmevt').write_bytes(bad)
   with self.assertRaises(ValueError):tool.build(tool.DEFAULT_ISO,dest)
   self.assertEqual((dest/'memory.pmevt').read_bytes(),bad)
 def test_pin_rejection(self):
  old=tool.GP_CODE
  try:
   tool.GP_CODE=bytes(16)
   with self.assertRaisesRegex(ValueError,'GP producer'):tool.read_source(tool.DEFAULT_ISO)
  finally:tool.GP_CODE=old
if __name__=='__main__':unittest.main()
