#!/usr/bin/env python3
"""Read original weapon icon bindings and equipment icon helpers, read-only."""
import hashlib
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO
BODIES={
    0x80124c10:(0x54,'d3dc333feea050e7cb7c2a870e3373133e1c7bd102247a474249b9466f8d1465'),
    0x80124c64:(0x54,'9b2f3081552c28dfcf88811f31ab9ce3c0df1788553d1ab1a1b21b4bc2c220bd'),
    0x803538a8:(0xc0,'f60b156b26f892f6d8ea90846bc06edb4f6023833b37380dbe1b28e130810e3c'),
    0x80353de8:(0xc0,'eff0775050243d3d51dcc107c1cb479e20e086baa19a2cfdc07030d5e02b303a'),
    0x80393ea8:(0xc0,'931e4cded0fc6f1703a5248c0336da745f84b2137c17c14713bd449949aaadab'),
}
def source_art(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as disc:
        header=disc.read(0x440)
        if header[:8]!=b'G8ME01\0\0':raise ValueError('US disc0 revision0 required')
        base=struct.unpack_from('>I',header,0x420)[0];disc.seek(base);sections=disc.read(256)
        def read(address,size):
            for i in range(18):
                offset,start,length=[struct.unpack_from('>I',sections,k+4*i)[0] for k in (0,0x48,0x90)]
                if offset and start<=address and address+size<=start+length:
                    disc.seek(base+offset+address-start);data=disc.read(size)
                    if len(data)!=size:raise ValueError('Truncated DOL')
                    return data
            raise ValueError('Unmapped weapon icon source')
        rows=[]
        for address,(size,digest) in BODIES.items():
            data=read(address,size)
            if hashlib.sha256(data).hexdigest()!=digest:raise ValueError('Weapon icon source mismatch')
            if size==0xc0:rows.append({'weapon':address,'icon':struct.unpack_from('>H',data,4)[0]})
    return {'weapons':rows,'jump_icons':[167,168,169],'hammer_icons':[164,165,166],
            'icons':[164,165,166,167,168,169,rows[2]['icon']],
            'bodies':len(BODIES),'scope':'Static original US bindings; no runtime trace'}
if __name__=='__main__':print(source_art())
