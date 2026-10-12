#!/usr/bin/env python3
"""Pin Mario toMovedir heading quantization and ordinary probe callers."""
import argparse
import json
import math
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_overworld_roads import verify

MANIFEST=Path(__file__).with_name('overworld_display_to_move_evidence.json')
def heading(d,offset=0,special=False,current=0):
    if special:
        if current in (280,460):d=0
        elif current in (260,440):d=180
        else:d=0 if current%360>=180 else 180
    a=270-d+offset
    q=math.trunc(a*1000+(.5 if a>=0 else -.5))/1000
    return q%360
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO)
    result=verify(p.parse_args().iso,MANIFEST)
    cases=[((0,0),270),((180,0),90),((26.25,0),243.75),((-30,0),300),((210,0),60),((0,90),0),((0,-300),330),((270.0006,0),359.999),((269.9994,0),.001)]
    for args,want in cases:assert abs(heading(*args)-want)<1e-9
    special=[(280,270),(460,270),(260,90),(440,90),(180,270),(179,90),(-1,270),(361,90)]
    for d,want in special:assert heading(123,special=True,current=d)==want
    result.update(derived_cases=len(cases)+len(special))
    print(json.dumps(result,sort_keys=True))
