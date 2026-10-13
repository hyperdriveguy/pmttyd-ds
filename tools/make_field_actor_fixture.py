#!/usr/bin/env python3
"""Native east-side stationary-NPC harness save; not an original game save."""
import argparse
from pathlib import Path
import struct

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,default=Path('artifacts/fixtures/east-npc-story8.bin'))
    args=parser.parse_args();data=bytearray(40);data[:4]=b'PM2D'
    struct.pack_into('<IffII',data,4,4,1.2,-4.,10,1)
    data[25]=1;data[26]=2;struct.pack_into('<i',data,28,8);data[32]=1
    checksum=2166136261
    for byte in data[:36]:checksum=((checksum^byte)*16777619)&0xffffffff
    struct.pack_into('<I',data,36,checksum)
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_bytes(data)

if __name__=='__main__':main()
