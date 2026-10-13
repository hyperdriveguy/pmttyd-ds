#!/usr/bin/env python3
"""Read-only original US dmo scale, book-open and four captured-paper requests."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
from rel_event_inventory import inventory_events

def verify(iso,symbols):
    info=inventory(iso)
    if info['disc_id']!='G8ME01':raise ValueError('US source required')
    entry=next(f for f in info['files'] if f['path']=='rel/dmo.rel')
    with Path(iso).open('rb') as source:
        source.seek(6)
        if source.read(2)!=bytes(2):raise ValueError('US disc0 revision0 required')
        source.seek(entry['offset']);raw=source.read(entry['size'])
    event=inventory_events(raw,symbols,'dmo',['demo_evt'])['events'][0]
    if event['sha256']!='3e3f3b88dd5e9c8268dfdb58cf0df3120b0548814225041879830c3e637bc6c0' or len(event['commands'])!=392:raise ValueError('Original demo event changed')
    calls={c['address']:c for c in event['commands']}
    def check(address,name,values=None):
        args=calls[address]['operands']
        if args[0].get('annotation')!=name or (values is not None and [o['raw'] for o in args[1:]]!=values):raise ValueError(f'Original book call changed at {address:x}')
    if calls[0x2418]['opcode']!=108 or calls[0x24a8]['opcode']!=109:raise ValueError('Original narration brother ownership changed')
    check(0x24ac,'demo_scale');check(0x258c,'evt_sub_intpl_msec_init',[11,0,1800,2000]);check(0x25c0,'book_r')
    if calls[0x25b4]['opcode']!=61 or calls[0x25b4]['operands'][1]['raw']!=4064977536:raise ValueError('Source book angle divisor changed')
    for address,word in [(0x2270,0x463b8000),(0x2274,0x44160000),(0x2278,0x3f800000),(0x227c,0x44bb8000),(0x2280,0x447a0000),(0x2260,0x3fc00000),(0x22a8,0x44180000),(0x22ac,0x43f00000)]:
        if struct.unpack_from('>I',raw,address)[0]!=word:raise ValueError('Source scale/viewport constant changed')
    for offset,size,digest in [(0x388,0x114,'b6dbf358ff3734debf4de2343c33724ff85f24449c92d14fa165def8b20d7b4c'),(0x938,0x900,'8323297ac6a02d7f698755fe12f18ea4dfd62e3a695c8f8a11bae6d21a937d6e'),(0x1acc,0xf8,'cf9213cf0072e98377c601163d257c2326aedcb418090c024787e4c1dc415d0e')]:
        if hashlib.sha256(raw[offset:offset+size]).hexdigest()!=digest:raise ValueError('Source book/scale/capture provider changed')
    for index,(paper,capture,anim,wait) in enumerate([(0x274c,0x2768,0x27c0,0x27d0),(0x28cc,0x28e8,0x2940,0x2950),(0x2a68,0x2a84,0x2adc,0x2aec),(0x2c38,0x2c54,0x2cac,0x2cbc)]):
        name=f'img{index+1}'
        check(paper,'evt_img_set_paper');check(capture,'evt_img_alloc_capture');check(anim,'evt_img_set_paper_anim');check(wait,'evt_img_wait_animend')
        if calls[paper]['operands'][2].get('identifier')!='OFF_d_meku_right' or calls[anim]['operands'][2].get('identifier')!='A_1':raise ValueError('Original paper resource/pose changed')
        for address in [paper,capture,anim,wait]:
            if calls[address]['operands'][1].get('identifier')!=name:raise ValueError('Original captured-image ownership changed')
        if [o['raw'] for o in calls[capture]['operands'][2:]]!=[0,0,1,124,0,386,480]:raise ValueError('Original capture rectangle changed')
    return 53

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--symbols',default='artifacts/reference/us_symbols.csv');a=p.parse_args();print(f'Original392-command demo/provider hashes and {verify(a.iso,a.symbols)} book/capture/constant anchors verified read-only')
if __name__=='__main__':main()
