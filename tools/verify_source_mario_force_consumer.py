#!/usr/bin/env python3
"""Read-only US revision-zero force consumer/coordinate leaf source pins."""
import hashlib
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_overworld_mario_air_shadow import dol_bytes
PINS = [(2148097892, 148, 'b5add877be7b3b1099c7c78fc718202df7d9b6cd77ef99bec99cd7bf34f483ad'), (2147507684, 168, '65be0808ab7633fd49dc50adf2f78846be4d70934676cca3e52337fe8deff2c5'), (2147868864, 8, '3a59459f28ddd5d287c200434ad158a9dfe277f4f6bcaae3eb8698505aa9773d'), (2147849692, 32, 'c2a51a0928153042d9a2f29579f0172171726001017c035f5b353ee2b9694ec0'), (2147856564, 52, 'db1b1a38375047f09978ad981f63c502ae48ef034de5f8ad305a607678d1bfbc'), (2151811680, 4, 'df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119'), (2151811700, 4, 'de620abd6d3615746360c1d15ce3a56291236d37424124054a49d25e947ffc4d'), (2151806040, 8, '36f9e7e3f395c6626372873c98576ee66a93e4807289cd68e45c853ce608932d')]

def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as source:
        if source.read(8) != b'G8ME01\0\0':
            raise ValueError('US revision zero required')
    for address, size, expected in PINS:
        if hashlib.sha256(dol_bytes(iso,address,size)).hexdigest() != expected:
            raise ValueError(f'Original force body/data changed: {address:08x}')
    print('Whole ForceMoveMain/movePos, ordinary branch/caller order, pointer and actual constants pinned')

if __name__ == '__main__':
    verify()
