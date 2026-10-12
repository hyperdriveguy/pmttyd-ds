#!/usr/bin/env python3
"""Pin camera interpolation dispatch, authored modes and raw tick producer domain."""
import argparse
import collections
import json
import math
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO, inventory
from verify_overworld_roads import verify

MANIFEST=Path(__file__).with_name('overworld_camera_interpolation_evidence.json')
def derived():
    pi=struct.unpack('>f',bytes.fromhex('40490fd8'))[0]
    half=struct.unpack('>f',bytes.fromhex('3fc90fd8'))[0]
    def calc(mode,s,n,a,b):
        if n==0:return b
        d=b-a;r=n-s
        if mode<=3:return a+d*(s/n)**(mode+1)
        if mode<=6:return a+d-d*(r/n)**(mode-2)
        if mode==7:return b-(r/n)**2*d*math.cos(4*pi*s/n)
        if mode in (8,9):return b-((r if mode==8 else s)/n)**2*d*math.cos(4*pi*(s/n)**2/.15)
        if mode==10:return b-abs((r/n)**2*math.cos(4*pi*(s/n)**2/.4))*d
        if mode==11:return a+d*(1-math.cos(pi*s/n))*.5
        if mode==12:return a+d*math.sin(half*s/n)
        if mode==13:return a+d*(1-math.cos(half*s/n))
        raise ValueError('Indirect interpolation callback')
    for mode in range(14):assert calc(mode,4,0,2,8)==8
    for mode in (0,1,2,3):assert calc(mode,50,100,2,8)==2+6*.5**(mode+1)
    for mode in (4,5,6):assert calc(mode,50,100,2,8)==8-6*.5**(mode-2)
    assert calc(0,-1,10,0,1)==-.1 and calc(0,11,10,0,1)==1.1
    assert calc(7,25,100,0,1)>1 and calc(9,0,100,0,1)==1
    assert 0<calc(12,50,100,0,1)<1
    assert calc(13,100,100,0,1)<1
    for hz in (40500000,40000001,1000,999):
        got=((hz*0x10624dd3)>>32)>>6
        assert got==hz//1000
    return 31

def check_requests(iso,evidence):
    row=next(r for r in inventory(iso)['files'] if r['path']=='rel/gor.rel')
    with Path(iso).open('rb') as disc:disc.seek(row['offset']);raw=disc.read(row['size'])
    hist=collections.defaultdict(collections.Counter)
    for call in evidence['basic_requests']:
        a=call['offset'];operands=call['operands'];n=len(operands)+1
        if struct.unpack_from('>I',raw,a)[0] != (n<<16)|91:raise ValueError('Camera event request header mismatch')
        if list(struct.unpack_from('>'+str(len(operands))+'I',raw,a+8))!=operands:raise ValueError('Camera event operands mismatch')
        hist[call['function']][str(operands[-1])]+=1
    got={name:dict(sorted(counts.items())) for name,counts in hist.items()}
    if got!=evidence['curve_histogram']:raise ValueError('Authored camera curve census mismatch')
    return len(evidence['basic_requests'])

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO)
    iso=p.parse_args().iso;e=json.loads(MANIFEST.read_text());result=verify(iso,MANIFEST)
    result.update(basic_requests=check_requests(iso,e),derived_cases=derived())
    print(json.dumps(result,sort_keys=True))
