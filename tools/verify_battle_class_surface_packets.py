#!/usr/bin/env python3
"""Read-only US source/class I4/object/FIFO/TEV pins; no GPU parity claim."""
import argparse
import hashlib
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_battle_class_raster import verify as verify_raster, BODIES
from verify_battle_submenu_packets import verify as verify_children
from verify_source_palette_texture_object import verify as verify_tex

def verify(iso=DEFAULT_ISO):
    verify_raster(iso)
    verify_children(iso)
    verify_tex(iso)
    with Path(iso).open('rb') as f:
        if f.read(8) != b'G8ME01\0\0':
            raise ValueError('US revision0 required')
        f.seek(0x420)
        dol, = struct.unpack('>I', f.read(4))
        f.seek(dol)
        hdr = f.read(256)
        def read(a, n):
            spans = []
            for i in range(18):
                off, base, size = [struct.unpack_from('>I', hdr, k+4*i)[0] for k in (0, 0x48, 0x90)]
                if base <= a and a+n <= base+size:
                    spans.append(dol+off+a-base)
            if len(spans) != 1:
                raise ValueError('Unknown source bytes')
            f.seek(spans[0])
            return f.read(n)
        if read(0x801038e8, 0x30).hex() != '3ce0cc01d0278000d0478000c002ae00d0078000986780009887800098a7800098c78000d0678000d08780004e800020':
            raise ValueError('Vertex X/Y -> fresh Z -> RGBA -> UV differs')
        tev = []
        for name, a, n, _ in BODIES[:2]:
            regs = {}
            for offset in range(0, n, 4):
                op, = struct.unpack('>I', read(a+offset, 4))
                if op >> 26 == 14 and (op >> 16)&31 == 0:
                    regs[(op >> 21)&31] = op&0xffff
                if op >> 26 == 18 and op&3 == 1:
                    delta = op&0x3fffffc
                    if delta&0x2000000:
                        delta -= 0x4000000
                    target = a+offset+delta
                    if target in (0x802b8960, 0x802b89a4):
                        args = tuple(regs.get(i) for i in range(3, 8))
                        expected = (0, 15, 10, 2, 15) if target == 0x802b8960 else (0, 7, 5, 4, 7)
                        if args != expected:
                            raise ValueError((name, hex(target), args))
                        tev.append((name, hex(target), args))
                    regs = {}
        if len(tev) != 4:
            raise ValueError('Missing actual class color/alpha source calls')
        f.seek(942647408)
        tpl = f.read(217920)
        if hashlib.sha256(tpl).hexdigest() != '8eb685317056f42b3ef73138eaad85d16fa4943d830b44a149f09971a5959bb1':
            raise ValueError('Original common TPL differs')
        word = lambda a: struct.unpack_from('>I', tpl, a)[0]
        headers = []
        for i, (height, width) in enumerate(((40, 40), (80, 128))):
            h = word(word(8)+8*i)
            fields = struct.unpack_from('>HHIIIIII', tpl, h)
            if fields != (height, width, 0, 4288 if i == 0 else 5088, 0, 0, 1, 1) or tpl[h+28:h+36] != bytes(8):
                raise ValueError('Actual I4 class dimensions/sampler/LOD differ')
            headers.append(dict(index=i, header=h, fields=fields))
    return dict(headers=headers, tev=tev, vertex_fifo='X,Y,freshZ,RGBA,UV', scope='Static original pins + finite native packet adapter; real source objects/camera/current owners remain required')

if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--iso', default=DEFAULT_ISO)
    print(verify(p.parse_args().iso))
