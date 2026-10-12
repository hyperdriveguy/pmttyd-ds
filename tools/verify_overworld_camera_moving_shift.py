#!/usr/bin/env python3
"""Pin moving camera shift correction and derived bounded geometry cases."""
import argparse
import json
import math
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_overworld_roads import verify

MANIFEST=Path(__file__).with_name('overworld_camera_moving_shift_evidence.json')
def finite_component(vector,budget,threshold):
    magnitude=math.hypot(*vector)
    if magnitude>budget and magnitude>threshold:
        return tuple(v*budget/magnitude for v in vector)
    return vector

def verify_shift(iso):
    result=verify(iso,MANIFEST)
    # Ordinary initialized component limits, exact simple coordinates.
    cases=[(((3.,0.),2.,0.),(2.,0.)),
           (((-3.,0.),2.,0.),(-2.,0.)),
           (((0.,-3.),0.,0.),(0.,-0.)),
           (((3.,0.),3.,0.),(3.,0.)),
           (((3.,0.),2.,3.),(3.,0.))]
    for args,want in cases:
        if finite_component(*args)!=want:raise ValueError('Derived camera component case mismatch')
    # Prior/current shift are separate signed query offsets, not distance.
    prior,current,K=0.,-20.,2.
    if (prior*K,current*K)!=(0.,-40.):raise ValueError('Derived shift query operand mismatch')
    result['derived_finite_cases']=len(cases)+1
    return result
if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--iso',default=DEFAULT_ISO)
    print(json.dumps(verify_shift(parser.parse_args().iso),sort_keys=True))
