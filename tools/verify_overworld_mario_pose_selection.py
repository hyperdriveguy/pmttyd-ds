#!/usr/bin/env python3
"""Pin ordinary Mario pose requests, rear selection and bounded animation clocks."""
import argparse
import hashlib
import json
import math
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO, inventory
from verify_overworld_roads import verify

MANIFEST=Path(__file__).with_name('overworld_mario_pose_selection_evidence.json')
def check_models(iso,evidence):
    files={r['path']:r for r in inventory(iso)['files']};count=0
    with Path(iso).open('rb') as disc:
        for model in evidence['actor_models']:
            row=files[model['path']];disc.seek(row['offset']);raw=disc.read(row['size'])
            if len(raw)!=model['size'] or hashlib.sha256(raw).hexdigest()!=model['sha256']:raise ValueError('Mario model changed')
            u=lambda a:struct.unpack_from('>I',raw,a)[0]
            if u(0x148)!=model['animation_count']:raise ValueError('Animation directory count changed')
            for anim in model['animations']:
                a=u(0x1ac)+anim['index']*64
                if raw[a:a+60].split(b'\0')[0].decode()!=anim['name']:raise ValueError('Requested pose index changed')
                v=u(a+60);lp=v+u(v+0x24);kp=v+u(v+0x28)
                flag,start,end=struct.unpack_from('>Iff',raw,lp)
                got=(v,flag,start,end,u(v+8),[struct.unpack_from('>f',raw,kp+j*44)[0] for j in range(u(v+8))])
                want=(anim['animation_offset'],anim['loop_flag'],anim['start'],anim['end'],anim['key_count'],anim['key_times'])
                if got!=want:raise ValueError('Pose loop/key metadata changed')
                count+=1
    return count

def derived():
    cases=[(0,True),(70,True),(70.001,False),(180,False),(289.999,False),(290,True),(359,True)]
    for heading,want in cases:assert (heading<=70 or heading>=290)==want
    def selected(t,loop,start,end):
        if end==start:return start
        if t<start:return start
        if loop:return t-math.trunc((t-start)/(end-start))*(end-start)
        return min(t,end)
    for t,loop,end,want in [(0,True,40,0),(40,True,40,0),(45,True,40,5),(2,False,2,2),(3,False,2,2),(-1,True,40,0)]:assert selected(t,loop,0,end)==want
    assert 100*60/1000==6 and 17*60/1000==1.02
    return 15



def check_clock_restart(evidence):
    # Literal queue/frame/force operands plus source-ordered request traces.
    expected={0x80057dbc:'93bf0018',0x80057dc4:'b01f0028',0x80057dd8:'60001000',
              0x80057fdc:'93bf0018',0x80057fe4:'b01f0028',0x80057ff8:'60001000',
              0x800578d8:'70605000',0x8005793c:'38a00001',0x8004120c:'2c1a0000',0x80041210:'40820010'}
    for address,word in expected.items():assert evidence['dol_anchors'][str(address)]==word
    def trace(old,requests):
        name=old;queued=False
        for requested in requests:
            if requested!=name:name=requested;queued=True
        return name,queued
    assert trace('M_W_1',['M_W_1'])==('M_W_1',False)
    assert trace('M_W_1',['M_W_1R'])==('M_W_1R',True)
    assert trace('M_W_1R',['M_W_1'])==('M_W_1',True)
    assert trace('M_W_1R',['M_W_1','M_W_1R'])==('M_W_1R',True)
    assert trace('M_S_1',['M_S_1'])==('M_S_1',False)
    assert trace('M_S_1R',['M_S_1','M_S_1R'])==('M_S_1R',True)
    return 6


def check_direction(evidence):
    expected={0x800583e0:.5,0x80058424:290.,0x80058430:250.,0x8005843c:180.,0x80058460:190.,0x80058470:350.,0x80058488:10.,0x80058498:170.,0x800585fc:10.,0x80058614:30.,0x80058644:20.,0x800586cc:170.,0x800586e4:210.,0x80058714:200.}
    for key,record in evidence['direction_literals'].items():
        word=int(evidence['dol_anchors'][key],16);imm=word&0xffff
        if imm&0x8000:imm-=0x10000
        assert word>>26==48 and ((word>>16)&31)==2
        assert 0x80427440+imm==record['address']
        value=struct.unpack('>f',bytes.fromhex(record['word']))[0]
        assert value==record['value']
        if int(key) in expected:assert value==expected[int(key)]
    point3=struct.unpack('>f',bytes.fromhex('3e99999a'))[0]
    def target(h,s,t):
        t=math.trunc(t+.5)
        if t in (290,250) and h>=180:t=0
        if s:
            if 190<=h<=350:t=0
            elif 10<=h<=170:t=180
        return t
    cases=[(189.99,1,180,180),(190,1,180,0),(350,1,180,0),(350.001,1,180,180),(9.999,1,0,0),(10,1,0,180),(170,1,0,180),(170.001,1,0,0),(0,1,180,180),(180,1,180,180),(90,0,0,0),(270,1,180,0)]
    for h,s,t,want in cases:assert target(h,s,t)==want
    def turn(d,t,c):
        initial=d
        if d==t:return d,c
        if t==0:
            if c&0x400:c&=~0x500
            c|=0x200
            d+=point3*(10-d) if c&0x100 else .125*(-30-d)
            if c&0x100:
                if d>=0:d=0;c&=~0x100
            elif d< -20:c|=0x100
        elif t==180:
            if c&0x200:c&=~0x300
            c|=0x400
            d+=point3*(170-d) if c&0x100 else .125*(210-d)
            if initial<=180:
                if c&0x100:
                    if d<=180:d=180;c&=~0x100
                elif d>200:c|=0x100
            elif not(c&0x100):
                if d>200:c|=0x100
                elif d<=180:d=180;c&=~0x100
        return d,c
    assert turn(0,180,0)==(26.25,0x400)
    assert turn(180,0,0)==(153.75,0x200)
    assert turn(-2,0,0x300)==(0,0x200)
    assert turn(0,0,0x500)==(0,0x500)
    assert turn(10,0,0x500)==(5,0x200)
    d,c=205.,0x500
    for _ in range(4):d,c=turn(d,180,c)
    assert d<180 and c&0x100
    d,c=turn(d,180,c);assert (d,c)==(180,0x400)
    def facing(s,h,air,saved_s,saved_h,count,current):
        if s and not air:
            if s>saved_s:saved_s,saved_h,count=s,h,14
            else:
                count-=1
                if count<=0:saved_s,saved_h,count=s,h,0
            current=saved_h
        else:saved_s=0
        return saved_s,saved_h,count,current
    state=facing(1,90,0,0,0,0,0);assert state==(1,90,14,90)
    for _ in range(13):state=facing(1,270,0,*state)
    assert state==(1,90,1,90)
    state=facing(1,270,0,*state);assert state==(1,270,0,270)
    assert facing(1,0,2,1,270,7,270)==(0,270,7,270)
    assert facing(0,0,0,1,270,7,270)==(0,270,7,270)
    return dict(direction_literal_loads=len(evidence['direction_literals']),sector_cases=len(cases),turn_cases=6,facing_traces=5)

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO)
    iso=p.parse_args().iso;e=json.loads(MANIFEST.read_text());result=verify(iso,MANIFEST)
    result.update(requested_model_poses=check_models(iso,e),derived_cases=derived(),clock_restart_traces=check_clock_restart(e),**check_direction(e))
    print(json.dumps(result,sort_keys=True))
