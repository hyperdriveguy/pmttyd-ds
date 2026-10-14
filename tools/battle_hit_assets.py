#!/usr/bin/env python3
"""Import initial original battle HitObj hierarchy/triangles in engine units."""
import argparse
import hashlib
import json
import math
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO, extract

SOURCES = {
    'bti_01': '213c62e758be15bc0731b3fa58558d1b2c565ea1c96e91111ea07b52cf751ca8',
    'stg_00_1': '8d749bd8caad7fced1596231b755dc4e372e938816e10add8a8e0d7a69f5a2e1',
}


def f32(value):
    return struct.unpack('<f', struct.pack('<f', value))[0]


def decode(raw, expected_hash):
    if hashlib.sha256(raw).hexdigest() != expected_hash:
        raise ValueError('Original battle map revision changed')
    def u(offset): return struct.unpack_from('>I', raw, offset)[0]
    def string(offset): return raw[offset:raw.index(b'\0', offset)].decode('ascii')
    base = 32
    table = base + u(4) + u(8)*4
    names = table + u(12)*8
    chunks = {string(names+u(table+n*8+4)): base+u(table+n*8) for n in range(u(12))}
    collision = string(base+u(chunks['information']+12))
    def locate(node):
        if string(base+u(node)) == collision: return node
        for pointer in (u(node+12), u(node+16)):
            if pointer:
                match = locate(base+pointer)
                if match is not None: return match
        return None
    root = locate(base+u(chunks['information']+4))
    if root is None: raise ValueError('Missing original collision root')
    objects = []
    # Source selected collision root receives Scale10 directly from _mapLoad.
    def walk(node, parent_scale, parent_translation, siblings=True):
        name = string(base+u(node))
        scale, rotation, translation = (struct.unpack_from('>3f', raw, node+24+12*i) for i in range(3))
        if any(rotation): raise ValueError('Initial battle collision rotation needs a source matrix adapter')
        world_scale = [f32(parent_scale[i]*scale[i]) for i in range(3)]
        world_translation = [f32(f32(parent_scale[i]*translation[i])+parent_translation[i]) for i in range(3)]
        def transform(point): return [f32(f32(world_scale[i]*point[i])+world_translation[i]) for i in range(3)]
        parts = u(node+92)
        draw = u(node+88)
        cull = raw[base+draw+1] if draw else 0
        attributes = u(base+draw+8) if draw else 0
        bounds = struct.unpack_from('>6f', raw, node+60)
        center = transform([f32(f32(bounds[i]+bounds[i+3])*.5) for i in range(3)]) if parts else transform(translation)
        maximum = transform([f32(bounds[i+3]+f32(.1)) for i in range(3)])
        delta = [f32(maximum[i]-center[i]) for i in range(3)]
        # Declared offline F32/sqrt construction, not a PPC rsqrt bit-parity claim.
        squared = f32(f32(math.fma(delta[0],delta[0],f32(delta[1]*delta[1])))+f32(delta[2]*delta[2]))
        radius = f32(math.sqrt(squared)) if parts else 0.
        triangles = []
        for part in range(parts):
            mesh = base+u(node+100+8*part)
            if not raw[mesh+3]: raise ValueError('Unsupported original unpacked HitObj mesh')
            bits, vcd = u(mesh+8), base+u(mesh+12)
            if not bits&1 or bits&~0x77: raise ValueError('Unsupported original HitObj attributes')
            attributes_present = [bit for bit in range(7) if bits&(1 << bit)]
            positions, fraction = base+u(vcd)+4, u(vcd+68)
            if fraction > 31: raise ValueError('Unsupported position quantization')
            for run in range(u(mesh+4)):
                cursor, end = base+u(mesh+16+8*run), base+u(mesh+16+8*run)+u(mesh+20+8*run)
                while cursor < end:
                    command = raw[cursor]; cursor += 1
                    if not command: continue
                    if command&0xf8 not in (0x80,0x88,0x90,0x98,0xa0): raise ValueError('Unsupported source primitive')
                    count = struct.unpack_from('>H', raw, cursor)[0]; cursor += 2
                    vertices = []
                    for _ in range(count):
                        indices = struct.unpack_from('>'+'H'*len(attributes_present), raw, cursor)
                        cursor += 2*len(attributes_present)
                        position = struct.unpack_from('>3h', raw, positions+6*indices[0])
                        vertices.append(transform([f32(v/(1 << fraction)) for v in position]))
                    # hitCalcVtxPosition strip winding/order, distinct from GX.
                    for i in range(count-2):
                        triangles.append([vertices[i], vertices[i+2-i%2], vertices[i+1+i%2]])
                if cursor != end: raise ValueError('Original hit display list boundary changed')
        objects.append(dict(name=name, node=node-base, parts=parts, flags=0,
                            cull=cull, attributes=attributes, center=center,
                            radius=radius, triangles=triangles))
        child, sibling = u(node+12), u(node+16)
        if child: walk(base+child, world_scale, world_translation)
        if siblings and sibling: walk(base+sibling, parent_scale, parent_translation)
    walk(root, [10.,10.,10.], [0.,0.,0.], False)
    return collision, objects


def build(iso=DEFAULT_ISO, output='romfs'):
    out = Path(output); out.mkdir(parents=True, exist_ok=True)
    work = Path('local-assets/battle-hit'); work.mkdir(parents=True, exist_ok=True)
    fingerprint = hashlib.sha256(Path(__file__).read_bytes()+Path('tools/iso_assets.py').read_bytes()+str((Path(iso).stat().st_size,Path(iso).stat().st_mtime_ns)).encode()).hexdigest()
    metadata = out/'battle-hit.json'
    if metadata.exists() and json.loads(metadata.read_text()).get('fingerprint') == fingerprint and (out/'battle-hit.bin').is_file(): return
    data = bytearray(struct.pack('<8sI', b'PMBHIT01', len(SOURCES)))
    maps = []
    for name, expected in SOURCES.items():
        source = work/(name+'.d'); extract(iso, 'm/'+name+'/d', source)
        root, objects = decode(source.read_bytes(), expected)
        data += struct.pack('<64s64sI', name.encode(), root.encode(), len(objects))
        for obj in objects:
            data += struct.pack('<64sIIHHI4fI', obj['name'].encode(), obj['node'], obj['parts'], obj['flags'], obj['cull'], obj['attributes'], *obj['center'], obj['radius'], len(obj['triangles']))
            for triangle in obj['triangles']: data += struct.pack('<9f', *(v for point in triangle for v in point))
        maps.append(dict(name=name, root=root, provenance=json.loads(source.with_suffix('.d.provenance.json').read_text()), objects=[{**o, 'triangles':len(o['triangles'])} for o in objects]))
    (out/'battle-hit.bin').write_bytes(data)
    metadata.write_text(json.dumps(dict(fingerprint=fingerprint, maps=maps, sha256=hashlib.sha256(data).hexdigest(), conversion='Original initial collision hierarchy and opposite-winding strip order; source Scale10 engine units, single-precision axis-aligned transforms/bbox expansion; native sqrt radius adaptation, no geometry reduction. Declared initial flags0 snapshot; later runtime history remains its own producer.', license='Original Nintendo/Intelligent Systems assets for private local use; no redistribution rights'), indent=2)+'\n')


if __name__ == '__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO); parser.add_argument('--output',default='romfs')
    args=parser.parse_args(); build(args.iso,args.output)
