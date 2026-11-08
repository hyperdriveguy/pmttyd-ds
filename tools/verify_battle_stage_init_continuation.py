#!/usr/bin/env python3
"""Read-only whole stage/object/spotlight initialization source proof."""
import argparse
import hashlib
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
SPANS = [('BattleStageInit', '0x80139780', '0xe4', 'c692d3c176e90df5800a44f8416eb8f1ad5772a59318f60756544db5af0e6bf2'), ('BattleStageObjectInit', '0x80149bd0', '0x8d4', 'e22954ec0bac86bf110c6767fa80cd0298a31d3ff74a5b453ab2d8ee7365e53d'), ('batSpotInit', '0x80138068', '0x138', '83c505917c70536a9cf15978489fae9b16d50e4283ca1102567720df79e9c347'), ('batSpotEntry', '0x80137a88', '0x94', '37526fcda4e6a9438e9a4f826d293522f454888bc04ee712bf5e3cf0f390bcbd'), ('BattleStageGetPtr', '0x80139864', '0x10', 'a57bcf41694fc253d3277e8b7a18da97f062317ed8cbd4a78305fae4fad005de'), ('pouchGetPtr', '0x800d5eb0', '0x8', 'dbcd2f415efe4aa4e5684ee546cf73d97cc641b5a0290f1b46b14ab606786537'), ('mapGetWork', '0x8002b438', '0x14', 'ddbc7aaeeb36ae24182217ef14c98d9a241a088126b1f39c314207b07ca13abc'), ('ObjectTemplatesAndNames', '0x802f42b8', '0x290', '3f3692e5584e1eae942f620bd923b8f57acc871dfda85759ea57474171f45af5'), ('SpotTemplatesAndBias', '0x802f37a8', '0x80', '50e237e3c80194310f059e2e405c0ef3507085277a4ba61d176212ed8d5eea2c'), ('SpotWorkDataPointer', '0x804175a0', '0x4', 'ad079f4e273969b653c6e37c443b1e7861d5af9420826dca39a589e284556304'), ('ObjectBuiltinsAndUnitConfigs', '0x80417ca0', '0xa8', '842935acb9cc3f57615aaead18e555f1634c661dce714709221b717e613184ad'), ('ObjectNames', '0x80423028', '0x18', '3d8da69b280677f4bb5b71a29d7db32d1ceb31891fc52c65d48861858d08a03d'), ('ObjectZero', '0x80422fd0', '0x4', 'df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119'), ('SpotColorDefault', '0x80422ad4', '0x4', 'ad95131bc0b799c0b1af477fb14fcf26a6a9f76079e48bf090acb7e8367bfd0e'), ('SpotPositionScalars', '0x80422b18', '0x8', 'e5ec4e01e6779af5429cb932e7db8606c4fae0c8c75d3c9deb2e1185824bac31'), ('StageFloorScalars', '0x80422b3c', '0x10', 'b51c7ffe7a4870f741b2fc8769cc2732b38e29ff5445886b26c4ab02794ec3df')]

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
    return dict(source='US G8ME01 revision0',spans=len(SPANS),
        scope='Whole stage/object/spotlight parents and source data; external map/heap/event owners remain explicit',
        original_executed=False)

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--iso',default=DEFAULT_ISO)
    print(verify(p.parse_args().iso))
