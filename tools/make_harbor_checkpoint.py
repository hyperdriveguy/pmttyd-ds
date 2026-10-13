#!/usr/bin/env python3
"""Create a native development story3 checkpoint, not an original GameCube save.

This starts at the documented position gate on validated dry harbor ground.
It bypasses the unimplemented arrival intro for scene testing only.
"""
import argparse
from pathlib import Path
import struct


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',default='artifacts/fixtures/harbor-story3.bin')
    args=parser.parse_args();b=bytearray(40);b[:4]=b'PM2D'
    struct.pack_into('<IffII',b,4,3,-1.5,-1.1,10,1)
    b[25]=1;b[26]=1;b[27]=2;struct.pack_into('<i',b,28,3);b[32]=1
    checksum=2166136261
    for byte in b[:36]:checksum=((checksum^byte)*16777619)&0xffffffff
    struct.pack_into('<I',b,36,checksum)
    output=Path(args.output);output.parent.mkdir(parents=True,exist_ok=True);output.write_bytes(b)


if __name__=='__main__':main()
