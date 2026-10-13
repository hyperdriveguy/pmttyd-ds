#!/usr/bin/env python3
"""Check original US startup source keys and actual nonnull effNiceAsync row."""
import argparse,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
from battle_startup_assets import RESOURCES

def verify(iso):
    with Path(iso).open('rb') as source:
        if source.read(8)!=b'G8ME01\0\0':raise ValueError('US disc0 revision0 required')
        source.seek(0x420);dol=struct.unpack('>I',source.read(4))[0];source.seek(dol);header=source.read(0x100)
        def read(address,size):
            for i in range(18):
                offset,base,length=(struct.unpack_from('>I',header,a+4*i)[0] for a in [0,0x48,0x90])
                if base<=address and address+size<=base+length:
                    source.seek(dol+offset+address-base);raw=source.read(size)
                    if len(raw)!=size:raise ValueError('Truncated source DOL')
                    return raw
            raise ValueError('Source DOL address outside image')
        for name,key,_ in RESOURCES:
            if read(key,len(name)+1)!=name.encode()+b'\0':raise ValueError('Original resource key changed')
        row=struct.unpack('>7I',read(0x80417a6c,28))
        expected=[r[1] for r in RESOURCES[3:8]]+[0,RESOURCES[8][1]]
        if list(row)!=expected:raise ValueError('Original US nice/lucky row changed')
    return 10

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);a=p.parse_args();print(f'Original9 resource strings and US row1 withNULL slot verified ({verify(a.iso)} anchors)')
if __name__=='__main__':main()
