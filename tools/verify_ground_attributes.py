#!/usr/bin/env python3
"""Verify packed floor attributes against original read-only map node bytes."""
from pathlib import Path
import struct
ROOT=Path(__file__).resolve().parents[1]
def verify(room,prefix):
    source=(ROOT/'local-assets'/f'{room}.d').read_bytes()
    packed=(ROOT/'romfs'/f'{prefix}.ground').read_bytes()
    tagged,=struct.unpack_from('<I',packed)
    assert tagged&0x80000000
    count=tagged&0x7fffffff
    assert 0<count<=100000 and len(packed)==4+count*44
    def word(offset):return struct.unpack_from('>I',source,offset)[0]
    names={};attributes={}
    for n in range(count):
        flags,node=struct.unpack_from('<II',packed,4+n*44+36)
        offset=32+node;assert 32<=offset<=len(source)-96
        draw=word(offset+88)
        assert flags==(word(32+draw+8) if draw else 0)
        name_offset=32+word(offset)
        name=source[name_offset:source.index(b'\0',name_offset)].decode('ascii')
        names[name]=names.get(name,0)+1;attributes[flags]=attributes.get(flags,0)+1
    if room=='gor_00':assert names['A_mizu']==16 and 0x200 in attributes
    print(f'{room}: {count} floor attributes/source nodes match original bytes; {len(names)} owners')
def main():
    for room,prefix in [('gor_01','map'),('gor_00','gor_00'),('gor_02','gor_02'),('gor_03','gor_03'),('gor_04','gor_04')]:verify(room,prefix)
if __name__=='__main__':main()
