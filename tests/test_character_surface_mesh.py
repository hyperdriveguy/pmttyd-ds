import sys
import unittest
from pathlib import Path
import numpy as np
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from character_surface_mesh import border_indices

class SurfaceAtlasBorders(unittest.TestCase):
    def test_clamp_duplicates_edge_texels(self):
        np.testing.assert_array_equal(border_indices(4,0),[0,0,0,1,2,3,3,3])

    def test_repeat_gutters_continue_opposite_edge(self):
        np.testing.assert_array_equal(border_indices(4,1),[2,3,0,1,2,3,0,1])

    def test_mirror_gutters_reverse_at_seam(self):
        np.testing.assert_array_equal(border_indices(4,2),[1,0,0,1,2,3,3,2])

    def test_unsupported_sampler_rejects(self):
        with self.assertRaises(ValueError):border_indices(4,3)
