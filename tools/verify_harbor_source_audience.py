#!/usr/bin/env python3
"""Read-only source identities for the native audience cold-child binding."""
import argparse
import hashlib
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
SPANS = [('tplRead', '0x801a86ac', '0x44', 'c92a924a1d3830b6bceba1544f835bbbc2363cdec2a9df5441aa3394d847b803'), ('GuestTPLRead', '0x801a6050', '0x74', 'b34d46c4ca66a2c36c02eec6b7c57c5ca0c03f18b0528c728616353894b710c2'), ('ThrowMaximum', '0x801a5a0c', '0xa0', 'dfebbb471c2cb5e209ba0a37dcf78381ceea2264b16a70c279e060106d0a48dc'), ('Volume', '0x8019fe28', '0x4c', '5ed4ebd79198572ac18db0fe0d2a74dfcc97fb8af766a849b1a3871c3638a464'), ('NumToTarget', '0x801a1c60', '0x7c', '30e68a9f7d1c3c49a6acbe8d03aa2cb19ad25523b7aaad25accb4509ff6b7bd3'), ('AudienceCount', '0x801a1bf4', '0x6c', '54463e6c2e7e37a46d39821e8a8ff45a8aadc24090dcab56b36f484c3efeeff0'), ('AddAudienceNumber', '0x801a1708', '0x1c4', 'c6aa52701b30e41fadc4b876810b1a10df925d35526660986e519b884f0d3234'), ('AudienceBase', '0x801a86a0', '0xc', '17096f3ef21a7a0336e1d70a08dc681fa1d734a641d0eef0da749a779d749f79'), ('ActorPointer', '0x801a8664', '0x3c', 'b9357ba56748bd7cca833c66858c0d7c8552b2f84196298ab4e054eefcc73d56'), ('ActorExists', '0x801a29c0', '0x64', '21d4c42ea7469e96002e8a917d4e5ed2736c391d5dcbed4200a0e2ebc445e95d'), ('PouchPointer', '0x800d5eb0', '0x8', 'dbcd2f415efe4aa4e5684ee546cf73d97cc641b5a0290f1b46b14ab606786537'), ('PouchAudience', '0x800d3bac', '0xc', '2295cb31b0022a43c846eeabe45ab19049fdfafcd9f0535fed9a05759f362c93'), ('DvdRoot', '0x80007388', '0x8', '20e17ad249044e215aba5f071125d98aee73da26094bbb4ff323cfcdf3574f7d'), ('GuestFormat', '0x802fa6fc', '0x14', '78a8642d35cb72dc53e9bfb564fb39004bff3d08969fb1e6c855c8afaa6dad08'), ('FileFormat', '0x80424a88', '0x6', 'bf97e0e05308934b324e1b07297aec3c0cd6423569b7e85b07c040ae96fd56e0'), ('ResourceNames', '0x802fa710', '0x38', '1a5d94781be8cee3eb0c097d54d1f63289ea97243755d3baa22c78ecc104b00d'), ('OriginalDvdRoot', '0x8041f494', '0x2', '075d3ddf5a3a826e13a92288e853bc4b2cb17fb05367ae865f401a4bb11f05ce'), ('StartupR2R13', '0x80005418', '0x10', '76a62b3dba4a365727ca04077b196ca71058a5e7c9fd484abfe1ad0eeb44bc32')]

def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as f:
        if f.read(8)!=b'G8ME01\0\0':
            raise ValueError('US revision0 disc required')
        f.seek(0x420)
        dol,=struct.unpack('>I',f.read(4))
        f.seek(dol)
        h=f.read(256)
        def read(address,size):
            for i in range(18):
                off,base,n=(struct.unpack_from('>I',h,k+4*i)[0] for k in (0,0x48,0x90))
                if base<=address and address+size<=base+n:
                    f.seek(dol+off+address-base)
                    data=f.read(size)
                    if len(data)!=size:
                        raise ValueError('Truncated original source')
                    return data
            raise ValueError('Unmapped original source')
        for name,address,size,digest in SPANS:
            if hashlib.sha256(read(int(address,16),int(size,16))).hexdigest()!=digest:
                raise ValueError('Original source changed: '+name)
    return dict(source='US G8ME01 revision0', spans=len(SPANS),
        scope='Whole source helpers; AddAudienceNumber only prefix implemented and explicit retained suffix',
        original_executed=False)

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--iso',default=DEFAULT_ISO)
    print(verify(p.parse_args().iso))
