#!/usr/bin/env python3
"""Read-only whole original SetDir/direction/frame bodies and actual literals."""
import hashlib
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_overworld_mario_air_shadow import dol_bytes
PINS = [(2148429288, 480, 'b9a9fa1cd299647ac3a8f3d1b17439dd10adc2bea28df01eeea522dc551ecc1a'), (2148099428, 52, '817781931eeb9c0fa55f1e7bdda521295e5f8c673bee13e2df988470851d6dcf'), (2148098552, 204, '7d534afba567b74c22c9a366cd0c9a5a1215fcdd0c8834f247eab920dfed5fd3'), (2147506636, 40, '8d1cc4322ce1bf1ed85e8e98f84112ac9bc645e0573105a01316348e01ad59ff'), (2147868864, 8, '3a59459f28ddd5d287c200434ad158a9dfe277f4f6bcaae3eb8698505aa9773d'), (2151811680, 12, '65299870f1c66c16b64fac13c9d80c8497ad735f3859d2ba037b21cd82638808'), (2151811720, 16, 'e9bb5813589daa1c6a15c5e0b116886fce353a0de6111cf8f4e05184a2f93b8f'), (2151815964, 4, '970a5a0f110b1f900dca6ea98eacfd89c1aaafab978b9706c11562fabf06d737'), (2151816004, 4, '1aeadc6fe40c6e83ce89ce6d57ec3cd9f1a30d2f545c978b504293509dc90880'), (2151815900, 4, 'df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119'), (2150383296, 8, '07af8387624cf8f36152797d2f561246f21237d1d22b5c13006eacfa6f475e43'), (2150514360, 8, '07af8387624cf8f36152797d2f561246f21237d1d22b5c13006eacfa6f475e43')]

def verify(iso=DEFAULT_ISO):
    with Path(iso).open('rb') as source:
        if source.read(8) != b'G8ME01\0\0':
            raise ValueError('US revision zero required')
    for address, size, expected in PINS:
        if hashlib.sha256(dol_bytes(iso,address,size)).hexdigest() != expected:
            raise ValueError(f'Original SetDir body/data changed: {address:08x}')
    print('Whole original SetDir/Revise360/ToMovedir/sysMsec2Frame and actual constants pinned')

if __name__ == '__main__':
    verify()
