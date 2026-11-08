#!/usr/bin/env python3
"""Pin original ordinary body/no-shadow consumers. Static bytes only, no execution."""
import argparse,hashlib,struct
from pathlib import Path
PINS=[(2148543580, 24, 'a46af3497382dd05448d3c215c6b3efc3164e404e04ebf6c35186bbb6366bb36'), (2148543792, 12, '841adc169a62d73bdd22bf780ca808386cb157fa95c122aa8ded79c84320028c'), (2148699288, 64, '21ed3890883c9f9c73389ff655cd579de1edc7fd1aabc7dce0101ecc93ff811e')]
def main():
 parser=argparse.ArgumentParser(description=__doc__)
 parser.add_argument('--iso',default='/home/carson/games/roms/G8ME01.iso')
 args=parser.parse_args()
 with Path(args.iso).open('rb') as source:
  assert source.read(6)==b'G8ME01'
  source.seek(0x420);dol=struct.unpack('>I',source.read(4))[0]
  source.seek(dol);header=source.read(256)
  for address,length,digest in PINS:
   offset=None
   for index in range(18):
    file=struct.unpack_from('>I',header,index*4)[0]
    ram=struct.unpack_from('>I',header,0x48+index*4)[0]
    size=struct.unpack_from('>I',header,0x90+index*4)[0]
    if ram<=address and address+length<=ram+size:offset=dol+file+address-ram;break
   assert offset is not None
   source.seek(offset);assert hashlib.sha256(source.read(length)).hexdigest()==digest
 print('3 original body-display instruction spans match; no original execution claim')
if __name__=='__main__':main()
