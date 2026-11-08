#!/usr/bin/env python3
"""Read-only whole source rotation and camera death-event proof."""
import argparse
import hashlib
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
SPANS = [('AddRotateEvent', '0x801115f8', '0x128', '9aea1e34433438c33b235a3c2c25fe6e0c3e97ce688cbada9f2e16e83163d0c5'), ('AddRotate', '0x801287c4', '0xd4', 'd1e9bfc4d83d1e416d5eb84f2ce432ea884c60f0d97650037ab2a6e9a0b9d82a'), ('SetRotateEvent', '0x80111720', '0x174', '68daca6fec9f4c5323f49256fe0c38b7d122ec9a0f17c9863a5ffcc5963b2a6c'), ('GetRotate', '0x80128898', '0x1c', '71136423aae4230cab821c5e865d9be322fbe18dbe543a318cbac1d43ffe7797'), ('SetRotate', '0x801288b4', '0xb4', 'cb3cea9b444a00f4852fd04a9dbd87a196e21e03d08ac39b0896ae39d9d9e0db'), ('ReviseAngle', '0x80007328', '0x60', 'e44664bf511be1e4b10e4f0b3e40004fbd1d412c40ce6b34fee14987db320ed5'), ('PriorityEvent', '0x8012c0ac', '0x30', '050f4e0222491c31d3aff26b317d8701583742734efce7a8579218cafb125ca6'), ('Priority', '0x8012c0dc', '0xc', '1cc63b9f87a07e666fbd2ac95b1cd90bfe40f38d847da511666918ee3a8c9d28'), ('NoMoveY', '0x8012b254', '0x58', '3ef451dc4e11ac3f9e064fd17b5734cbe41d25aa4bccad7a26b5bfb52f5400df'), ('Offset', '0x8012b108', '0x90', 'fadcfe2412982c2fb8dfdd44ab3a47fd4fd5cd3b3d9a07ed582d573ad8659d64'), ('AngleZero', '0x8041f448', '0x4', 'df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119'), ('AngleTurn', '0x8041f45c', '0x4', '1aeadc6fe40c6e83ce89ce6d57ec3cd9f1a30d2f545c978b504293509dc90880'), ('DoubleTurn', '0x802bf240', '0x8', 'fd8adff6e8f476c5527d46ac19f9e959e9b0b177b4fe577c44343af59b06dd29'), ('SignedBias', '0x802ee608', '0x8', '07af8387624cf8f36152797d2f561246f21237d1d22b5c13006eacfa6f475e43'), ('UnitZero', '0x804226a4', '0x4', 'df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119'), ('UnitTurn', '0x804226c8', '0x4', '1aeadc6fe40c6e83ce89ce6d57ec3cd9f1a30d2f545c978b504293509dc90880'), ('R2Setup', '0x80005418', '0x8', '033e7f738116a4d6fafbb4b98f1105157480bc452dae8682ad0319f53e26b1b2')]

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
        scope='Whole rotation/camera leaves; host SDK numeric policy, no GPU/battle-result completion',
        original_executed=False)

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--iso',default=DEFAULT_ISO)
    print(verify(p.parse_args().iso))
