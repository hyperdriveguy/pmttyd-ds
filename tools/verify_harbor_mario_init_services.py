#!/usr/bin/env python3
"""Check original Mario initializer, native registration/sound/part/emblem bindings."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
SPANS=[(2148574464, 92, 'da73990766cfc24a04c8ff41146651c192c0f13835ce741dd61d2b4128636607'), (2148574088, 100, '80292490f2827afe9c03507791c903df8748b85b6503d6ab1239267ba00a0381'), (2148574556, 92, '45907632c96c0d6b05c04ead0adea613beea853b28adc3ca1ea3675839be9337'), (2148573996, 92, '23d4a744d19aad39b116cc50d1d01d4cfe9a27c25636df48ce14aac0ba0cc415'), (2148574372, 92, '8280ab116f2c414d87c4899e864d86aaf98fb4bcd8fdb36224c7939e31bfa5dc'), (2148551432, 128, '99a96d24f101883b9b8fbb349663ec0c2c09d76a11cdde50000d9095c295e37f'), (2148552156, 188, '17071990fd46594c003f45f258af809147e4fbe05cff41ec204dbb31cd2673c2'), (2148552548, 188, '6fbf5c49b3c4f1805db8e3115ae950548462de395e07a71eb2176e11fa51e744'), (2148594892, 156, 'eaa015d620a6eae69dfe2839d3d9f7e442a16839354959a733e53ef0565212fc'), (2148557812, 116, 'c4d9d1f94f59df4f91aa4b3e617ce197a6f1518b6db7858869e2a10f7b1deb57'), (2148815876, 528, '65a7ed52e8d28122c99a1f9f2dc2bb63972065d41eadf4afd409bcefe92911d4'), (2151100100, 300, '48d367d76dc627d4422d34f1d3d4e3716810a8463f5ec57cb544c53a1f649e9b'), (2148689076, 24, '91afa779db990b45fef6dfe59a941f6b762a40470b665a1ed7b7365af3eeed19'), (2151818916, 4, 'df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119'), (2148558040, 108, 'a3df22081790df8816e0c9bf7f27cb0cc3ee69040da2fe2a6cf6f5559fcf31a8'), (2148536564, 56, '7a4b6196e4317bdd469222f296d25b540271f3b51d90678070d4166c78ad1204'), (2151098552, 272, '24285aa17213621aa1b1b35ddefbbcfe6a4ba339c357dd10c2ff6fad9ba6c723'), (2151821040, 6, 'c84a5981384ca7609517609c60ca3fc45ecf7d914cae452feae6309c39f7d79f')]
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
 print('Verified18 original Mario init/registration/sound/parts/emblem/wait-pose spans')
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);verify(p.parse_args().iso)
