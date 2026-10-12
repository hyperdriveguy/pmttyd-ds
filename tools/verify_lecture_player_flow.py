#!/usr/bin/env python3
"""Check static US lecture player-script selection,mode/feedback and window evidence.

Original bytes are read-only;reports contain identities/bindings,not dialogue text.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO,inventory
from rel_event_inventory import read_bindings
from dol_event_inventory import parse_commands
from message_resource_evidence import inspect_resource

BODIES = {
    'btlevtcmd_ac_timing_flag_onoff':(0x80106f40,0x60,'6339e74b1361639796e60050ddac6dae8335686f8eab9c7f38bd7fefce35297c'),
    'btlevtcmd_ac_timing_get_success_frame':(0x80106ef0,0x50,'63849b652db4b3a96360b91ee771c0752e584d3ad6ae3de10e6b5bbba9a14d87'),
    'BattleStatusWindowCheck':(0x800f7514,0xb8,'f8480d86b324df66a420f7f89bf775729bfe32591b87370c36290fc6f0763cd5'),
    'btlevtcmd_StatusWindowOnOff':(0x80103e34,0x7c,'6d48ef497be8d8e150e8c6d1dd162c894584aea7304f9dd4c3becc3f19e4d427'),
    'BattleStatusWindowEventOn':(0x800f7650,0x2c,'8f6d361123ef06e1225446824ef7e5fbd50094e449b4f1b10a4c2312ee3ba398'),
    'BattleStatusWindowEventOff':(0x800f75f8,0x2c,'6e154231c772a58be5aedf910d385c17783713ea0e19a54590e8f8288fa04581'),
    '_btlcmd_MakeSelectWeaponTable':(0x80123ec0,0xd50,'f5c5d8b7e96d3250b0d4ce29e746c2d4f39b5ac5dd0e58c421b8646c55c14d1b'),
}
EVENTS = {
    'lecture_jump':(0x68e48,0xb7c,197,'b1a897a7b0189610b96f3aef9a60939dca81fa800880020f5dd7c687d2f5d555'),
    'lecture_hammer':(0x699c4,0xa2c,193,'faeaf605b1aead51efcc6c2d03218f48d5e531da99b0c3c743786ebd47691985'),
    'lecture_end':(0x6a3f0,0xcc,13,'5720b90976f3c7b4e2f9aa78e9cf988814b9fd2354d5980f4fc0ce2134d2a831'),
}
DATA = {
    0x680cc:(0xc0,'d5e7386548b5d1faac1e2d9f0f74421e1c985b189d10f31fbda49e93e1c7d270'),
    0x6818c:(0xc0,'3a9a19b4ec33207814a7a4ecb01aa7d7c7c32e5a0833d4f43d31796666d8eabf'),
    0x6824c:(0x20,'502408a3281fe295ef5f0855c507bdd20aa34d4455e94ae58e3dc972d99cf24e'),
}
ANCHORS = {
    0x80124004:'801e0ef4',0x80124008:'74000900',0x80124010:'809e0424',
    0x80124014:'2c04ffff',0x80124024:'38800044',0x80124028:'48003315',
    0x80124038:'90640080',0x80124304:'74000a00',0x8012430c:'809e0424',
    0x80124320:'38800045',0x80124324:'48003019',0x80124334:'90640080',
    0x800f7528:'8003273c',0x800f753c:'80032740',0x800f7550:'8003274c',
    0x800f7564:'80032748',0x800f7570:'80032744',0x800f7590:'48044c61',
    0x800f75b0:'48044de1',0x80103e74:'38000001',0x80103e78:'901f2744',
    0x80103e84:'901f2748',0x80103e90:'901f2748',0x80103e94:'4bff3681',
    0x800f7664:'90032748',0x800f760c:'90032748',
    0x80106f60:'809f0004',0x80106f70:'7c002378',0x80106f74:'90031f50',
    0x80106f80:'7c002078',0x80106f84:'90031f50',
}
MESSAGES={0x68ec8:1,0x69254:2,0x69a64:8,0x69f00:9,0x6a008:10,0x6a474:24}


def verify(iso):
    info=inventory(iso)
    with Path(iso).open('rb') as disc:
        if disc.read(8)!=b'G8ME01\0\0':raise ValueError('US disc0 revision0 required')
        disc.seek(0x420);base=struct.unpack('>I',disc.read(4))[0];disc.seek(base);header=disc.read(256)
        def read(address,size):
            positions=[]
            for i in range(18):
                off,va,n=(struct.unpack_from('>I',header,k+i*4)[0] for k in (0,0x48,0x90))
                if va<=address and address+size<=va+n:positions.append(base+off+address-va)
            if len(positions)!=1:raise ValueError('Unmapped/ambiguous DOL address')
            disc.seek(positions[0]);data=disc.read(size)
            if len(data)!=size:raise ValueError('Truncated DOL')
            return data
        for name,(a,n,h) in BODIES.items():
            if hashlib.sha256(read(a,n)).hexdigest()!=h:raise ValueError(f'Body mismatch:{name}')
        for a,word in ANCHORS.items():
            if read(a,4).hex()!=word:raise ValueError(f'Anchor mismatch:{a:#x}')
        e=next(e for e in info['files'] if e['path']=='rel/gor.rel');disc.seek(e['offset']);raw=disc.read(e['size'])
        if hashlib.sha256(raw).hexdigest()!='d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a':raise ValueError('Harbor identity mismatch')
        local,imports,_=read_bindings(raw)
        for a,(n,h) in DATA.items():
            if hashlib.sha256(raw[a:a+n]).hexdigest()!=h:raise ValueError('Lecture data mismatch')
        for name,(a,n,count,h) in EVENTS.items():
            data=raw[a:a+n]
            if hashlib.sha256(data).hexdigest()!=h or len(parse_commands(data,a,{}))!=count:raise ValueError(f'Event mismatch:{name}')
        for site,target in {0x68250:0x680cc,0x68258:0x6818c,0x68260:0x6a3f0,
                            0x6817c:0x68e48,0x6823c:0x699c4}.items():
            if local.get(site,{}).get('file_offset')!=target:raise ValueError('Lecture weapon/event pointer mismatch')
        for a,callback,powers in ((0x680cc,0x80189cb0,(1,1,2,2,3,3)),(0x6818c,0x80189c38,(1,2,2,4,3,6))):
            if imports.get(a+0x1c)!=callback or struct.unpack_from('>6i',raw,a+0x20)!=powers:raise ValueError('Lecture power callback/table mismatch')
        for site,value in ((0x69998,2),(0x6a3c4,6)):
            if struct.unpack_from('>3I',raw,site+8)!=(0xfffffffa,1,value):raise ValueError('Lecture final mode writer mismatch')
        for site,param,output in ((0x69374,2,1),(0x69390,2,2),(0x693ac,2,3),
                                  (0x6a0d8,2,1),(0x6a0f4,2,2),(0x6a110,2,3)):
            if struct.unpack_from('>3I',raw,site+8)!=(0xfffffffa,param,output):raise ValueError('Lecture feedback writer mismatch')
        if struct.unpack_from('>4I',raw,0x69be0+8)!=(4,25,0,0):raise ValueError('Lecture Hammer count/period mismatch')
        for site,cmdid in ((0x68f84,1),(0x69c48,3)):
            if struct.unpack_from('>4I',raw,site+8)!=(0xfffffffe,cmdid,1,0):raise ValueError('Lecture AC setup mismatch')
        e=next(e for e in info['files'] if e['path']=='msg/US/gor_02.txt');disc.seek(e['offset']);msg_raw=disc.read(e['size']);resource=inspect_resource(msg_raw)
        messages=[]
        for site,suffix in MESSAGES.items():
            key=f'mac_0_b_t2_{suffix:03d}';off=local.get(site+12,{}).get('file_offset')
            speaker=0xfffffffe if suffix==24 else 0xfffffffa
            if off is None or raw[off:off+len(key)+1]!=key.encode()+b'\0' or imports.get(site+4)!=0x800d284c or struct.unpack_from('>I',raw,site+20)[0]!=speaker:raise ValueError('Lecture message/speaker binding mismatch')
            pairs=[p for p in resource['pairs'] if p['key_hex']==key.encode().hex()]
            if len(pairs)!=1:raise ValueError('US key missing/ambiguous')
            messages.append(dict(site=site,key=key,speaker_raw=speaker,text_sha256=pairs[0]['text_sha256']))
    return dict(schema=1,scope='Static lecture player/mode/window binding;not full controller or executed scene',
                body_checks=len(BODIES),instruction_checks=len(ANCHORS),data_checks=len(DATA),
                events=[dict(name=k,file_offset=a,size=n,commands=c,sha256=h) for k,(a,n,c,h) in EVENTS.items()],
                messages=messages,message_resource_sha256=hashlib.sha256(msg_raw).hexdigest())


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',type=Path);a=p.parse_args()
    if a.output and a.output.resolve()==Path(a.iso).resolve():p.error('Cannot overwrite ISO')
    r=verify(a.iso)
    if a.output:a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(r,indent=2)+'\n')
    print(f'Verified {len(BODIES)} bodies,{len(ANCHORS)} instructions,3 lecture events/3 data spans,'
          'weapon/mode/feedback/AC bindings and6 exact US message sites')


if __name__=='__main__':main()
