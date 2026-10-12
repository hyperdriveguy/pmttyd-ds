#!/usr/bin/env python3
"""Read original US DOL actor collision constants and key instruction sites."""
import struct
from iso_assets import DEFAULT_ISO
def main():
    with open(DEFAULT_ISO,'rb') as iso:
        assert iso.read(6)==b'G8ME01'
        iso.seek(0x420);base=struct.unpack('>I',iso.read(4))[0]
        iso.seek(base);header=iso.read(0x100)
        offsets=struct.unpack_from('>18I',header,0)
        addresses=struct.unpack_from('>18I',header,0x48)
        sizes=struct.unpack_from('>18I',header,0x90)
        def read(address):
            for offset,start,size in zip(offsets,addresses,sizes):
                if start<=address<=start+size-4:
                    iso.seek(base+offset+address-start);data=iso.read(4)
                    assert len(data)==4
                    return data
            raise ValueError(f'Unmapped DOL address {address:x}')
        def value(address,expected):
            result=struct.unpack('>f',read(address))[0]
            assert abs(result-expected)<1e-5,(hex(address),result,expected)
            return result
        width=value(0x80420088,20);height=value(0x804200c0,37)
        half=value(0x804208cc,.5);lower=value(0x804208b4,11)
        upper=value(0x8042092c,26.64)
        walk=value(0x80420008,1);dash=value(0x804200b8,2.25)
        cap=value(0x80420a64,70)
        for address,instruction in [(0x8005b72c,'d02301b8'),(0x8005b734,'d00301bc'),
                                    (0x8005b774,'d02301c0'),(0x8008b88c,'c07f01c0'),
                                    (0x8008a544,'efe006b2'),(0x8008a4ec,'d0210048'),
                                    (0x8008a4f0,'d001004c')]:
            assert read(address)==bytes.fromhex(instruction),hex(address)
        print(f'Original normal collision: width={width},height={height},half-width={width*half},wall rays={lower},{upper}')
        print(f'Native scale .01: clearance={width*half*.01},wall rays={lower*.01:.6f},{upper*.01:.6f}')
        for address,instruction in [(0x8005b6e8,'d0230184'),(0x8005b6f0,'d0030188'),
                                    (0x800877c4,'c3e30188'),(0x80087aa4,'c3e30184'),
                                    (0x80087910,'2c000bd1')]:
            assert read(address)==bytes.fromhex(instruction),hex(address)
        print(f'Ordinary walk/dash per tick={walk},{dash}; source stick cap={cap},dash threshold=55')
        ascent=[value(0x80416110+4*n,v) for n,v in enumerate((8,-.84,.08,-.02))]
        fall=[value(0x80416120+4*n,v) for n,v in enumerate((-.1,-.2,.024,-.004))]
        release=value(0x80420a38,-.526)
        assert read(0x80310250)==bytes.fromhex('8009421c')
        assert read(0x8009b4dc)==bytes.fromhex('907f0048')
        assert read(0x8009b660)==bytes.fromhex('901f0044')
        print(f'Original ordinary jump ascent={ascent},fall={fall},release={release}')
if __name__=='__main__':main()
