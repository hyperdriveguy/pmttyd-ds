import struct
import sys
import unittest
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from opening_heading_assets import stardust_data
from iso_assets import DEFAULT_ISO

@unittest.skipUnless(Path(DEFAULT_ISO).is_file(),'private original ISO unavailable')
class OriginalHeadingStardust(unittest.TestCase):
    def test_actual_source_bank(self):
        data,evidence=stardust_data(DEFAULT_ISO)
        self.assertEqual(len(data),847)
        self.assertEqual(data[:8],b'PMHSTAR1')
        self.assertEqual(len(evidence),7)
        # Frame0 is source array3; frame5 wraps to array0. Original RGBA/ST.
        self.assertEqual(struct.unpack_from('<5f4B',data,8),(-8.,-8.,0.,66.,0.,0,0,0,255))
        self.assertEqual(struct.unpack_from('<5f4B',data,8+5*96),(-8.,-8.,0.,0.,0.,0,0,0,255))
        self.assertEqual(list(data[8+768+36:8+768+43]),[60,90,100,104,106,108,109])
        constants=struct.unpack_from('<7f',data,819)
        for actual,expected in zip(constants,[-.05,.96,.01,.1,6.2831854820251465,8,1000]):self.assertAlmostEqual(actual,expected,places=6)
