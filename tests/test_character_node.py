import sys
import unittest
from pathlib import Path
import numpy as np

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from character_node import node_matrix, rotations, translation


def node():
    value = np.zeros(24)
    value[3:6] = 1
    return value


class CharacterNodeTests(unittest.TestCase):
    def test_joint_z_only_scale_is_skipped(self):
        value = node()
        value[5] = 2
        np.testing.assert_array_equal(node_matrix(value, True), np.eye(4))
        np.testing.assert_array_equal(node_matrix(value), np.diag([1, 1, 2, 1]))

    def test_joint_parent_gate_and_all_axis_zero_fallback(self):
        value = node()
        np.testing.assert_array_equal(node_matrix(value, True, (1, 1, 2)), np.eye(4))
        np.testing.assert_array_equal(node_matrix(value, True, (2, 1, 1)), np.diag([.5, 1, 1, 1]))
        np.testing.assert_array_equal(node_matrix(value, True, (2, 0, 4)), np.eye(4))

    def test_transform_ignores_secondary_rotation(self):
        value = node()
        value[9:12] = (20, 30, 40)
        np.testing.assert_array_equal(node_matrix(value), np.eye(4))
        np.testing.assert_allclose(node_matrix(value, True), rotations((0, 0, 0), (20, 30, 40)))

    def test_joint_ignores_both_pivots_and_offsets(self):
        value = node()
        value[12:24] = np.arange(12) + 1
        np.testing.assert_array_equal(node_matrix(value, True), np.eye(4))

    def test_zero_transform_pivots_suppress_offsets(self):
        value = node()
        value[18:24] = (5, 6, 7, 8, 9, 10)
        np.testing.assert_array_equal(node_matrix(value), np.eye(4))
        value[3:6] = (2, 3, 4)
        np.testing.assert_array_equal(node_matrix(value), np.diag([2, 3, 4, 1]))

    def test_identity_component_admits_offset_only_with_pivot(self):
        value = node()
        value[12:15] = (10, 20, 30)
        value[18:21] = (1, 2, 3)
        value[15:18] = (40, 50, 60)
        value[21:24] = (4, 5, 6)
        np.testing.assert_array_equal(node_matrix(value), translation((5, 7, 9)))

    def test_transform_scale_pivot_after_rotation(self):
        value = node()
        value[:3] = (1, 2, 3)
        value[3:6] = (2, 3, 4)
        value[6:9] = (0, 0, 45)
        value[15:18] = (5, 6, 7)
        value[21:24] = (2, 0, 0)
        expected = translation((1, 2, 3)) @ rotations((0, 0, 90))
        expected = expected @ ((translation((2, 0, 0)) @ translation((5, 6, 7))) @ np.diag([2, 3, 4, 1]) @ translation((-5, -6, -7)))
        np.testing.assert_allclose(node_matrix(value), expected, atol=1e-12)

    def test_joint_secondary_then_primary_axis_order(self):
        value = node()
        value[6:12] = (10, 20, 30, 40, 50, 60)
        expected = rotations((0, 0, 0), (40, 50, 60)) @ rotations((20, 40, 60))
        np.testing.assert_allclose(node_matrix(value, True), expected, atol=1e-12)


if __name__ == '__main__':
    unittest.main()
