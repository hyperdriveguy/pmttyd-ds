#!/usr/bin/env python3
"""Verify complete compiled field-map source spans against the read-only US DOL."""
import hashlib,re
from pathlib import Path
from field_map_npc_assets import Dol
from iso_assets import DEFAULT_ISO
ROOT=Path(__file__).resolve().parents[1]
BOUNDARIES={'mapInit':(0x8002af18,0x44),'mapGetMapObj':(0x8001f878,0xa8),'mapEntry':(0x8002a46c,0x9c),'mapEntrySub':(0x8002a508,0x278),'mapGetJoints':(0x8002b120,0x4c),'mapGetJointsSub':(0x8002b16c,0x2c4),'_mapEnt':(0x8002a780,0x588),'mapReCalcMatrix':(0x80028130,0x420)}
def verify(iso=DEFAULT_ISO):
 source=(ROOT/'source/field/field_map_source_program.inc').read_text()
 headers=re.findall(r'^// (\w+) ([0-9a-f]{8})/([0-9a-f]+) SHA256 ([0-9a-f]{64})$',source,re.M)
 if len(headers)!=len(BOUNDARIES):raise ValueError('Missing or duplicate complete source boundary')
 cases=re.findall(r'^case 0x([0-9a-f]{8}):.* // ([0-9a-f]{8}) ',source,re.M)
 words={int(a,16):bytes.fromhex(w) for a,w in cases}
 if len(words)!=len(cases):raise ValueError('Duplicate continuation address')
 dol=Dol(iso);covered=set()
 try:
  for name,address,size,sha in headers:
   address=int(address,16);size=int(size,16)
   if BOUNDARIES[name]!=(address,size):raise ValueError('Changed original callable boundary')
   raw=dol.read(address,size)
   if hashlib.sha256(raw).hexdigest()!=sha:raise ValueError('Original DOL hash mismatch')
   for offset in range(0,size,4):
    at=address+offset
    if words.get(at)!=raw[offset:offset+4]:raise ValueError('Missing or changed source instruction')
    covered.add(at)
 finally:dol.close()
 if covered!=set(words):raise ValueError('Extra instruction outside complete source boundaries')
 return len(words)
if __name__=='__main__':print('Verified',verify(),'original map instructions in',len(BOUNDARIES),'complete source boundaries')
