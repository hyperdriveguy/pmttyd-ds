import hashlib,json,struct,sys,unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from rel_event_inventory import read_bindings

@unittest.skipUnless(Path('romfs/door-data.pmevt').is_file(),'original local door pack unavailable')
class OriginalDoorData(unittest.TestCase):
 def test_source_record_and_both_original_lists(self):
  raw=Path('local-assets/door-data/gor.rel').read_bytes();meta=json.loads(Path('romfs/door-data.json').read_text())
  self.assertEqual(hashlib.sha256(raw).hexdigest(),'d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a')
  packed=Path('romfs/door-data.pmevt').read_bytes();count=struct.unpack_from('>I',packed,8)[0]
  def span(address,size):
   for n in range(count):
    base,length,offset=struct.unpack_from('>III',packed,12+12*n)
    if base<=address and address+size<=base+length:return packed[offset+address-base:offset+address-base+size]
   self.fail('Missing original door data')
  objects={o['name']:o for o in meta['objects']};source=objects['door_data_05'];self.assertEqual((source['file_offset'],source['size']),(0x46c9c,72))
  data=bytearray(raw[0x46c9c:0x46ce4]);local,imports,_=read_bindings(raw)
  for site,binding in local.items():
   if 0x46c9c<=site<0x46ce4:struct.pack_into('>I',data,site-0x46c9c,meta['sections'][binding['section']]['address']+binding['addend'])
  for site,value in imports.items():
   if 0x46c9c<=site<0x46ce4:struct.pack_into('>I',data,site-0x46c9c,value)
  self.assertEqual(span(source['address'],72),data)
  self.assertEqual(struct.unpack_from('>3I',data),(0,3,1))
  for key,expected in [('door_data_05_npc',['盗賊団親分','盗賊団１','盗賊団２']),('door_data_05_npc_01',['盗賊団親分','盗賊団１'])]:
   address=objects[key]['address'];actual=[]
   for n in range(4):
    pointer=struct.unpack('>I',span(address+4*n,4))[0]
    if not pointer:break
    name=bytearray()
    for k in range(32):
     byte=span(pointer+k,1)[0]
     if not byte:break
     name.append(byte)
    actual.append(name.decode('shift_jis'))
   self.assertEqual(actual,expected)
  header=struct.unpack('<8sI6I',Path('romfs/door-data.bin').read_bytes())
  self.assertEqual(header,(b'PMDRDT01',1,2,source['address'],11,objects['door_data_05_npc']['address'],objects['door_data_05_npc_01']['address'],1))
 def test_original_entry_children_and_generic_context(self):
  raw=Path('local-assets/door-data/gor.rel').read_bytes();meta=json.loads(Path('romfs/door-data.json').read_text());packed=Path('romfs/door-data.pmevt').read_bytes()
  objects={o['name']:o for o in meta['objects']};local,imports,_=read_bindings(raw)
  def span(address,size):
   for n in range(struct.unpack_from('>I',packed,8)[0]):
    base,length,offset=struct.unpack_from('>III',packed,12+12*n)
    if base<=address and address+size<=base+length:return packed[offset+address-base:offset+address-base+size]
   self.fail('Missing original entry closure')
  pairs=[('evt_door_02_entry','door_data_02',0x45d10,0x100),('evt_door_03_entry','door_data_03',0x462ac,0x100),('evt_door_03_roll_entry','door_data_03_roll',0x467b8,0x50)]
  expected=[]
  for script,record,start,size in pairs:
   obj=objects[script];self.assertEqual((obj['file_offset'],obj['size']),(start,size));data=bytearray(raw[start:start+size])
   for site,binding in local.items():
    if start<=site<start+size:struct.pack_into('>I',data,site-start,meta['sections'][binding['section']]['address']+binding['addend'])
   for site,value in imports.items():
    if start<=site<start+size:struct.pack_into('>I',data,site-start,value)
   self.assertEqual(span(obj['address'],size),data);expected.append((2,obj['address'],objects[record]['address']))
  expected.append((2,0x8033156c,objects['door_data_04']['address']))
  data=Path('romfs/field-area-events.bin').read_bytes();self.assertEqual(struct.unpack_from('<8sI',data),(b'PMFAEV02',4));self.assertEqual(list(struct.iter_unpack('<III',data[12:])),expected)
  self.assertEqual(hashlib.sha256(span(0x8033156c,0x134)).hexdigest(),'6269747e7ff14d9effbd84761663f25e880e3ed3c191c842e974db0559e78281')
  self.assertEqual(span(0x803316a0,4),struct.pack('>I',0x800e8288))
 def test_original_full_parent_and_linked_events(self):
  raw=Path('local-assets/door-data/gor.rel').read_bytes();meta=json.loads(Path('romfs/door-data.json').read_text());packed=Path('romfs/door-data.pmevt').read_bytes()
  def span(address,size):
   for n in range(struct.unpack_from('>I',packed,8)[0]):
    base,length,offset=struct.unpack_from('>III',packed,12+12*n)
    if base<=address and address+size<=base+length:return packed[offset+address-base:offset+address-base+size]
   self.fail('Missing original parent/script bytes')
  root=next(o for o in meta['objects'] if o['name']=='gor_02_init_evt');self.assertEqual((root['file_offset'],root['size']),(0x4d554,0x87c))
  self.assertEqual(hashlib.sha256(raw[0x4d554:0x4ddd0]).hexdigest(),'649d12889da97f2b7b29aa59f59f58671dfb916195160127f594df927e84109e')
  data=bytearray(raw[0x4d554:0x4ddd0]);local,imports,_=read_bindings(raw)
  for site,binding in local.items():
   if 0x4d554<=site<0x4ddd0:struct.pack_into('>I',data,site-0x4d554,meta['sections'][binding['section']]['address']+binding['addend'])
  for site,value in imports.items():
   if 0x4d554<=site<0x4ddd0:struct.pack_into('>I',data,site-0x4d554,value)
  self.assertEqual(span(root['address'],root['size']),data)
  self.assertEqual(struct.unpack('<8sIII',Path('romfs/field-area-init.bin').read_bytes()),(b'PMFAIN01',1,2,root['address']))
  linked={o['address']:o for o in meta['linked_dol_events']}
  for address,size,digest in [(0x8030ec7c,0x3e4,'bb92a499be0384713a44616f99f5e3f725f2cf507b3132126bb1e71b521f4a59'),(0x8030f060,0x278,'214cdff31534df948655149d8ba53441124be40ce6b980d353dee0b48952c519')]:
   self.assertIn(address,linked);self.assertEqual(linked[address]['size'],size);self.assertEqual(hashlib.sha256(span(address,size)).hexdigest(),digest)
  self.assertIn(0x8030f2d8,linked)
