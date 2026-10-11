import math
import struct
import sys
import unittest
from pathlib import Path
from unittest.mock import patch
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from room_convert import damage_returns

class DamageReturns(unittest.TestCase):
    def fixture(self,y=15):
        raw=bytearray(256);struct.pack_into('>I',raw,16,0x0002005b)
        struct.pack_into('>3f',raw,68,84,y,220);raw[128:136]=b'A_floor\0'
        return raw,{24:64,64:128},{20:0x8007458c}
    def parse(self,data):
        with patch('room_convert.relocations',return_value=data):return damage_returns('unused')
    def test_literal_and_automatic_positions_preserved(self):
        for y in (15,-1000):
            table=self.parse(self.fixture(y))[0]
            self.assertEqual((table['call_offset'],table['table_offset']),(16,64))
            self.assertEqual(table['entries'][0]['position'],[84,y,220])
            self.assertEqual(table['entries'][0]['automatic_position'],y==-1000)
    def test_bad_call_and_missing_pointer_rejected(self):
        raw,local,imports=self.fixture();struct.pack_into('>I',raw,16,0x0003005b)
        with self.assertRaises(ValueError):self.parse((raw,local,imports))
        raw,local,imports=self.fixture();del local[24]
        with self.assertRaises(ValueError):self.parse((raw,local,imports))
    def test_nonfinite_truncated_and_unterminated_tables_rejected(self):
        with self.assertRaises(ValueError):self.parse(self.fixture(math.nan))
        raw,local,imports=self.fixture();local[24]=252
        with self.assertRaises(ValueError):self.parse((raw,local,imports))
        raw,local,imports=self.fixture();struct.pack_into('>I',raw,80,1)
        with self.assertRaises(ValueError):self.parse((raw,local,imports))
    def test_other_imports_do_not_create_tables(self):
        raw,local,_=self.fixture();self.assertEqual(self.parse((raw,local,{20:0x80000000})),[])

if __name__=='__main__':unittest.main()
