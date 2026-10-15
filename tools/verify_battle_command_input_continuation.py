#!/usr/bin/env python3
"""Pin retained Input child boundaries against the complete read-only source map."""
import argparse
from pathlib import Path
import re
from iso_assets import DEFAULT_ISO
from verify_command_input import verify as verify_source, ANCHORS

def verify(iso=DEFAULT_ISO):
    result=verify_source(iso)
    source_calls={}
    for address,raw in ANCHORS.items():
        if not 0x801229d8 <= address < 0x80123210:
            continue
        word=int(raw,16)
        if word & 0xfc000003 == 0x48000001:
            relative=word & 0x03fffffc
            if relative & 0x02000000:
                relative-=0x04000000
            source_calls[address]=(address+relative)&0xffffffff
    implementation=(Path(__file__).resolve().parents[1]/'source/battle/battle_command_input_continuation.cpp').read_text()
    retained={int(site,16):int(function,16) for site,function in re.findall(
        r'case (0x[0-9a-f]+): \{\s*auto result=c.call\((0x[0-9a-f]+),',implementation)}
    if retained != source_calls:
        raise ValueError('Retained Input lost/changed an original callsite boundary')
    result['retained_original_callsites']=len(retained)
    result['limitation']='Static source evidence and boundary census; no original execution/native scheduling proof'
    return result

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
