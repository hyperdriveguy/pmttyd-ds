#!/usr/bin/env python3
"""Read-only complete US ending parent/ordinary used helpers, jump table and exact constants bodies."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
PINS=[(2149667380, 5232, 'ea77420a93d18acc970cb2ad2402cea39b6fa7b3e72953ebc9bf5759113e0cb0'), (2151353480, 128, '507b55a359872ec1869869fdc284b0cfb39f5a04210b83a9682a3193cc90c665'), (2148501416, 108, '26840147b00e408e2cd371d8c23ddd7057de2da195881a7ab014b758fa28f869'), (2148501288, 128, '618380e524d9f6805cd215b283aa4450e7e8fb392921ef72a3bec5b00cd8dd37'), (2148693492, 76, '7f3660c61d6b54fe6bafa2ae1dcf41b9712db24699ac333eda5148c485b9e428'), (2148693568, 72, 'ae16df77ea551f2b88055d07ace56792093c85ddf5a0e12a3c158f0a33fefab0'), (2151838360, 8, '5764f6879eda4d185c5ef5c0fcbc58603e97fb5bb58c5a557f3d02e24af380ce'), (2151838020, 4, 'e3820096cb82366b860b8a4e668453a7aaaf423af03bdf289fa308ea03a79332'), (2150623996, 48, '5ac68cdc89606b92e5a65391ab358030065b56ba945ade6ccafd5a37d0e5ce2f'), (2147552660, 44, '62d8e88e6c6ebb205a2b428a54a443bb13161d1ee3a84adeb7ba1816f1593557'), (2147557464, 4552, '369db5633483e4b397234036dae4080d6e8849d68ea747daec990b41fd891437')]
def verify(iso):
 with Path(iso).open('rb') as disc:
  if disc.read(8)!=b'G8ME01\0\0':raise ValueError('US disc0 revision0 required')
  disc.seek(0x420);dol,=struct.unpack('>I',disc.read(4));disc.seek(dol);header=disc.read(256)
  sections=[tuple(struct.unpack_from('>I',header,k+4*i)[0]for k in(0,0x48,0x90))for i in range(18)]
  for address,size,digest in PINS:
   offsets=[dol+o+address-b for o,b,n in sections if b<=address and address+size<=b+n]
   if len(offsets)!=1:raise ValueError('Source mapping differs')
   disc.seek(offsets[0]);data=disc.read(size)
   if len(data)!=size or hashlib.sha256(data).hexdigest()!=digest:raise ValueError('Original source body differs: '+hex(address))
 return len(PINS)
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);a=p.parse_args();print('Verified',verify(a.iso),'original complete bodies/data blocks')
