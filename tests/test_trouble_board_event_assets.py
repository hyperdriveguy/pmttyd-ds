import hashlib,json,struct,sys,unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from rel_event_inventory import read_bindings

class OriginalTroubleBoardData(unittest.TestCase):
    @unittest.skipUnless(Path('romfs/trouble-board-events.json').is_file(),'original local board pack unavailable')
    def test_data_only_closure_and_original_relocations(self):
        meta=json.loads(Path('romfs/trouble-board-events.json').read_text());raw=Path('local-assets/trouble-board/gor.rel').read_bytes()
        self.assertEqual(hashlib.sha256(raw).hexdigest(),'d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a')
        packed=Path('romfs/trouble-board-events.pmevt').read_bytes();self.assertEqual(packed[:8],b'PMEVT001')
        count=struct.unpack_from('>I',packed,8)[0];local,imports,_=read_bindings(raw)
        for i,segment in enumerate(meta['segments']):
            address,size,offset=struct.unpack_from('>III',packed,12+i*12);self.assertEqual((address,size),(segment['address'],segment['size']))
            begin=segment['file_offset'];source=bytearray(raw[begin:begin+size])
            owner=[s for s in meta['sections'] if s['file_offset'] and s['file_offset']<=begin and begin+size<=s['file_offset']+s['size']]
            self.assertEqual(len(owner),1);self.assertFalse(owner[0]['executable'])
            for site,binding in local.items():
                if begin<=site<begin+size:struct.pack_into('>I',source,site-begin,meta['sections'][binding['section']]['address']+binding['addend'])
            for site,value in imports.items():
                if begin<=site<begin+size:struct.pack_into('>I',source,site-begin,value)
            self.assertEqual(packed[offset:offset+size],source)
        self.assertEqual(count,len(meta['segments']))
        objects={o['name']:o for o in meta['objects']}
        for name in meta['roots']:self.assertIn(name,objects)
        for name in ('irai_start_event01','irai_start_event04','irai_start_event_null','irai_msg_exposition','irai_msg_kakunin','irai_start_event'):self.assertIn(name,objects)
        def word(address):
            for i in range(count):
                base,size,offset=struct.unpack_from('>III',packed,12+i*12)
                if base<=address and address+4<=base+size:return struct.unpack_from('>I',packed,offset+address-base)[0]
            self.fail('Missing source data word')
        table=objects['irai_start_event']['address']
        for q in range(1,31):self.assertEqual(word(table+4*(q-1)),objects['irai_start_event01' if q==1 else 'irai_start_event04' if q==4 else 'irai_start_event_null']['address'])

    @unittest.skipUnless(Path('romfs/trouble-board-events.json').is_file(),'original local board pack unavailable')
    def test_all_original_cancellation_lists(self):
        meta=json.loads(Path('romfs/trouble-board-events.json').read_text())
        packed=Path('romfs/trouble-board-events.pmevt').read_bytes()
        count=struct.unpack_from('>I',packed,8)[0]
        def word(address):
            for n in range(count):
                base,size,offset=struct.unpack_from('>III',packed,12+12*n)
                if base<=address and address+4<=base+size:return struct.unpack_from('>I',packed,offset+address-base)[0]
            self.fail('Original cancellation data absent')
        objects={o['name']:o for o in meta['objects']};table=objects['irai_item_table']['address']
        expected={1:49,2:95,13:107,16:110,17:98,19:73,23:99,25:109,29:100}
        for quest in range(1,31):
            pointer=word(table+4*(quest-1));first=word(pointer)
            if quest in expected:
                self.assertEqual(first,expected[quest]);self.assertEqual(word(pointer+4),0xffffffff)
            else:self.assertEqual(first,0xffffffff)
