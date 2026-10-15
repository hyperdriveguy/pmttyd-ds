#!/usr/bin/env python3
"""Read-only pins for complete retained weapon UI constructor/subtypes/end graph."""
import argparse
import hashlib
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO

BODIES = {'BattleMenuDisp_WeaponSelect_Init': {'address': 2148622448,
                                      'size': 116,
                                      'sha256': '1358303410e47292ae9302c342d469d3a20364e0f7057181736da7d5be123d6b',
                                      'calls': [(2148622496, 2148628812),
                                                (2148622524, 2148628696),
                                                (2148622540, 2148628576)]},
 'BattleMenuDisp_WeaponSelect_End': {'address': 2148622348,
                                     'size': 64,
                                     'sha256': '240931985ee1d3b51df0365500c77fa2d570b106fe25abd5d42ea90c72845df7',
                                     'calls': [(2148622380, 2148501768)]},
 'InitSubMenuCommonProcess': {'address': 2148628812,
                              'size': 288,
                              'sha256': '557bc4802f3302e952c3f1a668510f4e7d16a7204d4508533adadbd06a2c7fde',
                              'calls': [(2148628860, 2148501768),
                                        (2148628868, 2148501812),
                                        (2148628940, 2148501708)]},
 'InitSubMenuCommonProcess2': {'address': 2148628696,
                               'size': 116,
                               'sha256': 'cf70ab7a3d2dff336027e66594e0e193d0be4627c0a515f4dfefef87fbd72fde',
                               'calls': []},
 'InitSubMenuCommonProcess3': {'address': 2148628576,
                               'size': 120,
                               'sha256': 'e3e8dcaa504becc5f4ad136921794c4b1267dd88256a20ab1dd99e3f4bcbd708',
                               'calls': []},
 'BattleAlloc': {'address': 2148501812,
                 'size': 40,
                 'sha256': '5225f27b27e3e163d13c50a2bca3b38c4667e379e2c66eb8527316b279d81492',
                 'calls': [(2148501832, 2147680088)]},
 'BattleFree': {'address': 2148501768,
                'size': 44,
                'sha256': '4513c22a59f470372e6b7fe43710f4dc607a88f21d53631b14dcb90dac3ada4d',
                'calls': [(2148501792, 2147679684)]},
 'BattleGetUnitPtr': {'address': 2148501708,
                      'size': 60,
                      'sha256': '6a40e068a63873b3f93f3cf0bdc72c59323e60ff5b961ac740b22eddb58276fa',
                      'calls': []},
 '_mapAlloc': {'address': 2147680088,
               'size': 240,
               'sha256': 'f457c8b4a83980980f9cf99644459790574ac068700007511054775331143425',
               'calls': [(2147680276, 2147504308), (2147680288, 2150197844)]},
 '_mapFree': {'address': 2147679684,
              'size': 156,
              'sha256': '07af55b1249474eef33b6adfa025e4a5ef12cb624c0c9b77f2b6aabe0c9b52ba',
              'calls': []}}
LITERAL = '00000000'

def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as disc:
        if disc.read(8)!=b'G8ME01\0\0':
            raise ValueError('US G8ME01 disc0 revision0 required')
        disc.seek(0x420)
        dol=struct.unpack('>I',disc.read(4))[0]
        disc.seek(dol)
        header=disc.read(256)
        def read(address,size):
            spans=[]
            for i in range(18):
                off,va,n=(struct.unpack_from('>I',header,b+4*i)[0] for b in (0,0x48,0x90))
                if off and va<=address and address+size<=va+n:
                    spans.append(dol+off+address-va)
            if len(spans)!=1:
                raise ValueError('Unmapped/ambiguous source')
            disc.seek(spans[0])
            raw=disc.read(size)
            if len(raw)!=size:
                raise ValueError('Truncated source')
            return raw
        count=0
        for name,expected in BODIES.items():
            raw=read(expected['address'],expected['size'])
            if hashlib.sha256(raw).hexdigest()!=expected['sha256']:
                raise ValueError('Whole original body changed: '+name)
            calls=[]
            for i in range(0,len(raw),4):
                word=struct.unpack_from('>I',raw,i)[0]
                if word & 0xfc000003 == 0x48000001:
                    relative=word & 0x03fffffc
                    if relative & 0x02000000:
                        relative-=0x04000000
                    calls.append((expected['address']+i,(expected['address']+i+relative)&0xffffffff))
            if calls!=expected['calls']:
                raise ValueError('Original whole child census changed: '+name)
            count+=len(calls)
        if read(0x8042242c,4).hex()!=LITERAL:
            raise ValueError('Original work98 literal changed')
    return dict(whole_bodies=len(BODIES),original_calls=count,literal_address='8042242c',literal_hex=LITERAL,limitation='Static source pins, no executed-original or native display proof')

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
