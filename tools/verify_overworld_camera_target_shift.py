#!/usr/bin/env python3
"""Verify original ordinary camera contact-target and secondary shift owners."""
import argparse
import json
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_overworld_roads import verify

MANIFEST = Path(__file__).with_name('overworld_camera_target_shift_evidence.json')
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso', default=DEFAULT_ISO)
    print(json.dumps(verify(parser.parse_args().iso, MANIFEST), sort_keys=True))
