"""Synthetic relocation/event cases; no original event execution."""
import csv
from pathlib import Path
import struct
import sys
import tempfile
import unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from rel_event_inventory import read_bindings, inventory_events


def fixture():
    raw=bytearray(0x300)
    struct.pack_into('>I',raw,0,7)
    struct.pack_into('>II',raw,0x0c,3,0x80)
    struct.pack_into('>II',raw,0x28,0xa0,16)
    struct.pack_into('>II',raw,0x88,0x200,0x100)
    struct.pack_into('>II',raw,0x90,0,0x40)
    struct.pack_into('>II',raw,0xa0,0,0xc0)
    struct.pack_into('>II',raw,0xa8,7,0xe0)
    for site,values in [(0xc0,(0,202,1,0)),(0xc8,(4,1,0,0x80010000)),(0xd0,(0,203,0,0)),
                        (0xe0,(0,202,1,0)),(0xe8,(8,1,1,0x80)),(0xf0,(4,1,2,4)),(0xf8,(0,203,0,0))]:
        struct.pack_into('>HBBI',raw,site,*values)
    struct.pack_into('>5I',raw,0x200,0x3005b,0,0,0,2)
    raw[0x280:0x28c]=b'mac_fixture\0'
    return raw


class RelInventoryTests(unittest.TestCase):
    def test_distinguishes_import_local_and_bss_without_header_dereference(self):
        local,imports,foreign=read_bindings(fixture())
        self.assertEqual(imports,{0x204:0x80010000})
        self.assertEqual(local[0x208],dict(section=1,addend=0x80,file_offset=0x280))
        self.assertEqual(local[0x20c],dict(section=2,addend=4,file_offset=None))
        self.assertFalse(foreign)

    def test_one_past_target_keeps_identity_without_string_dereference(self):
        raw=fixture();struct.pack_into('>HBBI',raw,0xe8,8,1,1,0x100)
        local,_,_=read_bindings(raw)
        self.assertEqual(local[0x208],dict(section=1,addend=0x100,file_offset=None))

    def test_preserves_foreign_module_reference(self):
        raw=fixture();struct.pack_into('>I',raw,0xa8,9)
        local,_,foreign=read_bindings(raw)
        self.assertFalse(local)
        self.assertEqual(foreign[0x208],dict(module=9,section=1,addend=0x80))

    def test_rejects_bad_section_or_unterminated_stream(self):
        for site,value in [(0x10,0x2ff),(0xa4,0x2ff)]:
            raw=fixture();struct.pack_into('>I',raw,site,value)
            with self.assertRaises(ValueError):read_bindings(raw)
        with self.assertRaises(ValueError):read_bindings(b'\0')

    def test_rejects_duplicate_addr32_site(self):
        raw=fixture();struct.pack_into('>HBBI',raw,0xf0,0,1,2,4)
        with self.assertRaisesRegex(ValueError,'Duplicate'):read_bindings(raw)

    def test_inventory_retains_operands_identifiers_and_original_file(self):
        with tempfile.TemporaryDirectory() as tmp:
            root=Path(tmp);rel=root/'fixture.rel';rel.write_bytes(fixture());original=rel.read_bytes()
            symbols=root/'symbols.csv'
            with symbols.open('w') as f:
                fields=['area','name','namespace','type','file_addr','size','ram_addr']
                w=csv.DictWriter(f,fieldnames=fields);w.writeheader()
                w.writerow(dict(area='fixture',name='event',namespace='fixture.o',type='evt',file_addr='200',size='14'))
                w.writerow(dict(area='_main',name='callback',ram_addr='80010000'))
                w.writerow(dict(area='fixture',name='key',file_addr='280'))
            report=inventory_events(rel,symbols,'fixture',['event'])
            operands=report['events'][0]['commands'][0]['operands']
            self.assertEqual([a['raw'] for a in operands],[0,0,0])
            self.assertEqual(operands[0]['annotation'],'callback')
            self.assertEqual(operands[1]['identifier'],'mac_fixture')
            self.assertNotIn('rel_offset',operands[2])
            self.assertEqual(rel.read_bytes(),original)
