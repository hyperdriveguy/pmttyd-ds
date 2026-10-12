"""Original AnimGroup node branches, evaluated with host NumPy arithmetic.

See docs/behavior/display/ANIMATION_{JOINT,TRANSFORM}_NODE_MATRICES.md.
Branch/order fidelity does not imply Gekko paired-single/libm bit equality.
"""
import math
import numpy as np


def translation(value):
    result = np.eye(4)
    result[:3, 3] = value
    return result


def rotations(primary, secondary=None):
    result = np.eye(4)
    triples = [secondary, primary] if secondary is not None else [primary]
    for triple in triples:
        for axis in (2, 1, 0):
            angle = triple[axis]
            if angle == 0:
                continue
            # Original node radians literal; host trig/concat remain adapters.
            radians = float(np.float32(np.float32(angle) * np.float32(0.01745329238474369)))
            sine, cosine = math.sin(radians), math.cos(radians)
            matrix = np.eye(4)
            a, b = ((1, 2), (2, 0), (0, 1))[axis]
            matrix[a, a] = matrix[b, b] = cosine
            matrix[a, b] = -sine
            matrix[b, a] = sine
            result = result @ matrix
    return result


def joint_gate(scale):
    # The recovered Z test is equality, including for the parent reciprocal.
    return scale[0] != 1 or scale[1] != 1 or scale[2] == 1


def node_matrix(value, joint=False, parent_scale=(1, 1, 1)):
    value = np.asarray(value)
    if len(value) != 24:
        raise ValueError('An original node requires24 floats')
    t, scale = value[:3], value[3:6]
    primary = np.asarray(value[6:9] * np.float32(2), dtype=np.float32)
    components = []
    if np.any(t != 0):
        components.append(translation(t))
    if joint:
        parent_scale = np.asarray(parent_scale)
        if joint_gate(parent_scale):
            reciprocal = np.ones(3) if np.any(parent_scale == 0) else 1 / parent_scale
            components.append(np.diag([*reciprocal, 1]))
        rotation = rotations(primary, value[9:12])
        if np.any(primary != 0) or np.any(value[9:12] != 0):
            components.append(rotation)
        if joint_gate(scale):
            components.append(np.diag([*scale, 1]))
    else:
        rotation = rotations(primary)
        rotation_present = np.any(primary != 0)
        scale_matrix = np.diag([*scale, 1])
        scale_present = np.any(scale != 1)
        # Zero pivots suppress their offsets. With identity components, only
        # the admitted offset survives; do not introduce a cancelling pivot.
        for pivot, offset, is_rotation in ((value[12:15], value[18:21], True),
                                           (value[15:18], value[21:24], False)):
            if not np.any(pivot != 0):
                continue
            present = rotation_present if is_rotation else scale_present
            matrix = rotation if is_rotation else scale_matrix
            if present:
                matrix = ((translation(offset) @ translation(pivot)) @ matrix) @ translation(-pivot)
            elif np.any(offset != 0):
                matrix = translation(offset)
                present = True
            if is_rotation:
                rotation, rotation_present = matrix, present
            else:
                scale_matrix, scale_present = matrix, present
        if rotation_present:
            components.append(rotation)
        if scale_present:
            components.append(scale_matrix)
    if not components:
        return np.eye(4)
    result = components[0]
    for component in components[1:]:
        result = result @ component
    return result
