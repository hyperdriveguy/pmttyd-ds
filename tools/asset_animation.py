"""Editable, lossless JSON representation of existing PMANIM3/4 native clips."""
import struct


class Reader:
    def __init__(self, raw):
        self.raw = raw; self.pos = 0

    def take(self, fmt):
        size = struct.calcsize('<' + fmt)
        row = struct.unpack_from('<' + fmt, self.raw, self.pos); self.pos += size
        return list(row)


def decode(raw):
    reader = Reader(raw)
    magic, count = reader.take('8sI')
    if magic not in (b'PMANIM3\0', b'PMANIM4\0') or not 0 < count <= 256:
        raise ValueError('Expected PMANIM3/4 animation pack')
    surface = magic == b'PMANIM4\0'; clips = []
    for _ in range(count):
        name, loop, start, end, np, nn, ng, nv, nt, nk, normal_count, visibility_count = reader.take('32sIff8I')
        # Bound list allocation by input length before consuming hostile counts.
        if max(np, nn, ng, nv, nt, nk, normal_count, visibility_count) > len(raw):
            raise ValueError('Animation count exceeds input extent')
        clip = {'name': name.split(b'\0')[0].decode('ascii'), 'loop': loop, 'start': start, 'end': end,
                'positions': [reader.take('3f') for _ in range(np)],
                'normals': [reader.take('3f') for _ in range(normal_count)],
                'nodes': reader.take('f' * nn), 'visibility': reader.take('B' * visibility_count),
                'groups': [reader.take('iiIII') for _ in range(ng)],
                'vertices': [reader.take('IIIff') for _ in range(nv)],
                'palettes': [reader.take('I256H') for _ in range(nt)]}
        if surface:
            clip['textures'] = [reader.take('5f2I') for _ in range(nt)]
            clip['colors'] = [reader.take('4B') for _ in range(nv)]
            clip['surfaces'] = [reader.take('2I') for _ in range(nv // 3)]
        clip['keys'] = []
        for _ in range(nk):
            time, p, n, t, normal, visibility = reader.take('f5I')
            if max(p, n, t, normal, visibility) > len(raw):
                raise ValueError('Animation key count exceeds input extent')
            key = {'time': time, 'position': [reader.take('I3bB') for _ in range(p)],
                   'normal': [reader.take('I3bB') for _ in range(normal)],
                   'node': [reader.take('I3bB') for _ in range(n)],
                   'texture': [reader.take('IB3x') for _ in range(t)],
                   'visibility': [reader.take('IB3x') for _ in range(visibility)]}
            if surface:
                key['uv'] = [reader.take('2f') for _ in range(t)]
            clip['keys'].append(key)
        clips.append(clip)
    if reader.pos != len(raw):
        raise ValueError('Animation pack has trailing bytes')
    return {'schema': 1, 'format': 'PMANIM4' if surface else 'PMANIM3', 'clips': clips}


def encode(doc):
    if doc.get('schema') != 1 or doc.get('format') not in ('PMANIM3', 'PMANIM4'):
        raise ValueError('Animation requires schema:1 and PMANIM3/4 format')
    surface = doc['format'] == 'PMANIM4'; clips = doc['clips']
    if not 0 < len(clips) <= 256:
        raise ValueError('Invalid animation clip count')
    raw = bytearray()
    def put(fmt, *row):
        raw.extend(struct.pack('<' + fmt, *row))
    put('8sI', (doc['format'] + '\0').encode('ascii'), len(clips))
    names = set()
    for clip in clips:
        name = clip['name']; encoded = name.encode('ascii')
        if not encoded or len(encoded) > 31 or '\0' in name or name in names:
            raise ValueError('Animation names must be unique ASCII, 1..31 bytes')
        names.add(name)
        p, n, g, v, t, k, normals, visible = [clip[key] for key in
            ('positions', 'nodes', 'groups', 'vertices', 'palettes', 'keys', 'normals', 'visibility')]
        if not v or len(v) % 3:
            raise ValueError('Animation vertices require triangles')
        put('32sIff8I', encoded, clip['loop'], clip['start'], clip['end'],
            len(p), len(n), len(g), len(v), len(t), len(k), len(normals), len(visible))
        for row in p: put('3f', *row)
        for row in normals: put('3f', *row)
        put('f' * len(n), *n); put('B' * len(visible), *visible)
        for row in g: put('iiIII', *row)
        for row in v:
            if row[0] >= len(g) or row[1] >= len(p) or row[2] >= len(t):
                raise ValueError('Animation vertex binding exceeds arrays')
            put('IIIff', *row)
        for row in t: put('I256H', *row)
        if surface:
            for key, length, fmt in [('textures', len(t), '5f2I'), ('colors', len(v), '4B'), ('surfaces', len(v)//3, '2I')]:
                if len(clip[key]) != length:
                    raise ValueError('Animation surface array length mismatch')
                for row in clip[key]: put(fmt, *row)
        for key in k:
            put('f5I', key['time'], *(len(key[field]) for field in ('position', 'node', 'texture', 'normal', 'visibility')))
            for field, fmt, limit in [('position', 'I3bB', len(p)), ('normal', 'I3bB', len(normals)),
                                      ('node', 'I3bB', len(n)), ('texture', 'IB3x', len(t)), ('visibility', 'IB3x', len(visible))]:
                for row in key[field]:
                    if row[0] >= limit:
                        raise ValueError('Animation delta binding exceeds arrays')
                    put(fmt, *row)
            if surface:
                if len(key['uv']) != len(key['texture']):
                    raise ValueError('Animation UV delta length mismatch')
                for row in key['uv']: put('2f', *row)
    # Reject NaN/Inf anywhere in the authored data via JSON serialization at caller.
    return bytes(raw)
