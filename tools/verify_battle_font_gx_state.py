#!/usr/bin/env python3
"""Source pins backing the finite original-US native font GX profile."""
import argparse
import json
import struct
from field_map_npc_assets import Dol
from iso_assets import DEFAULT_ISO
from verify_source_font_draw_start import verify as verify_start
from verify_source_font_state import verify as verify_state

def verify(iso=DEFAULT_ISO):
    source=Dol(iso)
    expected={0x804203e4:1.,0x804203ec:1/512,0x804203f0:1/1024,0x804203f4:1/256,0x804203f8:0.}
    for address,value in expected.items():
        if source.read(address,4)!=struct.pack('>f',value):
            raise ValueError('Original font texture/fog scalar differs at '+hex(address))
    return dict(draw_start=verify_start(iso), shared_font_state=verify_state(iso),
                exact_scalars={hex(a):v for a,v in expected.items()},
                limitation='Finite native state translation; source pins do not establish GPU pixels, static object production or real camera admission.')

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    print(json.dumps(verify(parser.parse_args().iso),indent=2))
