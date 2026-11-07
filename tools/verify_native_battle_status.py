#!/usr/bin/env python3
"""Pin the actual original children/tables used by the native HUD adaptation."""
import hashlib,struct
from field_map_npc_assets import Dol
from iso_assets import DEFAULT_ISO
from verify_source_status_values import verify as verify_parent
BODIES={
 0x8013cb24:(0x91c,'60dcf91767ee595d1dce14579cdab17eb7966879167d3b01285495e9e7a3ba48'),
 0x800f88cc:(0x3c,'6a40e068a63873b3f93f3cf0bdc72c59323e60ff5b961ac740b22eddb58276fa'),
 0x800f87a8:(0x6c,'26840147b00e408e2cd371d8c23ddd7057de2da195881a7ab014b758fa28f869'),
 0x800f8728:(0x80,'618380e524d9f6805cd215b283aa4450e7e8fb392921ef72a3bec5b00cd8dd37'),
 0x800f8444:(0xf4,'b7f23d4e0a11af27bdd70a13edddc6d364afc8cbf66a7c051edcdd1a9fed67dd'),
 0x800d2e44:(0x14,'a608ec9867be372e24b67f83c266191374edfffcc2202c7022b715480a45364c'),
 0x8013d440:(0x214,'27d936e8779bba587eb1c21e7eaea9fb486670f1076bddc2bcdeaeb6824c575a')}
GETTERS={0x800d3d30:0x70,0x800d3d24:0x72,0x800d3c98:0x74,0x800d3c8c:0x76,0x800d3c44:0x7a,0x800d3bb8:0x7c,0x800d3424:0x96,0x800d3e64:0x78}
def verify(iso=DEFAULT_ISO):
 parent=verify_parent(iso);d=Dol(iso)
 try:
  for a,(n,h) in BODIES.items():
   if hashlib.sha256(d.read(a,n)).hexdigest()!=h:raise ValueError('Original HUD child changed: '+hex(a))
  for a,o in GETTERS.items():
   if d.read(a,12)!=bytes.fromhex('806d1be0')+struct.pack('>I',0xa8630000|o)+bytes.fromhex('4e800020'):raise ValueError('Original signed pouch leaf changed')
  if d.read(0x800d5eb0,8)!=bytes.fromhex('806d1be04e800020'):raise ValueError('Original pouch pointer leaf changed')
  raw=d.read(0x80417658,0x64)
  if hashlib.sha256(raw).hexdigest()!='aff9e7c83097699115477529eab31076514674c230bbc1e6e1669a71d5c0def8':raise ValueError('Original partner/gauge tables changed')
  portraits=list(struct.unpack('>8H',d.read(0x8041767c,16)));full=list(struct.unpack('>8H',d.read(0x8041768c,16)));partial=list(struct.unpack('>16H',d.read(0x8041769c,32)))
  assert portraits==[428,428,429,430,431,438,439,440] and full==list(range(447,455)) and partial==list(range(456,472))
  assert struct.unpack('>f',d.read(0x80422c58,4))[0]==12
 finally:d.close()
 return dict(parent=parent,whole_children=len(BODIES),signed_pouch_getters=len(GETTERS),portraits=portraits,gauge_full=full,gauge_partial=partial,scope='Native HUD adaptation; no source StatusWinMain/Disp scheduling or GX receipt')
if __name__=='__main__':print(verify())
