#!/usr/bin/env python3
"""Pin original bottomless start probe and source screen-service contract."""
import argparse
import json
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_overworld_roads import verify

MANIFEST=Path(__file__).with_name('overworld_bottomless_probe_evidence.json')
if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--iso',default=DEFAULT_ISO)
    result=verify(parser.parse_args().iso,MANIFEST)
    cases=[((0.,0.),True),((600.,535.),True),((600.99,535.99),True),((-.99,-.99),True),((-1.,0.),False),((601.,0.),False),((0.,536.),False)]
    for (x,y),want in cases:
        got=0<=int(x)<=600 and 0<=int(y)<=535
        if got!=want:raise ValueError('Derived source projection classifier mismatch')
    result['derived_classifier_cases']=len(cases)
    print(json.dumps(result,sort_keys=True))
