#!/usr/bin/env python3
"""Native Rogueport NPC harness save; not an original game save."""
import argparse
from pathlib import Path
import struct

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--actor',choices=['mouser','darkly','goomba','goomba-observe','goomba3'],default='mouser')
    parser.add_argument('--output',type=Path)
    parser.add_argument('--quest',type=int,help='Explicit known original GSW1420 byte; omitted means unknown')
    parser.add_argument('--flag5405',type=int,choices=[0,1])
    parser.add_argument('--flag5406',type=int,choices=[0,1])
    args=parser.parse_args();args.output=args.output or Path('artifacts/fixtures/'+('east-npc-story8.bin' if args.actor=='mouser' else args.actor+'-story8.bin'));data=bytearray(40);data[:4]=b'PM2D'
    if args.quest is not None and not 0<=args.quest<=255:parser.error('--quest must be0..255')
    x,z={'mouser':(1.2,-4.),'darkly':(-3.35,-3.65),'goomba':(-3.4,1.3),'goomba-observe':(-2.8,1.3),'goomba3':(-4.,-3.55)}[args.actor]
    struct.pack_into('<IffII',data,4,4,x,z,10,1)
    data[25]=1;data[26]=0 if args.actor=='goomba3' else 2;struct.pack_into('<i',data,28,8);data[32]=1
    if args.quest is not None or args.flag5405 is not None or args.flag5406 is not None:
        struct.pack_into('<I',data,4,7)
        packed=1 | ((args.quest or 0)<<8) | (int(args.quest is not None)<<16)
        packed|=int(args.flag5405 is not None)<<17 | int(args.flag5406 is not None)<<18
        packed|=(args.flag5405 or 0)<<19 | (args.flag5406 or 0)<<20
        struct.pack_into('<I',data,20,packed)
    checksum=2166136261
    for byte in data[:36]:checksum=((checksum^byte)*16777619)&0xffffffff
    struct.pack_into('<I',data,36,checksum)
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_bytes(data)

if __name__=='__main__':main()
