#!/usr/bin/env python3
"""Convert static TTYD map display lists to native 3DS triangle batches.
Format and transforms adapted from noclip.website/world.ts (MIT).
First-texture materials only; report multilayer materials for fidelity work.
"""
from pathlib import Path
import json
import math
import struct
from collections import defaultdict
import numpy as np
from character_bake import u,i,f,trans,rotate

def topology(command,verts):
    mode=command&0xf8
    if mode in (0x80,0x88):
        for n in range(0,len(verts)-3,4):
            yield [verts[n],verts[n+1],verts[n+2]];yield [verts[n],verts[n+2],verts[n+3]]
    elif mode==0x90:
        for n in range(0,len(verts)-2,3):yield verts[n:n+3]
    elif mode==0x98:
        for n in range(len(verts)-2):yield [verts[n+(n%2)],verts[n+1-(n%2)],verts[n+2]]
    elif mode==0xa0:
        for n in range(1,len(verts)-1):yield [verts[0],verts[n],verts[n+1]]
    else:raise ValueError(f'Unsupported GX primitive {command:#x}')

def convert(path,output,scale=.1,hidden=()):
    raw=Path(path).read_bytes();base=32
    table=32+u(raw,4)+u(raw,8)*4;names=table+u(raw,12)*8
    def string(o):return raw[o:raw.index(b'\0',o)].decode('ascii')
    chunks={string(names+u(raw,table+n*8+4)):base+u(raw,table+n*8) for n in range(u(raw,12))}
    tex=chunks['texture_table'];textures=[string(base+u(raw,tex+4+n*4)) for n in range(u(raw,tex))]
    mtable=chunks['material_name_table'];materials={};warnings=[]
    for n in range(u(raw,mtable)):
        o=base+u(raw,mtable+8+n*8);name=string(base+u(raw,o));color=tuple(raw[o+4:o+8]);samplers=raw[o+11]
        tid=-1;uvmatrix=np.eye(3);wrap=(0,0)
        if samplers:
            # GX stores sampler entries in reverse texture order.
            s=base+u(raw,o+12+(samplers-1)*4);entry=base+u(raw,s)
            tid=textures.index(string(base+u(raw,entry)));wrap=(raw[s+8],raw[s+9])
            v=struct.unpack_from('>7f',raw,o+44+(samplers-1)*28)
            ts,tt,ss,st,angle,cs,ct=v;angle=math.radians(-angle)
            a=np.array([[math.cos(angle),-math.sin(angle),0],[math.sin(angle),math.cos(angle),0],[0,0,1]])
            c=np.array([.5*cs,.5*ct-1])
            # Faithful texture transform order from the original map format.
            pre=np.eye(3);pre[:2,2]=np.array([ss,st])*c
            post=np.eye(3);post[:2,2]=-np.array([ss,st])*c
            uvmatrix=np.diag([ss,st,1])@post@a@pre
            uvmatrix[:2,2]+=np.array([ts,-tt])
        if samplers>1:warnings.append({'material':name,'samplers':samplers,'limitation':'Only texture zero currently rendered'})
        materials[o]={'name':name,'color':color,'color_source':raw[o+8],'layer':raw[o+10],'texture':tid,'uv':uvmatrix,'wrap':wrap}
    info=chunks['information'];scene_name=string(base+u(raw,info+8));collision_name=string(base+u(raw,info+12))
    vertices=defaultdict(list);nodes=[];collision=[];markers=[]
    def walk(o,parent,visible=False,collidable=False,depth=0):
        if depth>100:raise ValueError('Invalid map graph')
        name=string(base+u(raw,o));visible=visible or name==scene_name;collidable=collidable or name==collision_name
        values=struct.unpack_from('>9f',raw,o+24);m=parent@trans(values[6:9])@rotate(values[3:6])@np.diag([*values[:3],1])
        nodes.append(name)
        marker_points=[]
        for part in range(0 if name in hidden else u(raw,o+92)):
            mo=u(raw,o+96+part*8)
            if not mo:continue
            mesh=base+u(raw,o+100+part*8);packed=bool(raw[mesh+3])
            if not packed:raise ValueError('Unpacked map display list requires conversion support')
            bits=u(raw,mesh+8);vcd=base+u(raw,mesh+12)
            if bits&~0x77:raise ValueError(f'Unsupported vertex attributes {bits:#x}')
            arrays={0:base+u(raw,vcd)+4,1:base+u(raw,vcd+4)+4,2:base+u(raw,vcd+12)+4,4:base+u(raw,vcd+24)+4,5:base+u(raw,vcd+28)+4,6:base+u(raw,vcd+32)+4}
            attrs=[bit for bit in range(7) if bits&(1<<bit)]
            mat=materials[base+mo]
            for run in range(u(raw,mesh+4)):
                dl=base+u(raw,mesh+16+run*8);end=dl+u(raw,mesh+20+run*8)
                while dl<end:
                    cmd=raw[dl];dl+=1
                    if cmd==0:continue
                    count=struct.unpack_from('>H',raw,dl)[0];dl+=2
                    local=[]
                    for k in range(count):
                        indices={}
                        for attr in attrs:indices[attr]=struct.unpack_from('>H',raw,dl)[0];dl+=2
                        p=np.array([*struct.unpack_from('>hhh',raw,arrays[0]+indices[0]*6),1.],float)
                        p[:3]/=2**u(raw,vcd+68);p=m@p;p=p[:3]*scale
                        uv=np.array([0.,0.,1.])
                        if 4 in indices:uv[:2]=np.array(struct.unpack_from('>hh',raw,arrays[4]+indices[4]*4))/2**u(raw,vcd+72)
                        uv=mat['uv']@uv
                        color=np.array(mat['color'] if mat['color_source']==0 else [255]*4,float)/255
                        if 2 in indices:color*=np.array(list(raw[arrays[2]+indices[2]*4:arrays[2]+indices[2]*4+4]))/255
                        # Tex3DS uses bottom-origin texture coordinates.
                        local.append([*p,float(uv[0]),1-float(uv[1]),*color])
                    tris=list(topology(cmd,local))
                    if name.endswith("_bero"):marker_points.extend(v[:3] for v in local)
                    if visible:
                        for tri in tris:vertices[base+mo].extend(tri)
                    elif collidable:
                        for tri in tris:collision.append([v[:3] for v in tri])
        if name.endswith('_bero'):
            points=np.array(marker_points) if marker_points else np.array([(m@np.array([0,0,0,1]))[:3]*scale])
            markers.append({'name':name,'bounds':[points.min(0).tolist(),points.max(0).tolist()]})
        child=u(raw,o+12);sibling=u(raw,o+16)
        if child:walk(base+child,m,visible,collidable,depth+1)
        if sibling:walk(base+sibling,parent,visible if name!=scene_name else False,collidable if name!=collision_name else False,depth+1)
    walk(base+u(raw,chunks['information']+4),np.eye(4))
    output=Path(output);output.mkdir(parents=True,exist_ok=True)
    count=0;batches=[];bounds=[]
    with (output/'map.vertices').open('wb') as out:
        for mo in sorted(vertices,key=lambda x:materials[x]['layer']):
            v=vertices[mo];mat=materials[mo]
            for row in v:
                if mat['texture']<0:row[3]=496/512;row[4]=1-240/256
                out.write(struct.pack('<9f',*row));bounds.append(row[:3])
            batches.append((count,len(v),mat['texture'],mat['layer'],*mat['wrap']));count+=len(v)
    with (output/'map.batches').open('wb') as out:
        out.write(struct.pack('<II',len(batches),len(textures)))
        for b in batches:out.write(struct.pack('<IIiIII',*b))
    segments={}
    for tri in collision:
        a=np.array(tri);normal=np.cross(a[1]-a[0],a[2]-a[0]);length=np.linalg.norm(normal)
        if length<1e-8 or abs(normal[1])/length>.2:continue
        points=a[:,[0,2]]
        p,q=max([(points[x],points[y]) for x,y in [(0,1),(1,2),(0,2)]],key=lambda pair:np.linalg.norm(pair[0]-pair[1]))
        if np.linalg.norm(p-q)>.01:segments[tuple(sorted([tuple(np.round(p,4)),tuple(np.round(q,4))]))]=(float(a[:,1].min()),float(a[:,1].max()))
    with (output/'map.walls').open('wb') as out:
        out.write(struct.pack('<I',len(segments)|0x80000000))
        for (p,q),(lo,hi) in sorted(segments.items()):out.write(struct.pack('<6f',*p,*q,lo,hi))
    # Keep walkable collision surfaces, including ramps and elevated platforms.
    ground=[]
    for tri in collision:
        a=np.array(tri);normal=np.cross(a[1]-a[0],a[2]-a[0]);length=np.linalg.norm(normal)
        if length>1e-8 and abs(normal[1])/length>=.65:
            ground.append(tri)
    with (output/'map.ground').open('wb') as out:
        out.write(struct.pack('<I',len(ground)))
        for tri in ground:out.write(struct.pack('<9f',*(v for point in tri for v in point)))
    summary={'source':str(path),'scale':scale,'vertices':count,'triangles':count//3,'batches':len(batches),'textures':textures,
             'bounds':[np.min(bounds,axis=0).tolist(),np.max(bounds,axis=0).tolist()],
             'exit_markers':markers,'limitations':warnings,'hidden_nodes':list(hidden),'nodes':len(nodes),'collision_triangles':len(collision)}
    (output/'map-summary.json').write_text(json.dumps(summary,indent=2)+'\n')
    (output/'collision.json').write_text(json.dumps(collision))
    return summary
if __name__=='__main__':
    import argparse
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('input');p.add_argument('output');a=p.parse_args();print(convert(a.input,a.output))
