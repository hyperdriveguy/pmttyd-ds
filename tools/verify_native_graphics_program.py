#!/usr/bin/env python3
"""Pin the actual installed native SDK used for shader-lifetime evidence."""
import argparse
import hashlib
import json
from pathlib import Path
EXPECTED_C3D='3f34eff859c1f1e60ebe597b6df9cf5e5e74563d9e64ca01a7b3292fde63016a'
EXPECTED_C2D='f6c5352a838639c486b806b8dc847c74a1a190d98b09eb469d5c30a02ae6542f'
def verify(archive='/opt/devkitpro/libctru/lib/libcitro3d.a',
           c2d_archive='/opt/devkitpro/libctru/lib/libcitro2d.a'):
    actual=hashlib.sha256(Path(archive).read_bytes()).hexdigest()
    if actual!=EXPECTED_C3D:
        raise ValueError('Native SDK archive changed; re-audit Init/Fini/BindProgram cache and C2D teardown')
    actual_c2d=hashlib.sha256(Path(c2d_archive).read_bytes()).hexdigest()
    if actual_c2d!=EXPECTED_C2D:
        raise ValueError('Native C2D archive changed; re-audit Fini shader/cache cleanup ordering')
    return dict(citro3d_archive=archive,sha256=actual,
                citro2d_archive=c2d_archive,citro2d_sha256=actual_c2d,
                evidence='Actual ARM disassembly of C3D_Init/Fini/BindProgram and C2D_Fini in isolated native probe',
                limitation='Installed SDK identity pin, not original GameCube source or GPU behavior proof')
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--archive',default='/opt/devkitpro/libctru/lib/libcitro3d.a')
    p.add_argument('--c2d-archive',default='/opt/devkitpro/libctru/lib/libcitro2d.a')
    args=p.parse_args()
    print(json.dumps(verify(args.archive,args.c2d_archive),indent=2))
