#!/usr/bin/env python3
"""Pin whole original US statusGetValue and its actual linked child census."""
import argparse
import hashlib
import struct
from field_map_npc_assets import Dol
from iso_assets import DEFAULT_ISO
ADDRESS=0x8013c9ac
SIZE=0x178
SHA256='bc743f6eb6f985fd41b52dd17d76d542b1939081d4de853b7d0b73c003d473fd'
CALLS=[(2148780504, 2148351280), (2148780512, 2148351268), (2148780520, 2148351128), (2148780528, 2148351116), (2148780536, 2148351044), (2148780544, 2148350904), (2148780552, 2148348964), (2148780560, 2148351588), (2148780568, 2147860836), (2148780580, 2148798696), (2148780592, 2147868864), (2148780608, 2148359856), (2148780632, 2148359856), (2148780668, 2148501416), (2148780680, 2148501288), (2148780720, 2148351044), (2148780728, 2148350904), (2148780736, 2148348964), (2148780744, 2148351588), (2148780764, 2148500548)]
def verify(iso=DEFAULT_ISO):
    dol=Dol(iso)
    try:
        body=dol.read(ADDRESS,SIZE)
    finally:
        dol.close()
    if hashlib.sha256(body).hexdigest()!=SHA256:
        raise ValueError('Original whole statusGetValue changed')
    calls=[]
    for offset in range(0,len(body),4):
        word=struct.unpack_from('>I',body,offset)[0]
        if word&0xfc000003==0x48000001:
            relative=word&0x03fffffc
            if relative&0x02000000:
                relative-=0x04000000
            calls.append((ADDRESS+offset,(ADDRESS+offset+relative)&0xffffffff))
    if calls!=CALLS:
        raise ValueError('Original status value child census changed')
    return dict(whole_bodies=1,linked_calls=len(calls),sha256=SHA256,
                scope='Static source pins only; real getter children, live source mode/memory and actual output ownership remain required')
if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
