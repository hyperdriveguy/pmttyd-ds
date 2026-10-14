import json,struct,sys,unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from rel_event_inventory import read_bindings

@unittest.skipUnless(Path('romfs/bero-info.pmevt').is_file(),'original local Bero pack unavailable')
class BeroInfoAssets(unittest.TestCase):
 def test_all_source_records_preserve_relocations_and_actual_table_order(self):
  meta=json.loads(Path('romfs/bero-info.json').read_text());raw=Path('local-assets/bero-info/gor.rel').read_bytes();pack=Path('romfs/bero-info.pmevt').read_bytes()
  count=struct.unpack_from('>I',pack,8)[0];segments=[struct.unpack_from('>III',pack,12+12*n) for n in range(count)]
  def span(address,size):
   for a,length,offset in segments:
    if a<=address and address+size<=a+length:return pack[offset+address-a:offset+address-a+size]
   self.fail('Missing original span')
  sections=[struct.unpack_from('>II',raw,struct.unpack_from('>I',raw,16)[0]+8*n) for n in range(struct.unpack_from('>I',raw,12)[0])]
  local,imports,_=read_bindings(raw)
  self.assertEqual([t['namespace'] for t in meta['tables']],['gor_01.o','gor_00.o','gor_02.o','gor_03.o','gor_04.o'])
  self.assertEqual([len(t['entries']) for t in meta['tables']],[3,1,2,5,4])
  for table in meta['tables']:
   off,size=table['file_offset'],table['size'];expected=bytearray(raw[off:off+size])
   for site,b in local.items():
    if off<=site<off+size:struct.pack_into('>I',expected,site-off,0x90000000+b['section']*0x100000+b['addend'])
   for site,address in imports.items():
    if off<=site<off+size:struct.pack_into('>I',expected,site-off,address)
   self.assertEqual(span(table['address'],size),expected)
   for n,entry in enumerate(table['entries']):
    self.assertEqual(entry['address'],table['address']+60*n)
    ptr=struct.unpack('>I',span(entry['address'],4))[0];name=bytearray()
    for k in range(64):
     byte=span(ptr+k,1)[0]
     if byte==0:break
     name.append(byte)
    self.assertEqual(name.decode(),entry['name'])
  east=meta['tables'][2];self.assertEqual([e['name'] for e in east['entries']],['w_bero','dokan'])
  self.assertEqual(east['entries'][0]['source_node'],1215808)
  self.assertEqual(struct.unpack('>15I',span(0x8030cd78,60)),(0,0xf000000,20000,100000,0,0,0xffffffff,0,0xffffffff,0,0,0,0x10001,0,0))
  self.assertEqual(span(0x80416060,20).hex(),'00020032fe363c80000000000000000200000001')
  for n,name in enumerate([b'n_',b'ne_',b'e_',b'se_',b's_',b'sw_',b'w_',b'nw_']):
   ptr=struct.unpack('>I',span(0x802c2c28+4*n,4))[0];self.assertEqual(span(ptr,len(name)+1),name+b'\0')
