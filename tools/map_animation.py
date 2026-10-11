"""Offline original map mesh tracks; format reference noclip.world.ts (MIT).
Explicit frame sampling only: event selection/looping is not inferred.
"""
import math
import struct
from pathlib import Path
import numpy as np
from character_bake import trans,rotate

def read_animations(path):
    raw=Path(path).read_bytes();base=32
    def unpack(fmt,o):
        try:return struct.unpack_from('>'+fmt,raw,o)
        except struct.error as e:raise ValueError('Truncated map animation') from e
    def u(o):return unpack('I',o)[0]
    def string(o):
        if o<base or o>=len(raw):raise ValueError('Invalid animation string pointer')
        try:return raw[o:raw.index(b'\0',o)].decode('ascii')
        except (ValueError,UnicodeError) as e:raise ValueError('Invalid animation string') from e
    def count(o,limit=10000):
        n=u(o)
        if n>limit:raise ValueError('Excessive animation count')
        return n
    table=base+u(4)+u(8)*4;names=table+u(12)*8
    chunks={string(names+u(table+n*8+4)):base+u(table+n*8) for n in range(count(12))}
    table=chunks.get('animation_table')
    if table is None:return []
    result=[]
    for n in range(count(table)):
        entry=base+u(table+4+n*4);duration=unpack('f',entry+8)[0]
        if not math.isfinite(duration) or duration<0:raise ValueError('Invalid animation duration')
        animation={'name':string(base+u(entry)),'duration':duration,'tracks':[],
                   'material_tracks':[],'material_table_offset':u(entry+16)}
        tracks=u(entry+12)
        if tracks:
            tracks+=base
            for j in range(count(tracks)):
                track=base+u(tracks+4+j*4);offsets=list(unpack('9f',track+4));frames=[]
                if not all(map(math.isfinite,offsets)) or any(v==0 for v in offsets[6:]):raise ValueError('Invalid animation offsets')
                previous=-math.inf
                for k in range(count(track+88)):
                    frame=track+92+k*424;time=unpack('f',frame)[0];components=[]
                    if not math.isfinite(time) or time<=previous:raise ValueError('Unordered animation keyframes')
                    previous=time
                    for c in range(9):
                        p=frame+4+c*20;values=list(unpack('3f',p));step=u(p+16)
                        if not all(map(math.isfinite,values)) or step not in (0,1):raise ValueError('Invalid animation component')
                        components.append([*values,bool(step)])
                    frames.append({'time':time,'components':components})
                if not frames:raise ValueError('Empty mesh track')
                animation['tracks'].append({'node':string(base+u(track)),'offsets':offsets,'frames':frames})
        materials=u(entry+16)
        if materials:
            materials+=base
            for j in range(count(materials)):
                track=base+u(materials+4+j*4);center=list(unpack('2f',track+8));frames=[]
                if not all(map(math.isfinite,center)):raise ValueError('Invalid material animation center')
                previous=-math.inf
                for k in range(count(track+16)):
                    frame=track+20+k*104;time=unpack('f',frame)[0];components=[]
                    if not math.isfinite(time) or time<=previous:raise ValueError('Unordered material keyframes')
                    previous=time
                    for c in range(5):
                        point=frame+4+c*20;values=list(unpack('3f',point));step=u(point+16)
                        if not all(map(math.isfinite,values)) or step not in (0,1):raise ValueError('Invalid material component')
                        components.append([*values,bool(step)])
                    frames.append({'time':time,'components':components})
                if not frames:raise ValueError('Empty material track')
                texgen=u(track+4)
                if texgen>7:raise ValueError('Invalid material texgen')
                animation['material_tracks'].append({'material':string(base+u(track)), 'texgen':texgen,'center':center,'frames':frames})
        result.append(animation)
    return result

def interpolate(a,b,t,d):
    if a[3]:return a[0]
    return (2*t**3-3*t**2+1)*a[0]+(t**3-2*t**2+t)*a[2]*d+(-2*t**3+3*t**2)*b[0]+(t**3-t**2)*b[1]*d

def sample_values(track,frame):
    if not math.isfinite(frame):raise ValueError('Invalid animation frame')
    frames=track['frames'];a=b=frames[0]
    if frame>=frames[-1]['time']:a=b=frames[-1]
    elif frame>frames[0]['time']:
        for left,right in zip(frames,frames[1:]):
            if frame<right['time']:a,b=left,right;break
    d=b['time']-a['time'];t=(frame-a['time'])/d if d else 0
    return [interpolate(x,y,t,d) for x,y in zip(a['components'],b['components'])]

def sample(track,frame):
    values=sample_values(track,frame);offset=track['offsets']
    translation=np.array(values[:3])-offset[:3];rotation=np.array(values[3:6])-offset[3:6];scale=np.array(values[6:])/offset[6:]
    return trans(translation)@rotate(rotation)@np.diag([*scale,1])

def texture_matrix(ts,tt,ss,st,angle,cs,ct):
    values=[ts,tt,ss,st,angle,cs,ct]
    if not all(map(math.isfinite,values)):raise ValueError('Invalid texture transform')
    angle=math.radians(-angle)
    rotation=np.array([[math.cos(angle),-math.sin(angle),0],[math.sin(angle),math.cos(angle),0],[0,0,1]])
    center=np.array([.5*cs,.5*ct-1]);scale=np.diag([ss,st,1])
    pre=np.eye(3);pre[:2,2]=np.array([ss,st])*center
    post=np.eye(3);post[:2,2]=-np.array([ss,st])*center
    result=scale@post@rotation@pre;result[:2,2]+=np.array([ts,-tt])
    return result

def sample_material(track,frame):
    return texture_matrix(*sample_values(track,frame),*track['center'])

if __name__=='__main__':
    import argparse,json
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('input');p.add_argument('--output',required=True);args=p.parse_args()
    Path(args.output).write_text(json.dumps(read_animations(args.input),indent=2)+'\n')
