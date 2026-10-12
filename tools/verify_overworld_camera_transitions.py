#!/usr/bin/env python3
"""Pin ordinary camera road transition owners and finite source-derived cases."""
import argparse
import json
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_overworld_roads import verify

MANIFEST=Path(__file__).with_name('overworld_camera_transition_evidence.json')
def derived_admission(a_record,b_record,a_zone,b_zone,b_flags,a_segment,b_segment,start,end):
    """Only pre-cost mask/early admission; never substitutes original math."""
    mask=0
    if a_record==b_record and not ((a_zone==1 and b_zone==0) or (a_zone==0 and b_flags==1)):
        gap=abs(int(a_segment)-int(b_segment));span=end-start
        if gap<=1 or gap>=span-1:return None
        if gap>=span-2:mask|=32
    if a_record!=b_record:mask|=1
    if a_zone==1 and b_zone==0:mask|=2
    if a_zone==0 and b_flags==1:mask|=4
    if abs(a_segment-b_segment)>1:mask|=8
    return mask

def verify_transitions(iso):
    result=verify(iso,MANIFEST)
    cases=[((1,1,1,1,0,2.,3.,2,3),None),
           ((1,1,1,0,0,2.,3.,2,3),2),
           ((1,1,0,1,1,2.,3.,2,3),4),
           ((1,1,0,1,3,2.,3.,2,3),None),
           ((1,2,1,1,0,2.,3.,2,3),1),
           ((1,1,1,1,0,2.,4.,0,5),8),
           ((1,1,1,1,0,2.,5.,0,5),40),
           ((1,1,1,1,0,2.,6.,0,5),None)]
    for args,expected in cases:
        if derived_admission(*args)!=expected:raise ValueError('Derived transition admission mismatch')
    result['derived_admission_cases']=len(cases)
    return result
if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--iso',default=DEFAULT_ISO)
    print(json.dumps(verify_transitions(parser.parse_args().iso),sort_keys=True))
