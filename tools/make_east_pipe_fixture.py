#!/usr/bin/env python3
"""Create an isolated native development checkpoint on the original east pipe."""
from pathlib import Path
import struct
p=Path('artifacts/fixtures/east-pipe-story11.bin');p.parent.mkdir(parents=True,exist_ok=True)
b=bytearray(40);b[:4]=b'PM2D';struct.pack_into('<IffII',b,4,5,-1.9,3.291645,10,1)
b[25]=1;b[26]=2;b[27]=6;struct.pack_into('<i',b,28,11);b[32]=1
h=2166136261
for v in b[:36]:h=((h^v)*16777619)&0xffffffff
struct.pack_into('<I',b,36,h);p.write_bytes(b)
print(p)
