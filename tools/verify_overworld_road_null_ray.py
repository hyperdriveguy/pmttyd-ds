#!/usr/bin/env python3
"""Pin active-object and downward triangle acceptance for original NULL road rays."""
import argparse
import json
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_overworld_roads import verify

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    args=parser.parse_args()
    print(json.dumps(verify(args.iso,Path(__file__).with_name('overworld_road_null_ray_evidence.json')),sort_keys=True))
