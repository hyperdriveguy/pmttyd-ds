"""Unchanged US NPC event-name and numeric views; no PPC or mutable globals."""
import hashlib
from field_map_npc_assets import Dol
SPANS=[
 (0x802c1470,0xc,'15ec7bf0b50732b49f8228e07d24365338f9e3ab994b00af08e5a3bffe55fd8b'),
 (0x802c14b0,8,'07af8387624cf8f36152797d2f561246f21237d1d22b5c13006eacfa6f475e43'),
 (0x802c14f0,0x40,'9835efd706b06ea7c97de419a9f1d1bb41955c162869de8486c06146b353f55b'),
 (0x8041fd80,4,'4bdb347a5703ee47ed0bab05bf452f4b3c693bcae8ecc0ebdf7751db42d6503c'),
 (0x8041fdcc,0x10,'e4eca9d01339e5e5a1d33235a3949140a2140028b4256aba5c2ceeef28dbea8d'),
 (0x8041fde0,8,'261599a5e75c18dc68a059591cb81c14d33b12b4dc71c83fc67f0816af6618a5'),
 (0x8041f448,4,'df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119'),
 (0x8041f45c,4,'1aeadc6fe40c6e83ce89ce6d57ec3cd9f1a30d2f545c978b504293509dc90880'),
 (0x802bf240,8,'fd8adff6e8f476c5527d46ac19f9e959e9b0b177b4fe577c44343af59b06dd29')]
def actor_event_segments(iso):
 dol=Dol(iso);segments=[];views=[]
 try:
  for address,size,expected in SPANS:
   data=dol.read(address,size);digest=hashlib.sha256(data).hexdigest()
   if digest!=expected:raise ValueError('Unexpected original NPC actor-event data')
   segments.append((address,data));views.append(dict(address=address,size=size,sha256=digest))
  return segments,dict(dol_views=views,policy='Unchanged original strings/constants only; no BSS, NPC success fixtures or executable PPC. Local use only; no redistribution rights.')
 finally:dol.close()
