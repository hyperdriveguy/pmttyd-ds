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

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO)
    iso=p.parse_args().iso;e=json.loads(MANIFEST.read_text());result=verify(iso,MANIFEST)
    result.update(requested_model_poses=check_models(iso,e),derived_cases=derived())
    print(json.dumps(result,sort_keys=True))
