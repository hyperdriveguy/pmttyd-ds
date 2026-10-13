#!/usr/bin/env python3
"""Native development checkpoint immediately before the original trio approach."""
from pathlib import Path
import struct
p=Path('artifacts/fixtures/underground-trio-story11.bin');p.parent.mkdir(parents=True,exist_ok=True)
b=bytearray(40);b[:4]=b'PM2D';struct.pack_into('<IffII',b,4,5,-1.5,.7,10,1)
b[25]=1;b[26]=5;struct.pack_into('<i',b,28,11);b[32]=1;b[34]=1
h=2166136261
for v in b[:36]:h=((h^v)*16777619)&0xffffffff
struct.pack_into('<I',b,36,h);p.write_bytes(b);print(p)
