import sys,unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from verify_native_battle_status import verify
class NativeStatusSource(unittest.TestCase):
 def test_original_children_and_tables(self):
  p=verify();self.assertEqual(p['signed_pouch_getters'],8);self.assertEqual(p['portraits'],[428,428,429,430,431,438,439,440]);self.assertEqual(p['gauge_partial'],list(range(456,472)))
 def test_source_magic_division_domain(self):
  # Independently evaluate original mulhw/add/srawi/srwi/add instruction
  # sequence for the admitted remainder range, rather than repeating C++.
  k=0xa57fb503-(1<<32)
  for r in range(100):
   x=r*15;q=(((k*x)>>32)+x)>>6;q+=(q&0xffffffff)>>31
   self.assertEqual(q,r*15//99)
if __name__=='__main__':unittest.main()
