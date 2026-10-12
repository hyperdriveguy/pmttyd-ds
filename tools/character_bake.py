#!/usr/bin/env python3
"""Bake original TTYD AnimGroup poses to transparent paper billboards.
Format reference: noclip.website AnimGroup.ts (MIT; see third_party).
Node branches/traversal: recovered US contracts in docs/behavior/display.
This preserves original layered geometry/textures, rather than redrawing art.
"""
from pathlib import Path
import math
import struct
import numpy as np
from PIL import Image
from iso_assets import convert_tpl
from character_node import node_matrix

def u(raw,o):return struct.unpack_from('>I',raw,o)[0]
def i(raw,o):return struct.unpack_from('>i',raw,o)[0]
def f(raw,o):return struct.unpack_from('>f',raw,o)[0]
# Map conversion imports these generic helpers. Character nodes use the
# separate recovered joint/transform branches below.
def trans(v):
    m=np.eye(4);m[:3,3]=v;return m

def rotate(values):
    x,y,z=np.deg2rad(values)
    sx,cx=math.sin(x),math.cos(x);sy,cy=math.sin(y),math.cos(y);sz,cz=math.sin(z),math.cos(z)
    rx=np.array([[1,0,0,0],[0,cx,-sx,0],[0,sx,cx,0],[0,0,0,1]])
    ry=np.array([[cy,0,sy,0],[0,1,0,0],[-sy,0,cy,0],[0,0,0,1]])
    rz=np.array([[cz,-sz,0,0],[sz,cz,0,0],[0,0,1,0],[0,0,0,1]])
    return rz@ry@rx

def matrix(node,index,parent=-1,joint=False):
    return node_matrix(node[index:index+24],joint,
                       node[parent+3:parent+6] if parent>=0 else (1,1,1))

def geometry(model,animation=None,frame=0):
    raw=Path(model).read_bytes()
    offs={name:u(raw,offset) for name,offset in [('shape',0x14c),('call',0x150),('pos',0x154),('ipos',0x158),('clr',0x164),('iclr',0x168),('itex',0x16c),('tex',0x18c),('mtx',0x190),('base',0x194),('texture',0x198),('draw',0x19c),('vis',0x1a0),('node',0x1a4),('group',0x1a8),('anim',0x1ac)]}
    node=np.frombuffer(raw,dtype='>f4',count=u(raw,0x140),offset=offs['node']).astype(float)
    pos=np.frombuffer(raw,dtype='>f4',count=u(raw,0xf0)*3,offset=offs['pos']).astype(float).reshape(-1,3)
    vis=list(raw[offs['vis']:offs['vis']+u(raw,0x13c)])
    texadd=[raw[offs['mtx']+n*24] for n in range(u(raw,0x12c))]
    if animation:
        matches=[offs['anim']+n*64 for n in range(u(raw,0x148)) if raw[offs['anim']+n*64:offs['anim']+n*64+60].split(b'\0')[0].decode()==animation]
        if not matches:raise ValueError(f'Animation not found: {animation}')
        a=u(raw,matches[0]+60);fo=a+u(raw,a+0x28)
        for k in range(u(raw,a+8)):
            key=fo+k*44
            if f(raw,key)>frame:break
            # Cumulative deltas at passed keyframes; interpolation is deferred.
            for field,kind,stride in [(4,'pos',4),(20,'texadd',12),(28,'vis',2),(36,'node',4)]:
                start,count=u(raw,key+field),u(raw,key+field+4)
                base=a+u(raw,a+{'pos':0x2c,'texadd':0x34,'vis':0x38,'node':0x3c}[kind])+start*stride
                index=0
                for j in range(count):
                    o=base+j*stride;index+=raw[o]
                    if kind=='node':node[index]+=struct.unpack_from('>b',raw,o+1)[0]/16
                    elif kind=='vis':vis[index]=struct.unpack_from('>b',raw,o+1)[0]==1
                    elif kind=='texadd':texadd[index]+=struct.unpack_from('>b',raw,o+1)[0]
                    else:pos[index]+=np.array(struct.unpack_from('>bbb',raw,o+1))/16
    triangles=[]
    groups=u(raw,0x144)
    def walk(index,parent_matrix,parent_group=-1,depth=0):
        if index<0:return
        if depth>100 or index>=groups:raise ValueError('Invalid character graph')
        g=offs['group']+index*88
        sibling,child,shape,visibility,nodeidx,ssc=struct.unpack_from('>iiiIII',raw,g+64)
        if vis[visibility]:
            parent_node=-1
            if ssc and parent_group>=0:
                pg=offs['group']+parent_group*88
                if u(raw,pg+84):parent_node=u(raw,pg+80)
            m=parent_matrix@matrix(node,nodeidx,parent_node,bool(ssc))
            if shape>=0:
                s=offs['shape']+shape*168
                pfirst,cfirst,tfirst=u(raw,s+64),u(raw,s+80),u(raw,s+88)
                drawstart,drawcount=u(raw,s+152),u(raw,s+156)
                for dn in range(drawcount):
                    d=offs['draw']+(drawstart+dn)*108;texcount=u(raw,d)
                    if texcount>1:raise ValueError('Multi-texture character draw needs explicit conversion')
                    texid=i(raw,d+16) if texcount else -1
                    if texid>=0:
                        textureidx=u(raw,offs['base']+texid*8)+texadd[texid]
                        tplidx=u(raw,offs['texture']+textureidx*64+4)
                    else:tplidx=-1
                    runstart,runcount=u(raw,d+56),u(raw,d+60)
                    ipstart,itstart=u(raw,d+64),u(raw,d+76)
                    for run in range(runcount):
                        c=offs['call']+(runstart+run)*8;first,count=u(raw,c),u(raw,c+4)
                        verts=[]
                        for n in range(count):
                            pi=u(raw,offs['ipos']+(first+ipstart+n)*4)+pfirst
                            p=m@np.array([*pos[pi],1])
                            uv=np.array([0.,0.])
                            if texcount:
                                ti=u(raw,offs['itex']+(first+itstart+n)*4)+tfirst
                                uv=np.array(struct.unpack_from('>ff',raw,offs['tex']+ti*8))
                            verts.append((p[:3],uv))
                        for n in range(1,count-1):triangles.append((tplidx,[verts[0],verts[n],verts[n+1]]))
            walk(child,m,index,depth+1)
        # Original sibling call retains the popped world matrix but supplies
        # CURRENT group as parent for joint scale compensation, even if hidden.
        walk(sibling,parent_matrix,index,depth+1)
    walk(groups-1,np.eye(4))
    return triangles

def bake(model,tpl,output,animation=None,frame=0,size=128):
    tris=geometry(model,animation,frame)
    if not tris:raise ValueError('Empty pose')
    points=np.array([p for _,tri in tris for p,_ in tri]);lo=points.min(axis=0);hi=points.max(axis=0)
    factor=(size-10)/max(hi[0]-lo[0],hi[1]-lo[1]);center=(lo[:2]+hi[:2])/2
    canvas=np.zeros((size,size,4),dtype=np.uint8);depth=np.full((size,size),-np.inf)
    cache={}
    for tid,verts in tris:
        if tid not in cache and tid>=0:
            imagepath=Path(output).parent/f'texture-{Path(model).name}-{tid}.png'
            convert_tpl(tpl,imagepath,tid);cache[tid]=np.array(Image.open(imagepath).convert('RGBA'))
        positions=np.array([v[0] for v in verts]);uv=np.array([v[1] for v in verts])
        xy=(positions[:,:2]-center)*factor+size/2;xy[:,1]=size-xy[:,1]
        xmin=max(0,int(np.floor(xy[:,0].min())));xmax=min(size-1,int(np.ceil(xy[:,0].max())))
        ymin=max(0,int(np.floor(xy[:,1].min())));ymax=min(size-1,int(np.ceil(xy[:,1].max())))
        if xmin>xmax or ymin>ymax:continue
        a,b,c=xy;den=(b[1]-c[1])*(a[0]-c[0])+(c[0]-b[0])*(a[1]-c[1])
        if abs(den)<1e-9:continue
        yy,xx=np.mgrid[ymin:ymax+1,xmin:xmax+1];xx=xx+.5;yy=yy+.5
        w0=((b[1]-c[1])*(xx-c[0])+(c[0]-b[0])*(yy-c[1]))/den
        w1=((c[1]-a[1])*(xx-c[0])+(a[0]-c[0])*(yy-c[1]))/den;w2=1-w0-w1
        weights=np.stack([w0,w1,w2],axis=-1);z=weights@positions[:,2]
        pixel=np.full((*xx.shape,4),255,dtype=np.uint8)
        if tid>=0:
            texture=cache[tid];coords=weights@uv
            # GX texture t=0 is the top row of the decoded image.
            tx=np.clip((coords[:,:,0]*texture.shape[1]).astype(int),0,texture.shape[1]-1)
            ty=np.clip((coords[:,:,1]*texture.shape[0]).astype(int),0,texture.shape[0]-1)
            pixel=texture[ty,tx]
        region=canvas[ymin:ymax+1,xmin:xmax+1];zregion=depth[ymin:ymax+1,xmin:xmax+1]
        mask=(w0>=-1e-6)&(w1>=-1e-6)&(w2>=-1e-6)&(z>=zregion)&(pixel[:,:,3]>8)
        region[mask]=pixel[mask];zregion[mask]=z[mask]
    Image.fromarray(canvas).save(output)
    return {'bounds':[lo.tolist(),hi.tolist()],'triangles':len(tris),'animation':animation,'frame':frame}
if __name__=='__main__':
    import argparse
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('model');p.add_argument('tpl');p.add_argument('output');p.add_argument('--animation');p.add_argument('--frame',type=float,default=0)
    a=p.parse_args();print(bake(a.model,a.tpl,a.output,a.animation,a.frame))
