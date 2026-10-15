#!/usr/bin/env python3
"""Read-only original weapon queued source graph evidence for native admission."""
import argparse
import json
from iso_assets import DEFAULT_ISO
from verify_battle_weapon_draw_continuation import verify as verify_weapon

def verify(iso=DEFAULT_ISO):
    source=verify_weapon(iso)
    return dict(weapon_source=source, native_callback='80121ce8', camera=8,
                limitation='Original source pins only. Allocation/Main/queue/batch admission is explicit native policy, not executed PPC or normal activation proof.')

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--iso',default=DEFAULT_ISO)
    print(json.dumps(verify(p.parse_args().iso),indent=2))
