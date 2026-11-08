#!/usr/bin/env python3
"""Read-only source proof for the Harbor no-LifeShroom lethal admission."""
import argparse
import hashlib
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_battle_damage_orchestration import verify as verify_orchestration
SPANS = [('PouchCheckItem', '0x800d4ed4', '0x1f8', '42deab825c1637d84ab4ad27431a9571fbb92f349716b88a6eba7b736779ba57'), ('DamageDirect', '0x800fdfd4', '0xb24', 'cb580f8e21cc818b6e13f8a7f9850aa72fa7e58a074411daa57360936faec3ec'), ('EmergencyRevival', '0x800fba1c', '0xc0', '059b63d23b5a6bc7839ca43f56d5576f694c07da2793749727378723a7fa309c'), ('DisableRestore', '0x801141a4', '0x18', 'c53306e74d05f1c3a07d8a661f999fcc4b8590126d5373453c2923b2edbaaba5'), ('GetUnitKind', '0x8010ed8c', '0x6c', '59bd1076a82f59d6b08b1c589964054f5b176097a8113c00268f9b83df93fe9b'), ('OnOffStatus', '0x80105e50', '0xbc', '2b21c47a04d66d4726084d9666b11d4eb2232c0443f2c6cce23ff8a42d368fe8'), ('ClearAllStatus', '0x80105df8', '0x58', '5c5fdae1c51fedaeac3a55eb9dca65bf358babffcbb466cfc6c1d6ab5354dfb4'), ('SetStatus', '0x80127bac', '0x388', '30d8aa59d2ae6b40dec269bc06d5e748da33249ad871f033c7bad2f04c604dd5'), ('ClearStatus', '0x80127b58', '0x54', 'f78ede5ff372b7d96da81e1d7d9e20739b76a0bdf838b2315094082f8af2d8bf'), ('SetStatusTable', '0x80364268', '0x70', '4f30969da62a379e55efa536d3c77c346d5843cfb72093c5b6679b78a98b83e0')]

def verify(iso=DEFAULT_ISO):
    verify_orchestration(iso)
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
        table=struct.unpack('>28I',read(0x80364268,28*4))
        if table[27]!=0x80127f1c or read(table[27],4)!=bytes.fromhex('98a70135'):
            raise ValueError('Original SetStatus27 branch changed')
    return dict(source='US G8ME01 revision0',spans=len(SPANS),
        scope='Whole pouch helper, finite native admission and source death leaves; not battle-result completion',
        original_executed=False)

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--iso',default=DEFAULT_ISO)
    print(verify(p.parse_args().iso))
