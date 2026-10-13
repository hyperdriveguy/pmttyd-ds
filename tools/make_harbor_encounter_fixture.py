#!/usr/bin/env python3
"""Native story3/story4 test checkpoints; not original GameCube saves."""
import struct,argparse
from pathlib import Path
parser=argparse.ArgumentParser();parser.add_argument('--resume',action='store_true');args=parser.parse_args();story=4 if args.resume else 3
b=bytearray(40);b[:4]=b'PM2D';struct.pack_into('<IffII',b,4,4,0,-1,2,1);b[25]=1;b[26]=1;b[27]=2
struct.pack_into('<i',b,28,story);b[32]=1
h=2166136261
for v in b[:36]:h=((h^v)*16777619)&0xffffffff
struct.pack_into('<I',b,36,h)
out=Path(f'artifacts/fixtures/harbor-story{story}-entry.bin');out.parent.mkdir(parents=True,exist_ok=True);out.write_bytes(b);print(out)
