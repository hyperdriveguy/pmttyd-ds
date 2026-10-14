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

# Complete StartAC/help/current audience-window activation writers.
SPANS.extend([
 (0x80107788,0x64,'2762fc5508e747fac02e5d8ef87c40f93788ea9bc57ab616f0e40f4423028833'),
 (0x8021690c,0x48,'b6278b065afb487c64e45d10397ca1c30d0a05798ef246b1eb8a419de7acc975'),
 (0x80216954,0x10,'8b8ff3af915b6a64ad1eb827a193facf1c22c28601ad1262a9d33fe05fc53a82'),
 (0x801a1cdc,0x30,'33a3b7999c1fbf527f0fc10ddaf665d0c21e1bd2d2e43b481d2f59cc8c07896b'),
])

# Complete result/stop/delete and stored-result/count protocol.
SPANS.extend([
 (0x80107708,0x50,'98b506c4515be425bda40a46dc11da265d2e8db8ddbf8fb3efb9217380207ead'),
 (0x800fa574,0x3c,'e06ae1fbd6366bc9d95f673730e5da5f2307cf57e22b172e0c1f8f9e5d8e6cac'),
 (0x80107758,0x30,'4d7b1e106bdb24f69e8bc557bc99d0c6c23b1f501fc01e222d70b177330724ef'),
 (0x800fa420,0x30,'827ac29f800c2815b5866d21a5b16ced663d678704089b88efc4e8654a4e0dc4'),
 (0x8012a7c0,0xc,'b5bed9c540056780e1b5d2a5431edf510767f94f3c7f58a08087fd321ebb3343'),
 (0x80107548,0x34,'d22d7c93efd083c0ab04d4d0bd352908851b7b038d88851104548913da542cf4'),
 (0x80107660,0x34,'3a2c76926cdefc97a4eef4118bbab2912529502dc8f4824e41f59ba139a299dc'),
])

# Complete AC declaration, audience waiting/bad/good and literal stub routes.
SPANS.extend([
 (0x8010749c,0xac,'c20cf2c842b004460f0a56f7594c08c11d4d46878d5a9c599a7be9e0ac31f4df'),
 (0x800fa4f0,0x84,'c82286c77c2573bcd8ef50b896ee39f1d53abd6ff893f7a6b96a7b67b2cf3882'),
 (0x8019f670,0x98,'df7bd483b945f808d51cc4d65f8e385999ba98fe032d7a01e26a0a8de4c69901'),
 (0x8019f708,0x80,'be90a3d3880a8298008b8d298d927fa3b2df40b22675d48fa3e71813894dabc2'),
 (0x801a28e4,0x7c,'0205a1e7ac0247bb01bbe25b042f9dd72348421124e244268d11dd16c92c403e'),
 (0x801a2960,0x60,'c925063fca01bf9adf87713d9ebfd6f20d2951189ddfc799332aa03ba11d6da5'),
 (0x801a29c0,0x64,'21d4c42ea7469e96002e8a917d4e5ed2736c391d5dcbed4200a0e2ebc445e95d'),
 (0x80139898,0x24,'b11a0a11e6ea1691514c8a5787eee3c5f6a8de91321852a4aa49687dd10f20c8'),
 (0x8019f4d0,0x20,'52ccf99f0a70899a332ff396bfec066feebf8a48cbeb986b023de9826f2f73ea'),
 (0x80139af4,0x8,'8286a529f25a748db551e16c92ccf02567e092085eb2f9c47fed829693da3b13'),
])

# Complete AP report/caller, kind/presence and slot-work requests.
SPANS.extend([
 (0x8010436c,0x4c,'8b24fb2c40947642dbe860a327c4e7ae829d5f95caa353938a427cc648f54f52'),
 (0x801a1d0c,0x294,'7b6f8d494015a1b525c9edae4026f5b0f4a961f2219ea7b522d03b905f763d24'),
 (0x801a20d8,0x6c,'d9017167cf38f853affe735e5e14d74e83648e90b3ca2eb7eeacd14be62b36c0'),
 (0x801a1fa0,0x7c,'a025411304a9eb0c7244a2a237530021609b7f931cd67de7e6e8353f7252c0b4'),
 (0x80203370,0x24,'179deb1e6e16d83c96ae590e53cdc9ca62a913a787e5aa982bb776cd17f76f7b'),
 (0x8020524c,0x10,'78c442cd3a8d835c51e54e160ee702bd26547edbe06c69fad9a80202fcc65231'),
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
