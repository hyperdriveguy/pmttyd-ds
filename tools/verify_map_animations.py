#!/usr/bin/env python3
"""Verify locally extracted map tracks and a station door conversion sample."""
from pathlib import Path
import tempfile
import numpy as np
from map_animation import read_animations,sample,sample_material
from map_convert import convert
ROOT=Path(__file__).resolve().parents[1]
def main():
    count=0;material_count=0
    for name in ['gor_00','gor_01','gor_02','gor_03','gor_04','stage']:
        animations=read_animations(ROOT/'local-assets'/f'{name}.d')
        print(name,[(a['name'],len(a['tracks']),len(a['material_tracks'])) for a in animations])
        for animation in animations:
            for track in animation['tracks']:
                for frame in [track['frames'][0]['time'],animation['duration']/2,track['frames'][-1]['time']]:
                    assert np.isfinite(sample(track,frame)).all()
                    count+=1
            for track in animation['material_tracks']:
                for frame in [track['frames'][0]['time'],animation['duration']/2,track['frames'][-1]['time']]:
                    assert np.isfinite(sample_material(track,frame)).all();material_count+=1
    with tempfile.TemporaryDirectory(prefix='ttyd-door-') as work:
        folders=[Path(work)/str(frame) for frame in [0,23]]
        for frame,folder in zip([0,23],folders):
            convert(ROOT/'local-assets/gor_04.d',folder,animation='n_1_door_open',frame=frame)
        vertices=[np.fromfile(folder/'map.vertices',dtype='<f4').reshape(-1,9) for folder in folders]
        a,b=vertices;assert a.shape==b.shape;assert np.array_equal(a[:,3:],b[:,3:])
        changed=np.count_nonzero(np.linalg.norm(a[:,:3]-b[:,:3],axis=1)>1e-5)
        assert changed==702,f'Unexpected moved door geometry: {changed}'
        for ext in ['walls','ground','batches']:assert (folders[0]/('map.'+ext)).read_bytes()==(folders[1]/('map.'+ext)).read_bytes()
    with tempfile.TemporaryDirectory(prefix='ttyd-material-') as work:
        for room,clip,changed_expected in [('gor_02','mizu',135),('gor_03','neon_01',2010)]:
            animation=next(a for a in read_animations(ROOT/'local-assets'/f'{room}.d') if a['name']==clip)
            folders=[Path(work)/(room+str(frame)) for frame in [0,animation['duration']/2]]
            for folder,frame in zip(folders,[0,animation['duration']/2]):convert(ROOT/'local-assets'/f'{room}.d',folder,animation=clip,frame=frame)
            a,b=[np.fromfile(folder/'map.vertices',dtype='<f4').reshape(-1,9) for folder in folders]
            assert np.array_equal(a[:,:3],b[:,:3]);assert np.array_equal(a[:,5:],b[:,5:])
            changed=np.count_nonzero(np.linalg.norm(a[:,3:5]-b[:,3:5],axis=1)>1e-5)
            assert changed==changed_expected
            for ext in ['batches','ground','walls']:assert (folders[0]/('map.'+ext)).read_bytes()==(folders[1]/('map.'+ext)).read_bytes()
            print(f'{room}/{clip}: {changed} UV vertices changed; geometry/colors/collision/batches unchanged')
    print(f'{material_count} material samples and {count} original track samples finite; 702 station door vertices moved; UV/color/batches/collision unchanged')
if __name__=='__main__':main()
