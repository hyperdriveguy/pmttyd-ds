"""Unchanged US NPC event-name and numeric views; no PPC or mutable globals."""
import hashlib
from field_map_npc_assets import Dol
SPANS=[
 (0x8041fce0,0x4,'638354a0b185ec05318029e52610c8de48d33523e2b4854e71154187583d1037'),
 (0x8041fd68,0x4,'ff83f8ff106269769ede5d0da22e7f511640e2146d3bfe363f9a295b9bf00414'),
 (0x8041fcd8,0x4,'b705efea49cb3296c6f0417c21fe14f10adb140333fd79d7cfc3318904f887ea'),
 (0x8041fd5c,0x4,'60f2e332c71bcda81951ea38d3c52d4a9b1a2056b334f99001f41c588afd0892'),
 (0x8041fd34,0x4,'c31425b44a7ed682a3356bf6820141e1584a15c43b6d6793f552e5d4621586f7'),
 (0x8041fd60,0x8,'d84054cd36fb1c3e72ac3b8ab934cf7c8db3d3d1993a0bf8c052fc64784c1058'),
 (0x802c1210,0x10,'d35f00dc2f639a2e4bff7c49f45aa7962803c9f3456b35ededb539b0ebf93ec4'),
 (0x802c1460,0x8,'2724f6ae533751c2b98b98a574b3dab78eb2e0aab09d2c22d6c33ba2353579ed'),
 (0x8041fd54,0x8,'ffcf0137bc017ac3967be37a1ab0790c644d249d787a7353ab11be943ee607e1'),
 (0x8041fcb0,0x4,'cfbe24a4d3802daf97e27ca5dbbc6eb2eae5b8c09b2d320a7952a777bcba6069'),
 (0x8041fcc4,0x4,'970a5a0f110b1f900dca6ea98eacfd89c1aaafab978b9706c11562fabf06d737'),
 (0x8041fccc,0x4,'9cb804aa75da9392cd1764207bff62607ab385f4147a6ebbb10d7dc42102d391'),
 (0x8041fcd4,0x4,'6c49047666986cfff6a1787c9b624f0478e689b1036914cafabd43fd2f529760'),

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
