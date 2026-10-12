"""Bounded ordinary Mario animation import and host reference evaluation.
Source contract: docs/behavior/overworld/OVERWORLD_MARIO_ANIMATION_SPARSE_UPDATES.md.
Host double/trig evaluation is an arithmetic adapter, not original runtime parity.
"""
import bisect
import math
import struct

RAD=struct.unpack('>f',bytes.fromhex('3c8efa33'))[0]
def u(raw,a):return struct.unpack_from('>I',raw,a)[0]
def f(raw,a):return struct.unpack_from('>f',raw,a)[0]
def s8(raw,a):return struct.unpack_from('>b',raw,a)[0]
def animation(raw,name):
    directory=u(raw,0x1ac)
    match=[directory+i*64 for i in range(u(raw,0x148)) if raw[directory+i*64:directory+i*64+60].split(b'\0')[0].decode()==name]
    if len(match)!=1:raise ValueError('Expected one named animation')
    a=u(raw,match[0]+60)
    if u(raw,a+4)!=1:raise ValueError('Bounded ordinary animation needs one loop descriptor')
    loop,start,end=struct.unpack_from('>Iff',raw,a+u(raw,a+0x24))
    frames=[];stats={'position':0,'normal':0,'texture':0,'visibility':0,'node':0,'instant_node':0}
    streams=[('position',4,4,0x2c),('normal',12,4,0x30),('texture',20,12,0x34),('visibility',28,2,0x38),('node',36,4,0x3c)]
    counts={'position':u(raw,0xf0),'normal':u(raw,0xf8),'texture':u(raw,0x12c),'visibility':u(raw,0x13c),'node':u(raw,0x140)}
    for i in range(u(raw,a+8)):
        k=a+u(raw,a+0x28)+i*44;frame={'time':f(raw,k)}
        for kind,field,stride,base in streams:
            offset,count=u(raw,k+field),u(raw,k+field+4)
            if offset+count>u(raw,a+base-0x20):raise ValueError('Sparse update range exceeds stream')
            pointer=a+u(raw,a+base)+offset*stride;records=[];index=0
            for j in range(count):
                p=pointer+j*stride;index+=raw[p]
                if index>=counts[kind]:raise ValueError('Sparse animation target exceeds buffer')
                if kind in ('position','normal'):record=(index,tuple(s8(raw,p+x) for x in (1,2,3)))
                elif kind=='texture':record=(index,raw[p+1],f(raw,p+4),f(raw,p+8))
                elif kind=='visibility':record=(index,raw[p+1])
                else:
                    record=(index,s8(raw,p+1),s8(raw,p+2),s8(raw,p+3))
                    if not (-89<record[3]<89):stats['instant_node']+=1
                    elif not (-89<=record[2]<89):raise ValueError('Partial tangent IN outside initialized source table')
                records.append(record)
            frame[kind]=records;stats[kind]+=count
        frames.append(frame)
    times=[k['time'] for k in frames]
    if not frames or times[0]!=start or times[-1]!=end or any(a>=b for a,b in zip(times,times[1:])):raise ValueError('Outside ordinary strictly increasing endpoint-key domain')
    positions=[list(struct.unpack_from('>3f',raw,u(raw,0x154)+i*12)) for i in range(counts['position'])]
    normals=[list(struct.unpack_from('>3f',raw,u(raw,0x15c)+i*12)) for i in range(counts['normal'])]
    nodes=list(struct.unpack_from('>'+str(counts['node'])+'f',raw,u(raw,0x1a4)))
    visibility=list(raw[u(raw,0x1a0):u(raw,0x1a0)+counts['visibility']])
    textures=[list(struct.unpack_from('>5f',raw,u(raw,0x190)+i*24+4)) for i in range(counts['texture'])]
    texture_indices=[raw[u(raw,0x190)+i*24] for i in range(counts['texture'])]
    return dict(name=name,loop=bool(loop),start=start,end=end,frames=frames,stats=stats,
                positions=positions,normals=normals,nodes=nodes,visibility=visibility,textures=textures,texture_indices=texture_indices)

def node_delta(value,angle_in,angle_out,q,duration):
    if angle_out<=-89 or angle_out>=89:return 0.
    q2=q*q;q3=q*q2
    tin=math.tan(RAD*angle_in);tout=math.tan(RAD*angle_out)
    return (q3-2*q2+q)*duration*tin + (3*q2-2*q3)*value/16 + (q3-q2)*duration*tout

def evaluate(anim,frame_time):
    """Stateless ordinary output buffers; host double/trig numeric adaptation."""
    start,end=anim['start'],anim['end'];duration=end-start
    if duration==0:t=start
    elif frame_time<start:t=start
    elif anim['loop']:t=frame_time-math.trunc((frame_time-start)/duration)*duration
    else:t=min(frame_time,end)
    keys=anim['frames'];times=[k['time'] for k in keys];last=bisect.bisect_right(times,t)-1
    nxt=last+1
    if nxt>=len(keys):nxt=0 if anim['loop'] else last
    if nxt==last:span=0.;q=0.
    else:
        span=(end-times[last]) if nxt==0 else times[nxt]-times[last]
        q=(t-times[last])/span if span else 0.
    out={k:([list(x) for x in anim[k]] if k in ('positions','normals','textures') else list(anim[k])) for k in ('positions','normals','nodes','visibility','textures','texture_indices')}
    for key in keys[:last+1]:
        for kind,buffer in [('position','positions'),('normal','normals')]:
            for index,delta in key[kind]:
                for axis in range(3):out[buffer][index][axis]+=delta[axis]/16
        for index,value,_,_ in key['node']:out['nodes'][index]+=value/16
        for index,value in key['visibility']:out['visibility'][index]=(out['visibility'][index]+value)&255
        for index,delta,s,tv in key['texture']:
            out['texture_indices'][index]=(out['texture_indices'][index]+delta)&255
            out['textures'][index][0]+=s;out['textures'][index][1]+=tv
    if last<nxt:
        key=keys[nxt]
        for kind,buffer in [('position','positions'),('normal','normals')]:
            for index,delta in key[kind]:
                for axis in range(3):out[buffer][index][axis]+=delta[axis]*q/16
        for index,value,tin,tout in key['node']:out['nodes'][index]+=node_delta(value,tin,tout,q,span)
    out.update(selected_time=t,last=last,next=nxt,fraction=q)
    return out

