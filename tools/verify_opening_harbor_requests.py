#!/usr/bin/env python3
"""Pin the read-only US used harbor ship arrival/departure event requests."""
import argparse,struct,hashlib
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
from rel_event_inventory import inventory_events
HASHES={'gor_00_init_evt':'feca019e68a4b78036ea454dee444be4287f18b1b2f3c322d82b232190a68496','enter_gorotsuki_town':'6679b66c1735f1f541ff22f8aca1e155282a7463006212d5f353c29606bcce6d','ship_a_animation':'d8375356a54d5c7345e688565430bf7f1d9b8eb1d8b3512dd2cd49fd5a175bea','ship_pera':'931052cd141060967e76bc1cbba8601654bd0cd5e0f7a420686c4eea0b957f75','ship_a_kaji_animation':'4186c0474d694c37847e6e8ec4c499ae953d906f091c5064cd01cb619b36ab90','ship_a_wakka_animation':'52b25cf64e70cdc88008ae363c37b67c422266c0b6be3958f936756e5a162d4b','ship_a_entotsu_animation':'8c56025a7dcacc41a4346ae7b780cdfe4d1420696b221f83449c4f3f1fb012e8','ship_a_neji_animation':'2b352da94e6f26b434b7f9646252b2cd8a92c76c5fa1c771442ff620a175f5a4','ship_a_yure_animation':'e4c49cf6f5c3163e269d4b5a1cfc46cf24a071a09a633b022a05c77fc2b2ded9'}
def verify(iso,symbols):
 entry=next(f for f in inventory(iso)['files'] if f['path']=='rel/gor.rel')
 with Path(iso).open('rb') as s:s.seek(entry['offset']);raw=s.read(entry['size'])
 data=inventory_events(raw,symbols,'gor',list(HASHES));calls={}
 for e in data['events']:
  if e['sha256']!=HASHES[e['name']]:raise ValueError('Original harbor event changed: '+e['name'])
  calls.update((c['address'],c) for c in e['commands'])
 checks=[(0x255d0,10,[3000]),(0x255f4,91,['evt_sub_intpl_msec_init',0,0xfd050f84,0xfffffef2,2000]),(0x25698,91,['evt_sub_intpl_msec_init',4,0xfffffef2,0xffffffb5,2000]),(0x25888,10,[1000]),(0x258c0,10,[2000]),(0x258f8,10,[1000]),(0x2593c,10,[400]),(0x25a34,10,[2000]),(0x25ab4,91,['evt_sub_intpl_msec_init',1,0xffffffb5,0xfffffef2,2000]),(0x25b1c,91,['evt_sub_intpl_msec_init',0,0xfffffef2,0xfffffb14,6000])]
 for address,opcode,wanted in checks:
  c=calls[address];args=[o.get('annotation') if i==0 and opcode==91 else o['raw'] for i,o in enumerate(c['operands'])]
  if c['opcode']!=opcode or args!=wanted:raise ValueError(f'Original harbor request changed:{address:x}')
 for address,opcode in [(0x25710,107),(0x25938,107),(0x25ab0,107),(0x251d8,91)]:
  if calls[address]['opcode']!=opcode:raise ValueError('Original parallel/paper wait changed')
 for address,name,identifier in [(0x258a8,'evt_msg_print','mac_0_014'),(0x258e0,'evt_msg_print','mac_0_015'),(0x250c0,'evt_img_set_paper','OFF_d_kaiten'),(0x25168,'evt_img_set_paper_anim','Z_1'),(0x251c8,'evt_img_set_paper_anim','A_1')]:
  args=calls[address]['operands']
  if args[0].get('annotation')!=name or not any(o.get('identifier')==identifier for o in args):raise ValueError('Original message/paper identity changed')
 if calls[0x2f088]['opcode']!=25 or [o['raw']for o in calls[0x2f088]['operands']]!=[4124967296,2] or calls[0x2f094]['operands'][0].get('annotation')!='evt_snd_bgmon' or calls[0x2f094]['operands'][1]['raw']!=512 or calls[0x2f094]['operands'][2].get('identifier')!='BGM_STG0_GOR1':raise ValueError('Original story2 harbor BGM On changed')
 if struct.unpack_from('>f',raw,0x1582c)[0]!=2000 or struct.unpack_from('>f',raw,0x157c0)[0]!=2:raise ValueError('Original rocking rate changed')
 with Path(iso).open('rb') as source:
  source.seek(0x420);dol=struct.unpack('>I',source.read(4))[0];source.seek(dol);header=source.read(256)
  def read(address,size):
   for j in range(18):
    offset,base,length=(struct.unpack_from('>I',header,b+4*j)[0] for b in [0,0x48,0x90])
    if base<=address and address+size<=base+length:
     source.seek(dol+offset+address-base);return source.read(size)
   raise ValueError('Unmapped jump source')
  if read(0x80315ac8+67*16+12,4)!=bytes([70,64,0,0]):raise ValueError('Original harbor BGM table volume/pan changed')
  if hashlib.sha256(read(0x80051868,0x364)).hexdigest()!='eb67fa0707263fcd321df7e977c9f034517a71bfcd835ca96454feacd088a003':raise ValueError('Original nohit jump callback changed')
  if struct.unpack('>I',read(0x8041fdfc,4))[0]!=0xc0800000:raise ValueError('Original nohit height coefficient changed')
 return len(HASHES)+len(checks)+4+5+2+2+2
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--symbols',default='artifacts/reference/us_symbols.csv');a=p.parse_args();print('Original harbor ship requests:',verify(a.iso,a.symbols),'event/request/scalar anchors verified')
