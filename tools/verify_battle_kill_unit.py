#!/usr/bin/env python3
"""Read-only complete US KillUnit/registry/ClearStatus/SetStatus/table/change-info bodies."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
PINS=[(2148610820, 184, '69e29c569b4fa0307f472b97dd92bceb857b92b47d026b60ea7e087d628be02d'), (2148501708, 60, '6a40e068a63873b3f93f3cf0bdc72c59323e60ff5b961ac740b22eddb58276fa'), (2148694872, 84, 'f78ede5ff372b7d96da81e1d7d9e20739b76a0bdf838b2315094082f8af2d8bf'), (2148694956, 904, '30d8aa59d2ae6b40dec269bc06d5e748da33249ad871f033c7bad2f04c604dd5'), (2151039592, 112, '4f30969da62a379e55efa536d3c77c346d5843cfb72093c5b6679b78a98b83e0'), (2149145908, 72, '276ae75f8cfe57227b8810bef14b85a6e5e2b8f0898899d1a0e51b7a8a5c7749')]
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
