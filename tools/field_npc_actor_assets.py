"""Unchanged US NPC event-name and numeric views; no PPC or mutable globals."""
import hashlib
from field_map_npc_assets import Dol
SPANS=[
 (0x802c13c8,0x18,'7b478c1af21c79d171ad581ffd7ac318fde7808030822ef0ed03d174e918d38f'),
 (0x8041e3c8,0x4,'24ae0d93f1af72addc019182fae1ab44547a1e84758785745f4358373eab1960'),
 (0x8041fcdc,0x4,'31cf8391f9bfd8eedef79bcf6b13f2db5f20089185538b317e01e372d5b8b750'),

 (0x802c1394,0x24,'ff015cac7de04f35d881bd70a7ff41f26ec3f63aabfe7f07757e1a7be56a4b49'),
 (0x8041fcf0,4,'9dc1b62d60acff118f222dc22cb76a1c7e9f2fcc1ae7d16c56861eb0301bd59a'),
 (0x8041fd50,4,'db3aefef05f0dfc29a110d15dbd860630fa5173d7f5d908a57a60984e7495322'),
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
