import hashlib,struct,sys,tempfile,unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
import source_window_startup_assets as tool
class WindowStartupAssets(unittest.TestCase):
 def test_original_cold_spans_and_preserve_edits(self):
  raw,m=tool.read_source(tool.DEFAULT_ISO)
  self.assertEqual(hashlib.sha256(raw).hexdigest(),'2afaa984dfd08c90bf7674e2df468bf9dff9b2b02873d01f601417714fba5b96')
  spans={a:raw[o:o+n]for a,n,o in(struct.unpack_from('>III',raw,12+12*i)for i in range(4))}
  self.assertEqual(spans[0x802c0140][:8],bytes.fromhex('4330000000000000'))
  self.assertEqual(spans[0x803dbff0],bytes(160));self.assertEqual(spans[0x8041ea18],bytes(16))
  self.assertEqual([x for x in spans[0x802c3080].split(b'\0')if x],[b'font/msgWindow.tpl',b'%s/f/msgWindow.tpl'])
  for name in ['archive-descriptors','window-source-globals']:
   self.assertTrue(all(x==['source-loader-BSS',None]for x in next(x['origins']for x in m['spans']if x['name']==name)))
  with tempfile.TemporaryDirectory()as d:
   out=Path(d)/'fresh';self.assertEqual(tool.build(tool.DEFAULT_ISO,out),m);stamp=(out/'memory.pmevt').stat().st_mtime_ns;self.assertEqual(tool.build(tool.DEFAULT_ISO,out),m);self.assertEqual((out/'memory.pmevt').stat().st_mtime_ns,stamp)
   bad=bytearray(raw);bad[-1]^=1;(out/'memory.pmevt').write_bytes(bad)
   with self.assertRaises(ValueError):tool.build(tool.DEFAULT_ISO,out)
   self.assertEqual((out/'memory.pmevt').read_bytes(),bad)
 def test_actual_read_async_whole_leaf(self):
  with Path(tool.DEFAULT_ISO).open('rb')as f:
   f.seek(0x420);dol=struct.unpack('>I',f.read(4))[0];f.seek(dol);header=f.read(256)
   for i in range(18):
    o,v,n=[struct.unpack_from('>I',header,k+4*i)[0]for k in (0,0x48,0x90)]
    if v<=0x80144de4 and 0x80144e18<=v+n:
     f.seek(dol+o+0x80144de4-v);raw=f.read(0x34);break
   else:self.fail('Missing original source ReadAsync leaf')
  self.assertEqual(raw.hex(),'9083007c3800000090a3008090c30084a083009060840001b0830090a0830090548407fab083009090e3008c900300884e800020')
 def test_actual_archive_source_pin(self):
  old=tool.ARC_SHA
  try:
   tool.ARC_SHA='0'*64
   with self.assertRaisesRegex(ValueError,'archive dispatcher'):tool.read_source(tool.DEFAULT_ISO)
  finally:tool.ARC_SHA=old

class ArchiveAddressGuard(unittest.TestCase):
 def test_wrong_zero_filled_bss_address_is_rejected_against_instructions(self):
  original=tool.ARCHIVE_DESCRIPTORS
  try:
   tool.ARCHIVE_DESCRIPTORS=0x803ebff0
   with self.assertRaisesRegex(ValueError,'signed archive descriptor address'):tool.read_source(tool.DEFAULT_ISO)
  finally:tool.ARCHIVE_DESCRIPTORS=original

 def test_exported_span_cannot_select_an_unrelated_zero_filled_range(self):
  original=tool.SPANS
  try:
   tool.SPANS=[(name,0x803ebff0 if name=='archive-descriptors'else address,size)for name,address,size in original]
   with self.assertRaisesRegex(ValueError,'signed archive descriptor address/layout'):tool.read_source(tool.DEFAULT_ISO)
  finally:tool.SPANS=original

 def test_only_known_generated_legacy_cache_migrates(self):
  import json
  raw,meta=tool.read_source(tool.DEFAULT_ISO);legacy=bytearray(raw)
  for index in range(struct.unpack_from('>I',legacy,8)[0]):
   offset=12+12*index
   if struct.unpack_from('>I',legacy,offset)[0]==0x803dbff0:struct.pack_into('>I',legacy,offset,0x803ebff0)
  legacy_sha='cbb8a9711aff88735eb8b2ee129f93eb8d35484a2f12ebfc1f8bca76677da8be'
  self.assertEqual(hashlib.sha256(legacy).hexdigest(),legacy_sha)
  previous=json.loads(json.dumps(meta));previous['pack_sha256']=legacy_sha
  for span in previous['spans']:
   if span['name']=='archive-descriptors':span['address']=0x803ebff0
  with tempfile.TemporaryDirectory()as temporary:
   out=Path(temporary)/'generated';out.mkdir();(out/'memory.pmevt').write_bytes(legacy);(out/'memory.json').write_text(json.dumps(previous))
   with self.assertRaises(ValueError):tool.build(tool.DEFAULT_ISO,out)
   self.assertEqual(tool.migrate_generated_cache(tool.DEFAULT_ISO,out),meta)
   self.assertEqual((out/'memory.pmevt').read_bytes(),raw)
   edited=bytearray(legacy);edited[-1]^=1;(out/'memory.pmevt').write_bytes(edited);(out/'memory.json').write_text(json.dumps(previous))
   with self.assertRaisesRegex(ValueError,'preserves edited'):tool.migrate_generated_cache(tool.DEFAULT_ISO,out)
   self.assertEqual((out/'memory.pmevt').read_bytes(),edited);self.assertEqual(json.loads((out/'memory.json').read_text()),previous)

if __name__=='__main__':unittest.main()
