#!/usr/bin/env python3
"""Verify shipped camera-zone triangle domains and original general query owner."""
import argparse
import hashlib
import json
import math
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO, inventory
from verify_overworld_roads import verify

MANIFEST=Path(__file__).with_name('overworld_camera_zone_triangle_evidence.json')
def sub(a,b):return tuple(x-y for x,y in zip(a,b))
def cross(a,b):return (a[1]*b[2]-a[2]*b[1],a[2]*b[0]-a[0]*b[2],a[0]*b[1]-a[1]*b[0])
def dot(a,b):return sum(x*y for x,y in zip(a,b))
def classify(raw):
    u=lambda a:struct.unpack_from('>I',raw,a)[0]
    zones,polys,verts,indices=[u(a) for a in [0xf0,0xf4,0xfc,0x100]];out=[]
    for z in range(u(0xcc)):
        zone=zones+104*z;base=verts+12*u(zone+0x58)
        for p in range(u(zone+0x64)):
            poly=polys+8*(u(zone+0x60)+p);ib=indices+4*u(poly);count=u(poly+4)
            points=[struct.unpack_from('>3f',raw,base+12*u(ib+4*k)) for k in range(count)]
            for t in range(1,count-1):
                A,B,C=points[0],points[t],points[t+1];N=cross(sub(A,B),sub(B,C))
                if not all(math.isfinite(x) for v in [A,B,C] for x in v):raise ValueError('Nonfinite authored triangle')
                out.append(dict(zone=z,polygon=p,fan_index=t,nonhorizontal=not A[1]==B[1]==C[1],zero_3d=all(x==0 for x in N),zero_xz=N[1]==0))
    return out

def finite_query(Q,A,B,C,best):
    e0,e1,e2=sub(A,B),sub(C,A),sub(B,C);N=cross(e0,e2);D=(0.,-1.,0.)
    if N==(0.,0.,0.):return None
    S,V=dot(N,sub(Q,A)),dot(N,D)
    if (S>0 and V>=0) or (S<=0 and V<=0):return None
    K=[dot(cross(e1,D),sub(Q,A)),dot(cross(e0,D),sub(Q,B)),dot(cross(e2,D),sub(Q,C))]
    if any(k>0 if S>0 else k<0 for k in K):return None
    height=Q[1]+S/V
    return height if height>=best else None

def verify_triangles(iso):
    result=verify(iso,MANIFEST);e=json.loads(MANIFEST.read_text());files={r['path']:r for r in inventory(iso)['files']};alltri=[]
    with Path(iso).open('rb') as disc:
        for row in e['camera_files']:
            source=files[row['path']];disc.seek(source['offset']);raw=disc.read(source['size'])
            if len(raw)!=row['size'] or hashlib.sha256(raw).hexdigest()!=row['sha256']:raise ValueError('Camera container mismatch')
            actual=classify(raw)
            if actual!=row['triangles']:raise ValueError('Zone geometry classification mismatch')
            alltri+=actual
    A,B,C=(0.,0.,0.),(0.,0.,2.),(2.,0.,0.)
    cases=[(((.5,1.,.5),A,B,C,-1.),0.),(((.5,1.,.5),A,C,B,-1.),0.),
           (((.5,0.,.5),A,B,C,-1.),None),(((.5,0.,.5),A,C,B,-1.),0.),
           (((.5,-1.,.5),A,B,C,-2.),None),(((.5,1.,.5),A,B,C,0.),0.),
           (((0.,1.,0.),A,B,C,-1.),0.),(((3.,1.,3.),A,B,C,-1.),None),
           (((.5,2.,.5),A,B,(2.,2.,0.),-1.),.5),
           (((0.,1.,0.),A,A,C,-1.),None),
           (((0.,1.,0.),A,(0.,1.,0.),B,-1.),None)]
    for args,want in cases:
        got=finite_query(*args)
        if got!=want:raise ValueError(f'Derived triangle case mismatch: {got} != {want}')
    result.update(triangles=len(alltri),nonhorizontal=sum(t['nonhorizontal'] for t in alltri),zero_3d=sum(t['zero_3d'] for t in alltri),zero_xz=sum(t['zero_xz'] for t in alltri),derived_cases=len(cases))
    if (result['triangles'],result['nonhorizontal'],result['zero_3d'])!=(494,234,8):raise ValueError('Triangle inventory changed')
    return result
if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--iso',default=DEFAULT_ISO)
    print(json.dumps(verify_triangles(parser.parse_args().iso),sort_keys=True))
