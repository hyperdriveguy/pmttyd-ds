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
        padding=value(0x80420964,.01);above=value(0x804208c0,5)
        footprint=value(0x80420978,.375)*width
        for address,instruction in [(0x8008e798,'ee52102a'),(0x8008e7a4,'ee82082a'),
                                    (0x8008e8a4,'ef75a02a'),(0x8008e8ac,'ef54902a'),
                                    (0x8008f044,'2c1a0005')]:
            assert read(address)==bytes.fromhex(instruction),hex(address)
        depth=value(0x804208ec,.1)
        value(0x804208fc,.265);value(0x80420928,30)
        assert read(0x803100c0)==bytes.fromhex('8009b158')
        for address,instruction in [(0x8008fa00,'c0029474'),(0x8008fa10,'c0029480'),
                                    (0x8008fa24,'c00294e8'),(0x8009b1a0,'38000003'),
                                    (0x8009b1dc,'ec020024'),(0x8009b204,'d01f0090')]:
            assert read(address)==bytes.fromhex(instruction),hex(address)
        value(0x80420954,50);value(0x80420950,75)
        for address,instruction in [(0x8008eb60,'c2429514'),(0x8008ed34,'fc019040'),
                                    (0x8008ed40,'3a800000')]:
            assert read(address)==bytes.fromhex(instruction),hex(address)
        value(0x804208f0,2);value(0x804208b4,11)
        for address,instruction in [(0x8008ef24,'fc02a840'),(0x8008ef30,'c00294b0'),
                                    (0x8008ef44,'fc009a10'),(0x8008ef4c,'4081002c'),
                                    (0x8008efd4,'c0029474'),(0x8008efe8,'40820058')]:
            assert read(address)==bytes.fromhex(instruction),hex(address)
        for address,instruction in [(0x8008f114,'c02294ac'),(0x8008f11c,'c05f007c'),
                                    (0x8008e890,'c2bd0090'),(0x8008f160,'408000d4'),
                                    (0x8008f164,'c00294cc')]:
            assert read(address)==bytes.fromhex(instruction),hex(address)
        assert read(0x8008dca8)==bytes.fromhex('fc20f890')
        assert read(0x8008dcb0)==bytes.fromhex('fc40f890')
        assert read(0x80093bd0)==bytes.fromhex('4bffa0a1')
        print(f'Falling marioChkLandon: above=abs(v)+{above},below=abs(v)+{padding}; separate marioChkGnd2 depth={depth},acceptance -10<=root-floor<0')
        precision=value(0x80420900,100);rootPrecision=value(0x80420904,10)
        clearance=value(0x804208c4,1);fastFall=value(0x8042090c,-10)
        for address,instruction in [(0x8008c284,'ec001028'),(0x8008c678,'ec23d03a'),
                                    (0x8008c6a0,'ec411824'),(0x8008c6d0,'ec01b82a'),
                                    (0x8008cbb0,'ec23007a'),(0x8008cbd8,'ec010028'),
                                    (0x800161c4,'28000001'),(0x80016210,'418202e8')]:
            assert read(address)==bytes.fromhex(instruction),hex(address)
        print(f'Normal head: radius={(width-half)*half},5 probes,contact precision={precision},root precision={rootPrecision},clearance={clearance},fast-fall cutoff={fastFall}')
        for address,instruction in [(0x8001684c,'562007fe'),(0x80016890,'7d254b78'),
                                    (0x800168a8,'7c060378'),(0x8001699c,'48288209'),
                                    (0x800169ac,'482881f9'),(0x800169cc,'482882d9')]:
            assert read(address)==bytes.fromhex(instruction),hex(address)
        for address,instruction in [(0x8005affc,'48038d3d'),(0x8005b000,'48031905'),
                                    (0x8005b004,'4bffea75'),(0x8005b034,'480340b1')]:
            assert read(address)==bytes.fromhex(instruction),hex(address)
        assert read(0x8005b04c)==bytes.fromhex('4bffea2d')
        assert read(0x8005b074)==bytes.fromhex('48031891')
        assert read(0x8005b078)==bytes.fromhex('48038899')
        for address,instruction in [(0x8008ca4c,'c02294c4'),(0x8008cbe4,'5460039d'),
                                    (0x8008cbe8,'408200b4'),(0x8008cce0,'4800744d'),
                                    (0x80093b54,'c0430084'),(0x80093bd0,'4bffa0a1')]:
            assert read(address)==bytes.fromhex(instruction),hex(address)
        print('Ordinary ascent: jump, overhead, move; normal falling: move before fall gravity/search')
        threshold=value(0x80420aa8,-2000);respawnOffset=value(0x80420aa4,37)
        probe=value(0x804209c0,300);rise=value(0x804209c4,10);travel=value(0x804209c8,8)
        assert read(0x80091cf4)==bytes.fromhex('38600032')
        assert read(0x80091d40)==bytes.fromhex('3800000c')
        print(f'Void threshold={threshold},respawn offset={respawnOffset},screen probe={probe},rise step={rise},travel={travel},landing wait50,injury wait12')
if __name__=='__main__':main()
