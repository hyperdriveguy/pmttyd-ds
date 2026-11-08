#!/usr/bin/env python3
"""Verify complete compiled field-map source spans against the read-only US DOL."""
import hashlib,re
from pathlib import Path
from field_map_npc_assets import Dol
from iso_assets import DEFAULT_ISO
ROOT=Path(__file__).resolve().parents[1]
BOUNDARIES={'mapInit': (2147659544, 68), 'mapGetMapObj': (2147612792, 168), 'mapEntry': (2147656812, 156), 'mapEntrySub': (2147656968, 632), 'mapGetJoints': (2147660064, 76), 'mapGetJointsSub': (2147660140, 708), '_mapEnt': (2147657600, 1416), 'mapReCalcMatrix': (2147647792, 1056), 'mapLoad': (2147650824, 104), '_mapLoad': (2147651352, 3040), 'mapUnLoad': (2147650936, 416), '_mapAllocTail': (2147679840, 248), '_mapFree': (2147679684, 156), 'arcOpen': (2147975908, 212), 'arcDataCheck': (2147976260, 172), 'arcDelete': (2147976120, 56), 'UnpackTexPalette': (2147711852, 288), 'mapSearchDmdJoint': (2147612960, 56), 'mapSearchDmdJointSub': (2147614024, 1408), 'mapSearchDmdJointSub2': (2147613016, 1008), 'TEXGet': (2147711836, 16), 'mapBuildTexture': (2147659016, 528), 'mapGetBoundingBox': (2147659612, 452), 'makeDisplayList': (2147656232, 580), 'mapGetWork': (2147660856, 20), 'DVDMgrOpen': (2148814472, 412), 'DVDMgrRead': (2148814360, 112), 'DVDMgrGetLength': (2148814284, 8), 'DVDMgrClose': (2148814292, 16), 'DVDMgrMain': (2148814884, 612), 'compare': (2148815496, 68), 'qqsort': (2147509500, 388), 'fsort': (2147509888, 1548), 'hitEntry': (2147583576, 156), 'hitEntrySub': (2147583732, 632), '_hitEnt': (2147584364, 1164), 'hitCalcVtxPosition': (2147575208, 1204), 'camLoadRoad': (2147545964, 232), 'camRoadSetup': (2147541252, 644), 'camUnLoadRoad': (2147545872, 92), 'camSetTypePersp': (2147543232, 28), 'camShiftInit': (2148481400, 100), 'mapGetDvdRoot': (2147660900, 8), 'GXGetTexBufferSize': (2150330508, 348), 'shadowInit': (2147671276, 1272), 'shadowGetCharShadowProjection': (2147662972, 12)}
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
