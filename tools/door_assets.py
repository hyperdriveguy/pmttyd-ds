#!/usr/bin/env python3
"""Build scoped original type3 inn/professor hit destinations and collision IDs.

Requires build_assets.py first. All source and generated geometry stays local.
Coordinates retain the EVT integer conversion after hit center+40*normal.
"""
import json
import math
from pathlib import Path
import struct
import numpy as np

ROOT = Path(__file__).resolve().parents[1]

def hit_point(nodes, triangles, name):
    matches = [n for n in nodes if n['name'] == name]
    if len(matches) != 1 or not matches[0]['parts']:
        raise ValueError('Missing/ambiguous source door hit: '+name)
    node = matches[0]
    matching = [t for t in triangles if t[10] == node['source_node']]
    if not matching:
        raise ValueError('Missing door triangle: '+name)
    a, b, c = np.array(matching[0][:9]).reshape(3, 3)
    normal = np.cross(c-a, a-b)
    normal /= np.linalg.norm(normal)
    point = list(node['center'])
    for axis in (0, 2):point[axis] += float(normal[axis])*.4
    return node['source_node'], [math.trunc(v*100)*.01 for v in point]

def subtree(nodes, name):
    roots = [n['source_node'] for n in nodes if n['name'] == name]
    if len(roots) != 1:raise ValueError('Missing collision subtree: '+name)
    included = set(roots)
    while True:
        new = {n['source_node'] for n in nodes if n['parent'] in included}
        if new <= included:break
        included |= new
    return sorted(included)

def build(root=ROOT):
    result = []
    for room, prefix, mask, stem, collision in (
            (0, 'map', 1, 'A_ie_03_f1_doa', None),
            (2, 'gor_02', 2, 'A_ie_02_doa', 'A_ie_02C')):
        directory = root/'local-assets'/prefix
        summary = json.loads((directory/'map-summary.json').read_text())
        raw = (directory/'map.head').read_bytes()
        magic, count = struct.unpack_from('<4sI', raw)
        if magic != b'HEAD' or len(raw) != 8+count*48:raise ValueError('Invalid head pack')
        triangles = list(struct.iter_unpack('<9fIII', raw[8:]))
        opened, outside = hit_point(summary['collision_nodes'], triangles, stem+'01')
        closed, inside = hit_point(summary['collision_nodes'], triangles, stem+'02')
        disabled = subtree(summary['collision_nodes'], collision) if collision else []
        result.append(dict(room=room, mask=mask, open_node=opened, close_node=closed,
                           outside=outside, inside=inside, interior_nodes=disabled))
    out = bytearray(struct.pack('<8sI', b'PMDOOR1', len(result)))
    for r in result:
        out += struct.pack('<4I6fI', r['room'], r['mask'], r['open_node'], r['close_node'],
                           *r['outside'], *r['inside'], len(r['interior_nodes']))
        out += struct.pack('<'+'I'*len(r['interior_nodes']), *r['interior_nodes'])
    (root/'romfs/doors.pmdoor').write_bytes(out)
    (root/'local-assets/doors.json').write_text(json.dumps(result, indent=2)+'\n')
    print('Original inn/professor door points and collision bindings:', len(result))
    return result

if __name__ == '__main__':build()
