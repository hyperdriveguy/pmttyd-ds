#!/usr/bin/env python3
"""Verify head pack metadata against original read-only collision node bytes."""
from pathlib import Path
import struct
ROOT=Path(__file__).resolve().parents[1]
def main():
    for room,prefix in [('gor_01','map'),('gor_00','gor_00'),('gor_02','gor_02'),('gor_03','gor_03'),('gor_04','gor_04')]:
        source=(ROOT/'local-assets'/f'{room}.d').read_bytes()
        packed=(ROOT/'romfs'/f'{prefix}.head').read_bytes()
        magic,count=struct.unpack_from('<4sI',packed)
        assert magic==b'HEAD' and 0<count<=100000 and len(packed)==8+count*48
        owners=set();modes={}
        for n in range(count):
            flags,node,cull=struct.unpack_from('<III',packed,8+n*48+36)
            offset=32+node;assert 32<=offset<=len(source)-96
            draw=struct.unpack_from('>I',source,offset+88)[0]
            assert flags==(struct.unpack_from('>I',source,32+draw+8)[0] if draw else 0)
            assert cull==(source[32+draw+1] if draw else 3)
            owners.add(node);modes[cull]=modes.get(cull,0)+1
        print(f'{room}: {count} head triangles, {len(owners)} source owners; exact source attributes/cull bytes {modes}')
if __name__=='__main__':main()
