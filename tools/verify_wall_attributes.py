#!/usr/bin/env python3
"""Compare wall owner/attribute records with read-only original map node bytes."""
from pathlib import Path
import struct
ROOT=Path(__file__).resolve().parents[1]
def verify(room,prefix):
    source=(ROOT/'local-assets'/f'{room}.d').read_bytes()
    packed=(ROOT/'romfs'/f'{prefix}.walls').read_bytes()
    tagged,=struct.unpack_from('<I',packed)
    assert tagged&0xe0000000==0xe0000000
    count=tagged&0x1fffffff
    assert count<=100000 and len(packed)==4+count*68
    def word(offset):return struct.unpack_from('>I',source,offset)[0]
    owners=set();rejected=0
    for n in range(count):
        flags,node=struct.unpack_from('<II',packed,4+n*68+24)
        offset=32+node;assert 32<=offset<=len(source)-96
        draw=word(offset+88)
        assert flags==(word(32+draw+8) if draw else 0)
        # High-bit named special cases require a separate original filter path.
        assert not flags&0x80000000
        owners.add(node)
        rejected+=bool(flags&4 and not flags&0x00880080)
    print(f'{room}: {count} wall records match original attributes/owners; {len(owners)} owners; {rejected} non-solid records')
def main():
    for room,prefix in [('gor_01','map'),('gor_00','gor_00'),('gor_02','gor_02'),('gor_03','gor_03'),('gor_04','gor_04')]:verify(room,prefix)
if __name__=='__main__':main()
