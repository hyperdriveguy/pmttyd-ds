#!/usr/bin/env python3
"""Read-only US DOL voice/unit/PSND source pins. No playback or emulation."""
import argparse
import hashlib
import json
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
PINS = {
    0x80104508: (0xd8, '0b05d46b342e87120a81c08700119270fb77fa41f39f1ca238e37ad43161c538'),
    0x8012619c: (0x5c, '7b8a9ad3919b190d9cf4e26c9a6c1485d6c4f4c60957e64f7419a713a61c8e60'),
    0x80126088: (0x114, '8d02b8c164e67841fc6c732ba7dff6bdc62f7dccb298e079d53ec4dba7b12681'),
    0x80128c7c: (0x1c, 'b86b6dbcd68d6df4e550ce72488eb126842189c1f29b8df5e950221aa41da78c'),
    0x800d98bc: (0x3c, 'eeb7479a31260ba74b0b7cee77816220a8f80919d3ffa05e3e0fb515652a8d73'),
    0x800d9848: (0x3c, '29fdab1a516235c9bfbb11635067a2f53e115e54d41985990fbd5e09b6d1e971'),
    0x800d9934: (0x540, '3a3a312954b07a93a0468dd3edfc5f1d2110d9b92e36a8aff2a9d46caec22e71'),
    0x800d9744: (0x8c, 'd3b349ba74f742d153191b5dde47b824bd43db843ae8a79a1edc49daec2b9ef8'),
}
def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as disc:
        header = disc.read(0x440)
        if header[:8] != b'G8ME01\0\0':
            raise ValueError('Original US revision0 disc required')
        base = struct.unpack_from('>I', header, 0x420)[0]
        disc.seek(base); dol = disc.read(256)
        sections = [tuple(struct.unpack_from('>I', dol, b+4*i)[0]
                          for b in (0, 0x48, 0x90)) for i in range(18)]
        def read(address, size):
            for offset, virtual, length in sections:
                if virtual <= address and address+size <= virtual+length:
                    disc.seek(base+offset+address-virtual); data = disc.read(size)
                    if len(data) != size: raise ValueError('Truncated original DOL')
                    return data
            raise ValueError(f'Unmapped source address {address:08x}')
        def word(address): return struct.unpack('>I', read(address, 4))[0]
        for address, (size, digest) in PINS.items():
            if hashlib.sha256(read(address, size)).hexdigest() != digest:
                raise ValueError(f'Changed complete source body {address:08x}')
        # Actual cold register initializer, not a guessed palette/data base.
        assert read(0x80005420, 8).hex() == '3da0804161adcf20'
        sda = 0x8041cf20
        assert sda+0x1c70 == 0x8041eb90
        assert sda-25680 == 0x80416ad0 and sda-25660 == 0x80416ae4
        assert word(0x801260e0) == 0x3803ffff  # status11:2*S-1
        assert word(0x80126120) == 0x38030001  # status10:2*S+1
        assert read(0x8012615c, 12).hex() == '3c1f0ee728004d8040820014'
        keys = list(struct.unpack('>15I', read(0x80416ad0, 60)))
        names = [read(key, 64).split(b'\0')[0].decode('ascii') for key in keys]
        menu = {}
        for index in range(2520, 2524):
            row = read(0x80316b28+20*index, 20)
            menu[str(index)] = {'bytes':row.hex(), 'descriptor':struct.unpack_from('>I',row,4)[0]}
        assert menu['2522']['descriptor'] == 0
        assert word(0x804218cc) == 0x447a0000  # actual 1000.0f
    return {'whole_bodies':len(PINS), 'voice_table_addresses':['80416ad0','80416ae4'],
            'voice_keys':keys, 'voice_names':names, 'menu_rows':menu,
            'volume_rule':'(raw+0x0ee70000) mod2^32 == 0x4d80 only; else low8'}
if __name__ == '__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--iso',default=DEFAULT_ISO)
    print(json.dumps(verify(parser.parse_args().iso),sort_keys=True))
