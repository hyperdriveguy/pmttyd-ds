#!/usr/bin/env python3
"""Check original US camera/facing/precheck spans for native tutorial binding."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
SPANS=[(0x8012bfd0,0x58,'727c3dd79c607ef888e3d934e7d06e8a2456c9ebb8e00a58ba5440e8fe8884db'),(0x8012c028,0x84,'4b894e0aee993e8e3f8d099f3d3db18fe0d4cb49987d8c073f3ab684ce3c8c5f'),(0x8012be38,0x198,'dcc7d6a13fac97533484ce832eb4d0abab2cc3a2dfd74fd8bce4ad76d703d83c'),(0x8012b8cc,0x58,'0f37dfc6e6512b95c41e3a0966f7223b390922b976289e3ac6f006769c6f6ea4'),(0x8012b958,0x3c,'d543b3f2f773e77f5a273bbe3ff2fd98d630779d4ec4ec9ab79d4eaae40c13b8'),(0x8012ba14,0x80,'44f82d4d1e96fd28f79e2a2352e763f9c2507212a70f5c9c7b743a076f5a8d61'),(0x8012bab4,0x18,'fd6cbf76b61e6afc125e0d52f6c09c4ddfde080990050aab2247d3a562781669'),(0x800fbadc,0x2b0,'788a3c7e9880a816bad6cd1812b3f67504ff49571a1efd6685e2643ab787a69d'),(0x8010abac,0x100,'66555e5c75118f298471c84b7d74863802ed749b341a4306fa3a3c1f98d7c0e6'),(0x8010b7c4,0x98,'a030cd088fc59347390c08fe1a317e32940c6ed8a9690c820cf82d28393eea42'),(0x8010b85c,0x98,'52bf39d1e16869e3268443bc10387e498a07eba31bdd9a105f079ce1f6513d4b')]
# Complete explicit-parts camera and ordinary Jump action preparation.
SPANS.extend([
 (0x8012bc70,0x1c8,'1bdffa6bf5f7e6c0c1cff377497f5962795769a1e53e4b74e4ba927199c37bb5'),
 (0x801058e0,0x48,'87257b4e87dc89fc0456326e2daf5c61c4cc8f8aa07e08f895380b5e026db143'),
 (0x80107b38,0x80,'e2184642be30b9139403c4f2430018aa636e4da54161eefe8faa1e893188c332'),
 (0x800fa12c,0x30,'b0479170aa099f4c57399a1b82c4058318239cc9f7e8e61217a5c216d3944dcf'),
 (0x801077ec,0x98,'46c3ada1b1b0f1dec4d9f1b3276a3bbe14e667125639d177309ffba4b6d80d60'),
 (0x800fa45c,0x94,'f7d1ca253a87fce21d15f38d32e6c3bc4f755d3a7226a10f71daca2afbadcbe9'),
])

# Complete calculated-facing and direction-write wrappers.
SPANS.extend([
 (0x8010e684,0x318,'9b55dc157bfebad36a84e0ff70f1f62b551ff23592e6d0f3e130668c4cee361b'),
 (0x8010e5a8,0x6c,'104e3182211866b072302e8667e05a27c26d20d2865b1237e1132adbc631e4bc'),
])

def verify(iso=DEFAULT_ISO):
 with Path(iso).open('rb') as disc:
  header=disc.read(0x440)
  if header[:8]!=b'G8ME01\0\0':raise ValueError('US disc0 revision0 required')
  dol=struct.unpack_from('>I',header,0x420)[0];disc.seek(dol);h=disc.read(256)
  for address,length,expected in SPANS:
   for i in range(18):
    off,base,size=[struct.unpack_from('>I',h,o+4*i)[0] for o in [0,0x48,0x90]]
    if base<=address and address+length<=base+size:
     disc.seek(dol+off+address-base);data=disc.read(length)
     if len(data)!=length or hashlib.sha256(data).hexdigest()!=expected:raise ValueError('Unexpected original span '+hex(address))
     break
   else:raise ValueError('Unmapped source function '+hex(address))
 print(f'Verified{len(SPANS)} original camera/action/facing/precheck spans; generic jump pinned separately by verify_battle_jump_contracts.py')
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);verify(p.parse_args().iso)
