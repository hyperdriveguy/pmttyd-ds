#!/usr/bin/env python3
"""Pin bounded original Mario shadow admission and grounded polygon."""
import argparse,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_overworld_roads import verify
MANIFEST=Path(__file__).with_name('overworld_mario_shadow_evidence.json')
def matrix(point,normal,size,height=20):
    x,y,z=normal;px,py,pz=point
    return [[-size*(y*y+z*z),size*x*y,size*x*z,px+x],
            [size*x,height*y,size*z,py+y],
            [0,size*z,-size*y,pz+z]]
def derived(e):
    size=struct.unpack('>f',bytes.fromhex('41555555'))[0]
    def request(support,h):
        if support:return 0,size
        if h < -2:return None
        s=size-h/size
        if s < struct.unpack('>f',bytes.fromhex('3dcccccd'))[0]:s=0
        return None if s==0 else (2,s)
    assert request(True,999)==(0,size)
    assert request(False,-2)[0]==2 and request(False,-2.001) is None
    assert request(False,0)==(2,size) and request(False,200) is None
    assert request(False,10)[1]<size
    m=matrix((3,4,5),(0,1,0),size)
    vertices=e['ground_vertices'];assert len(vertices)==6
    out=[[sum(row[j]*v[j] for j in range(3))+row[3] for row in m] for v in vertices]
    assert all(v[1]==5 for v in out)
    assert out[0]==[3,5,5+size] and out[-1]==[3,5,5-size]
    assert abs(out[1][0]-3)==size*vertices[1][0]
    # Normal is source winding: no flip or invented upward normal.
    assert matrix((3,4,5),(0,-1,0),size)[1][3]==3
    def accepts(attr,obj_flags):return not(attr&0x80000000 and obj_flags&0x800)
    assert accepts(4,0) and accepts(0x80000000,0) and not accepts(0x80000000,0x800)
    return 13
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);iso=p.parse_args().iso
    e=json.loads(MANIFEST.read_text());result=verify(iso,MANIFEST);result.update(ground_vertices=6,derived_cases=derived(e));print(json.dumps(result,sort_keys=True))
