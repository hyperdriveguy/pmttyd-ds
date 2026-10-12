"""Compile original DOL shadow arrays into an ignored local native pack."""
import hashlib
import struct
from verify_overworld_mario_air_shadow import geometry,dol_bytes


def compile_shadow(iso,output):
    original=geometry(iso)
    with open(output,'wb') as file:
        file.write(struct.pack('<8sII',b'PMSHDW1\0',len(original['vertices']),len(original['lists'])))
        for v in original['vertices']:file.write(struct.pack('<3f',*v))
        for palette in ['ordinary','no_decay']:
            for color in original['palettes'][palette]:file.write(bytes(color))
        for primitive in original['lists']:
            file.write(struct.pack('<16sII',primitive['name'].encode(),primitive['primitive'],len(primitive['pairs'])))
            for pair in primitive['pairs']:file.write(bytes(pair))
    addresses=[(0x80304ce0,720),(0x80415140,12),(0x80415160,12)]
    addresses.extend((p['address'],64) for p in original['lists'])
    return {'source':'US G8ME01 revision0 DOL','pack':str(output),'vertices':60,'lists':5,
            'spans':[{'address':hex(a),'bytes':n,'sha256':hashlib.sha256(dol_bytes(iso,a,n)).hexdigest()} for a,n in addresses],
            'conversion':'Original float words/endpoints and indexed position/color order retained; little-endian native pack'}
