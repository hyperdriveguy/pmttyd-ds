#!/usr/bin/env python3
"""Pin original US stage-floor bodies/constants, read-only; static evidence only."""
import argparse
import hashlib
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO
BODIES = {
    'TransStageFloorPosition': (0x8010510c, 0xc0, 'e179ac9992f4ca2f2f99224d7b5954b194301012874f9720d577513038dfaa52'),
    'BattleGetFloorHeight': (0x800f7b84, 0x50, 'da651e064b9f49e0f750687f1e78ba167ddb0a72cc618e4359ec849c1a865923'),
    'BattleStageGetPtr': (0x80139864, 0x10, 'a57bcf41694fc253d3277e8b7a18da97f062317ed8cbd4a78305fae4fad005de'),
    'BattleStageInit': (0x80139780, 0xe4, 'c692d3c176e90df5800a44f8416eb8f1ad5772a59318f60756544db5af0e6bf2'),
}
CONSTANTS = {0x80422b44: '42b40000c2480000', 0x80422170: '00000000'}
def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as disc:
        header=disc.read(0x440)
        if header[:8] != b'G8ME01\0\0':
            raise ValueError('US disc0 revision0 required')
        base=struct.unpack_from('>I',header,0x420)[0]
        disc.seek(base);sections=disc.read(256)
        def read(address,size):
            for i in range(18):
                offset,start,length=[struct.unpack_from('>I',sections,k+4*i)[0] for k in (0,0x48,0x90)]
                if offset and start<=address and address+size<=start+length:
                    disc.seek(base+offset+address-start);data=disc.read(size)
                    if len(data)!=size:raise ValueError('Truncated DOL')
                    return data
            raise ValueError('Unmapped source address')
        for name,(address,size,digest) in BODIES.items():
            if hashlib.sha256(read(address,size)).hexdigest()!=digest:
                raise ValueError('Source body mismatch: '+name)
        for address,data in CONSTANTS.items():
            if read(address,len(data)//2).hex()!=data:
                raise ValueError('Source floor constant mismatch')
    return {'bodies':len(BODIES),'constants':len(CONSTANTS),'scope':'Static US source; no original execution'}
if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    print(verify(parser.parse_args().iso))
