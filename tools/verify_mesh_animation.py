#!/usr/bin/env python3
"""Compare runtime plaza/harbor transforms with original offline animation poses."""
from pathlib import Path
import struct,tempfile
import numpy as np
from map_convert import convert
ROOT=Path(__file__).resolve().parents[1]
def verify(room,clip,vertices,tracks_expected,frames,ambient=()):
    with tempfile.TemporaryDirectory(prefix='ttyd-mesh-') as directory:
        root=Path(directory);source=ROOT/'local-assets'/f'{room}.d'
        summary=convert(source,root/'runtime',mesh_ambient=(clip,),ambient=ambient);assert summary['animated_position_vertices']==vertices
        original=np.fromfile(root/'runtime/map.vertices',dtype='<f4').reshape(-1,9)
        raw=(root/'runtime/map.meshanim').read_bytes();magic,version,count=struct.unpack_from('<4sII',raw);assert (magic,version,count)==(b'MSAN',1,tracks_expected)
        tracks=[];offset=12
        for _ in range(count):
            duration,bindings=struct.unpack_from('<II',raw,offset);offset+=8
            matrices=np.frombuffer(raw,dtype='<f4',count=duration*12,offset=offset).reshape(-1,3,4);offset+=duration*48
            bind=[]
            for _ in range(bindings):bind.append(struct.unpack_from('<Ifff',raw,offset));offset+=16
            tracks.append((matrices,bind))
        assert offset==len(raw)
        for frame in frames:
            folder=root/str(frame);convert(source,folder,animation=clip,frame=frame)
            expected=np.fromfile(folder/'map.vertices',dtype='<f4').reshape(-1,9)
            actual=original.copy()
            for matrices,bindings in tracks:
                for vertex,x,y,z in bindings:actual[vertex,:3]=matrices[frame]@np.array([x,y,z,1])
            assert np.allclose(actual[:,:3],expected[:,:3],atol=1e-5),f'Runtime/offline pose differs at frame {frame}'
            for ext in ['walls','ground','batches']:assert (folder/('map.'+ext)).read_bytes()==(root/'runtime'/('map.'+ext)).read_bytes()
        print(f'{room}/{clip}: {len(frames)} runtime poses match original offline samples; {vertices} animated vertices; collision/batches unchanged')
def main():
    verify('gor_01','hae',12,2,[0,1,27,155,398])
    verify('gor_00','nami',4581,3,[0,1,155,400,978],ambient=('nami','nami_02','hiki_nami'))
if __name__=='__main__':main()
