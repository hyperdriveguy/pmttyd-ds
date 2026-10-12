#!/usr/bin/env python3
"""Check US Crump/NPC versus lecture helper identities and mode bindings read-only.

Metadata only. Whole event hashes do not imply all event/callback semantics are closed.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from dol_event_inventory import parse_commands
from iso_assets import DEFAULT_ISO, inventory
from rel_event_inventory import read_bindings
from message_resource_evidence import inspect_resource

DATA = {
    'npc_kind':(0x66d78,0xc4,'6b00099c439022ecc741ae0d442a9e6cbb2426a95df27dcf8aa43d5ef6905bd0'),
    'lecture_kind':(0x66fe8,0xc4,'204f42c178a0c9876487d9fb97a026eae48b41fa05b7b90af7a759e03b927f32'),
    'boss_helper_setup':(0x647d8,0x30,'f62061faba2ab15d9efe5e313b6ccb19d05d7f83f1ac0f460fcc8ac898ce2054'),
    'lecture_setup':(0x60cb0,0x30,'f513e136be5105e8cc69c34dee087e268c00cf381038fa2469839e0ecccc79bb'),
    'lecture_group':(0x60ce0,0x20,'490b0ff985647f92f3dee5183acced938dd6edf8f841edf182b144f0f8eeb19b'),
    'lecture_offlist':(0x60de8,0x18,'d0ba2a1f974f2831ccc0e7f9a13e7ff217c1a199ce7d3774bdd922741e184e53'),
    'lecture_config':(0x60efc,0x44,'24266c16f303bd4515d81868568b366fc955455815300433197577c779fe70df'),
    'professor_setup':(0x67130,0x30,'4251fdb009fd42da7c4c13338828725ef320e14bb7a52cd00886b55df1289ab0'),
}
LEAVES = {
    '_get_defence_ac_result':(0x11700,0x84,'84c74cd15681b5247e1b4afca2551dbc91df7d9737e5cccb0dc758d2fca49f50'),
    '_battle_set_lecture_unit_id':(0x118f0,0x40,'22d584aa7de60350fe7ce4e891614067f96670529bef44f3ad424876795e4b9b'),
    '_set_lecture_mode':(0x11930,0x5c,'ef22e790534f1362c9decffc62c9ee429e9853bbf84120a3c27c55c7c37eac09'),
}
EVENTS = {
    'npc_init':(0x66ed8,0x64,8,'6828d2b138a84c79265a4954ffabaf18cd14a9ab0f2a007c2dd71d63b955d0a3'),
    'npc_damage':(0x66f3c,0x28,5,'f59a6bfb7c00767c28083d3812e89e8a081a3c9e96ec8e55de2e2537ee1a04ba'),
    'npc_phase':(0x66f64,8,2,'1e9fcd4ca7e6723c4a822c370faad68aa89031573b2376d87e4cc3c6626ede61'),
    'npc_attack':(0x66f6c,0x14,3,'4663084af36b2537184d3e8b9be45025369a6f56b49e7187ffbcff14b7ab145f'),
    'npc_wait':(0x66f80,0x18,3,'14bb860655a0fa243d510f0c64ef87508cce5f9fac9524eada619e21c22af21e'),
    'lecture_init':(0x67160,0x138,19,'e42159636818851bcab531d5d623874df0eb75fd6c733830297273f604dedacb'),
    'lecture_attack':(0x674b4,0x4c,8,'c80901164876194130ba57657434b8db9e91211bd5dfeb8afa61dcc1b2fdc782'),
    'professor_init':(0x682b8,0x98,11,'d208eae99180813d315b9d5815b75aa85a51f0b7ceb5f37dc89f9b0a45e4ada1'),
    'professor_phase':(0x68378,0xaa4,200,'4077a55020985e82c79fe151b1ce7d57590c74d52b2dc0572f9ef01648b12bca'),
    'lecture_counter':(0x672c0,0x1d4,26,'4a27b753915e5b6ab29af1c11a02d28d9fa932c81877acbdd2dfeb09f8fd3ec3'),
}
LINKS = {
    0x647d8:0x66d78,0x66e34:0x66ed8,0x66e38:0x66e84,
    0x60f10:0x60de8,0x60dec:0x60ce0,0x60ce4:0x60cb0,0x60cb0:0x66fe8,
    0x670a4:0x67160,0x670a8:0x670d4,0x670d8:0x672c0,
    0x67130:0x67fb8,0x67208:0x11930,0x67218:0x67130,0x67238:0x118f0,
    0x674b8:0x67680,0x68074:0x682b8,
    0x66ee4:0x66f80,0x66ef4:0x66f6c,0x66f04:0x66f3c,
    0x6716c:0x6749c,0x6717c:0x67494,0x6718c:0x674b4,0x6719c:0x67298,
}
LEAF_IMPORTS = {
    0x1170a:(6,0x8041eb90),0x11716:(4,0x8041eb90),0x1176c:(10,0x80030cfc),
    0x118fa:(6,0x8041eb90),0x1190a:(4,0x8041eb90),0x11910:(10,0x80031260),
    0x1193a:(6,0x8041eb90),0x1194a:(4,0x8041eb90),0x1194c:(10,0x801253ec),0x1195c:(10,0x800f88cc),
}

MESSAGE_SITES = {
    0x6845c:(0,'print',0xfffffffe),0x68538:(3,'print',0xfffffffe),
    0x685ec:(4,'print',0xfffffffe),0x68644:(5,'print',0xfe363c83),
    0x6876c:(7,'print',0xfffffffe),0x688b8:(11,'print',0xfffffffe),
    0x6896c:(12,'print',0xfffffffe),0x689c4:(13,'print',0xfe363c83),
    0x68b0c:(16,'print',0xfffffffe),0x68b68:(17,'print',0xfffffffe),
    0x68b80:(18,'select',None),0x68b9c:(19,'append',None),
    0x68bd0:(20,'append',None),0x68c60:(21,'print',0xfffffffe),
    0x68c80:(22,'print',0xfffffffe),0x68ca0:(23,'print',0xfffffffe),
    0x68cc0:(21,'print',0xfffffffe),0x68da0:(21,'print',0xfffffffe),
    0x68dc0:(22,'print',0xfffffffe),0x68de0:(23,'print',0xfffffffe),
}


def verify(iso):
    info=inventory(iso)
    with Path(iso).open('rb') as disc:
        if disc.read(8)!=b'G8ME01\0\0':raise ValueError('US disc0 revision0 required')
        entry=next(x for x in info['files'] if x['path']=='rel/gor.rel')
        disc.seek(entry['offset']);raw=disc.read(entry['size'])
        if hashlib.sha256(raw).hexdigest()!='d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a':raise ValueError('Harbor identity mismatch')
        local,imports,_=read_bindings(raw)
        for name,(off,size,digest) in {**DATA,**LEAVES}.items():
            if hashlib.sha256(raw[off:off+size]).hexdigest()!=digest:raise ValueError(f'Data/leaf mismatch:{name}')
        for name,(off,size,count,digest) in EVENTS.items():
            data=raw[off:off+size]
            if hashlib.sha256(data).hexdigest()!=digest or len(parse_commands(data,off,{}))!=count:raise ValueError(f'Event mismatch:{name}')
        for site,target in LINKS.items():
            if local.get(site,{}).get('file_offset')!=target:raise ValueError(f'Local binding mismatch:{site:#x}')
        for off,key in ((0x66d78,0xdd),(0x66fe8,0xb1)):
            if struct.unpack_from('>I',raw,off)[0]!=key or struct.unpack_from('>2h',raw,off+8)!=(10,0):raise ValueError('Kind identity/HP/FP mismatch')
        if struct.unpack_from('>I',raw,0x670d4)[0]!=0x24 or raw[0x66e84:0x66e8c]!=bytes(8):raise ValueError('Helper data table key/terminator mismatch')
        bindings={};table=struct.unpack_from('>I',raw,0x10)[0]
        imp,size=struct.unpack_from('>II',raw,0x28)
        for j in range(imp,imp+size,8):
            module,stream=struct.unpack_from('>II',raw,j)
            if module:continue
            section,offset=None,0
            for _ in range(len(raw)//8):
                delta,kind,target,addend=struct.unpack_from('>HBBI',raw,stream);stream+=8
                if kind==203:break
                if kind==202:section,offset=target,0;continue
                offset+=delta
                site=(struct.unpack_from('>I',raw,table+section*8)[0]&~3)+offset
                if site in LEAF_IMPORTS:bindings[site]=(kind,addend)
            else:raise ValueError('Unterminated REL import')
        if bindings!=LEAF_IMPORTS:raise ValueError('Lecture leaf global/service relocations mismatch')
        # Pin concrete work-state writers without interpreting unknown phase services.
        writers={0x68318:0,0x683d8:1,0x684d8:3,0x6870c:5,0x687dc:7,0x68a8c:9}
        for off,value in writers.items():
            if struct.unpack_from('>3I',raw,off+8)!=(0xfffffffe,1,value):raise ValueError('Professor phase/init work writer mismatch')
        if struct.unpack_from('>3I',raw,0x674e0+8)!=(0xfffffffa,1,4) or struct.unpack_from('>3I',raw,0x6745c+8)!=(0xfffffffa,1,8):raise ValueError('Lecture wrapper/counter transition mismatch')
        messages=[]
        msg_entry=next(x for x in info['files'] if x['path']=='msg/US/gor_02.txt')
        disc.seek(msg_entry['offset']);message_raw=disc.read(msg_entry['size'])
        resource=inspect_resource(message_raw)
        for site,(suffix,form,speaker) in MESSAGE_SITES.items():
            key=f'mac_0_b_t2_{suffix:03d}'
            key_off=local.get(site+12,{}).get('file_offset')
            if key_off is None or raw[key_off:key_off+len(key)+1]!=key.encode()+b'\0':raise ValueError('Coordinator message binding mismatch')
            target={'print':0x800d284c,'append':0x800d23c4,'select':0x800d2090}[form]
            if imports.get(site+4)!=target:raise ValueError('Coordinator message service mismatch')
            if form=='print' and struct.unpack_from('>I',raw,site+20)[0]!=speaker:raise ValueError('Coordinator speaker mismatch')
            pairs=[pair for pair in resource['pairs'] if pair['key_hex']==key.encode().hex()]
            if len(pairs)!=1:raise ValueError('US message key missing/ambiguous')
            messages.append(dict(site=site,key=key,form=form,speaker_raw=speaker,text_sha256=pairs[0]['text_sha256']))
        disc.seek(0x420);dol_base=struct.unpack('>I',disc.read(4))[0]
        disc.seek(dol_base);header=disc.read(256)
        def read(address,length):
            positions=[]
            for i in range(18):
                off,va,n=(struct.unpack_from('>I',header,k+i*4)[0] for k in (0,0x48,0x90))
                if va<=address and address+length<=va+n:positions.append(dol_base+off+address-va)
            if len(positions)!=1:raise ValueError('Unmapped DOL evidence')
            disc.seek(positions[0]);data=disc.read(length)
            if len(data)!=length:raise ValueError('Truncated DOL')
            return data
        if read(0x80125484,4).hex()!='80650424':raise ValueError('Lecture selector-6 field mismatch')
    return dict(schema=1,scope='Static US helper identities,init/mode requests and coordinator branches;not original execution',
                rel_sha256=hashlib.sha256(raw).hexdigest(),message_resource_path='msg/US/gor_02.txt',message_resource_sha256=hashlib.sha256(message_raw).hexdigest(),
                message_sites=messages,data_checks=len(DATA),rel_leaf_checks=len(LEAVES),
                event_checks=len(EVENTS),local_binding_checks=len(LINKS),leaf_import_checks=len(LEAF_IMPORTS),
                events=[dict(name=n,file_offset=o,size=s,commands=c,sha256=h) for n,(o,s,c,h) in EVENTS.items()])


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',type=Path)
    a=p.parse_args()
    if a.output and a.output.resolve()==Path(a.iso).resolve():p.error('Cannot overwrite ISO')
    r=verify(a.iso)
    if a.output:a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(r,indent=2)+'\n')
    print(f'Verified {len(DATA)} data spans,{len(LEAVES)} REL leaf bodies,{len(EVENTS)} event spans,'
          f'{len(LINKS)} local and{len(LEAF_IMPORTS)} import bindings,work writers and selector-6')


if __name__=='__main__':main()
