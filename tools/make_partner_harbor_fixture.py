#!/usr/bin/env python3
"""Create an isolated native harbor ledge harness save, not an original save."""
import argparse
from pathlib import Path
import struct


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,default=Path('artifacts/fixtures/partner-harbor-ledge.bin'))
    args=parser.parse_args()
    b=bytearray(40);b[:4]=b'PM2D'
    struct.pack_into('<IffII',b,4,3,1.,-5.1,7,1)
    b[25]=1;b[26]=1;b[27]=12
    struct.pack_into('<i',b,28,11);b[32]=1
    checksum=2166136261
    for value in b[:36]:checksum=((checksum^value)*16777619)&0xffffffff
    struct.pack_into('<I',b,36,checksum)
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_bytes(b)


if __name__=='__main__':main()
