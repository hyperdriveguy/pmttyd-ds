#!/usr/bin/env python3
"""Pin ordinary camera dolly/event consumers and basic producer lifetimes."""
import argparse
import json
import math
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_overworld_roads import verify

MANIFEST = Path(__file__).with_name('overworld_camera_event_blends_evidence.json')
def f32(x):
    return struct.unpack('>f', struct.pack('>f', x))[0]
def curve11(step):
    pi = struct.unpack('>f', bytes.fromhex('40490fd8'))[0]
    return (1 - f32(math.cos(f32(f32(pi * step) / 100000)))) * .5

def derived():
    assert curve11(0) == 0 and curve11(100000) == 1
    eye, at = (10., 20., 30.), (20., 40., 50.)
    assert tuple(e + (a-e)*0/100 for e,a in zip(eye,at)) == eye
    assert tuple(e + (a-e)*100/100 for e,a in zip(eye,at)) == at
    assert tuple(e + (a-e)*50/100 for e,a in zip(eye,at)) == (15.,30.,40.)
    assert tuple(e + (a-e)*-50/100 for e,a in zip(eye,at)) == (5.,10.,20.)
    for state,elapsed,duration,want in [(1,10,10,(1,0.)),(1,11,10,(0,0.)),(3,10,10,(3,1.)),(3,11,10,(0,1.)),(1,0,0,(0,0.)),(3,0,0,(0,1.))]:
        if duration <= 0: got=(0,0. if state==1 else 1.)
        else:
            p=1-elapsed/duration if state==1 else elapsed/duration
            got=(0,0.) if state==1 and p<0 else (0,1.) if state!=1 and p>1 else (state,p)
        assert got==want
    previous,anchor,t=(1.,2.,3.),(4.,6.,8.),.5
    follow=tuple(p*(1-t)+a*t for p,a in zip(previous,anchor))
    look=follow
    assert follow==look==(2.5,4.,5.5)
    return 13

if __name__ == '__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO)
    result=verify(p.parse_args().iso,MANIFEST)
    result['derived_cases']=derived()
    print(json.dumps(result,sort_keys=True))
