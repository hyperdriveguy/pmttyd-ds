#!/usr/bin/env python3
"""Native east-side stationary-NPC harness save; not an original game save."""
import argparse
from pathlib import Path
import struct

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--actor',choices=['mouser','darkly','goomba','goomba-observe'],default='mouser')
    parser.add_argument('--output',type=Path)
    args=parser.parse_args();args.output=args.output or Path('artifacts/fixtures/'+('east-npc-story8.bin' if args.actor=='mouser' else args.actor+'-story8.bin'));data=bytearray(40);data[:4]=b'PM2D'
    x,z={'mouser':(1.2,-4.),'darkly':(-3.35,-3.65),'goomba':(-3.4,1.3),'goomba-observe':(-2.8,1.3)}[args.actor]
    struct.pack_into('<IffII',data,4,4,x,z,10,1)
    data[25]=1;data[26]=2;struct.pack_into('<i',data,28,8);data[32]=1
    checksum=2166136261
    for byte in data[:36]:checksum=((checksum^byte)*16777619)&0xffffffff
    struct.pack_into('<I',data,36,checksum)
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_bytes(data)

if __name__=='__main__':main()
