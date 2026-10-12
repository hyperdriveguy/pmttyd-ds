#!/usr/bin/env python3
"""Read-only US sampler/reset references and system-level/event-speed evidence.

References are discovery evidence, not executed callers or a complete writer graph.
Metadata reports must remain outside source history.
"""
import argparse
from bisect import bisect_right
import csv
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO, inventory
from rel_event_inventory import read_bindings
from us_usage_audit import audit

TARGETS = ('BtlPad_WorkUpdate','BtlPad_WorkInit','BattlePadInit','BattlePadManager',
           'BattleActionCommandManager','evtSetSpeed','marioStSystemLevel',
           'marioStGetSystemLevel','evtStartAll','evtStopAll')
BODIES = {
    'BattleActionCommandResetDefenceResult':(0x800fa164,0x30,'5d74376c499c9d81b71e66ca1b467c333f3d2192ce5f55c57e3ae6e3d7812bf2'),
    'BattleActionCommandManagerInit':(0x800fa62c,0x4c,'656299e101396896e82d2a4479ae029ea5eb3015202f9b0273154cae9d90d3db'),
    'BattlePadInit':(0x8011a48c,0x4c,'989faa72399b2bf64d5a7d511dfd5b74e0a274948e742aa4c71532160a395626'),
    'marioStSystemLevel':(0x8006f7c8,0x114,'d8f6eed8b820d321ef48c40c6ecc1cde3e43bfbad25df6f8ca7e44dbff23ca8b'),
    'marioStGetSystemLevel':(0x8006f7bc,0xc,'6ab95a82dba571326506cd6230377a7db2a818ae8982feb7374fafa113403dcb'),
    'evtSetSpeed':(0x800365ec,0x10,'5144f5c22dafbc452594da5a77473a23375f80786ce3461bd4872e1fcfe56ef6'),
}
ANCHORS = {
    0x8006f7bc:'806d90b0',0x8006f7c0:'80630018',
    0x8006f800:'906d1a20',0x8006f804:'386000ef',0x8006f810:'54000036',
    0x8006f824:'38600001',0x8006f830:'540007f6',0x8006f840:'60000001',
    0x8006f854:'38600002',0x8006f860:'540007b6',0x8006f870:'60000003',
    0x8006f884:'38600010',0x8006f890:'54000776',0x8006f8a0:'60000007',
    0x8006f8b4:'386000ef',0x8006f8c0:'6000000f',
    0x800365ec:'c00d82cc',0x800365f0:'ec010032',0x800365f4:'d0030164',
    0x800327f0:'4bffe051',0x800327f8:'48003df5',0x800327fc:'3bc00002',
    0x80037590:'987e000c',0x80037598:'c02d82cc',0x8003759c:'d03e0164',
    0x800375a0:'d01e0168',0x80036d08:'c02d82cc',0x80036d0c:'d03e0164',
    0x80037004:'c02d82cc',0x80037008:'d03e0164',
}


def collect(iso,symbols):
    rows=list(csv.DictReader(Path(symbols).open()))
    by_start={}
    for row in rows:
        if row['area']=='_main' and row['sec_type']=='text' and row['ram_addr'] and int(row['size'],16)>0:
            by_start.setdefault(int(row['ram_addr'],16),[]).append(row)
    starts=sorted(by_start)
    def owner(site):
        i=bisect_right(starts,site)-1
        if i<0:return None
        candidates=[r for r in by_start[starts[i]] if site<int(r['ram_addr'],16)+int(r['size'],16)]
        return [r['name']+'@'+r['namespace'] for r in candidates] or None
    usage=audit(iso,symbols)
    selected={r['address']:dict(r,direct_references=[]) for r in usage['functions'] if r['name'] in TARGETS}
    if len(selected)!=len(TARGETS):raise ValueError('Missing/ambiguous target symbols')
    speed_stores=[]
    with Path(iso).open('rb') as disc:
        if disc.read(8)!=b'G8ME01\0\0':raise ValueError('US disc0 revision0 required')
        disc.seek(0x420);dol_base=struct.unpack('>I',disc.read(4))[0]
        disc.seek(dol_base);header=disc.read(256)
        def read(address,size):
            positions=[]
            for i in range(18):
                off,va,length=(struct.unpack_from('>I',header,j+i*4)[0] for j in (0,0x48,0x90))
                if va<=address and address+size<=va+length:positions.append(dol_base+off+address-va)
            if len(positions)!=1:raise ValueError(f'Unmapped/ambiguous address {address:#x}')
            disc.seek(positions[0]);data=disc.read(size)
            if len(data)!=size:raise ValueError('Truncated binary')
            return data
        for name,(address,size,digest) in BODIES.items():
            if hashlib.sha256(read(address,size)).hexdigest()!=digest:raise ValueError(f'Body mismatch:{name}')
        for address,expected in ANCHORS.items():
            if read(address,4).hex()!=expected:raise ValueError(f'Anchor mismatch:{address:#x}')
        if read(0x804151ec,4)!=struct.pack('>f',1.0):raise ValueError('Initial event-speed multiplier mismatch')
        if struct.unpack('>I',read(0x80304fd0+98*4,4))[0]!=0x800327e4:raise ValueError('Speed opcode98 target mismatch')
        for i in range(7):
            off,va,length=(struct.unpack_from('>I',header,j+i*4)[0] for j in (0,0x48,0x90))
            if not length:continue
            data=read(va,length)
            for pos in range(0,len(data)-3,4):
                word=struct.unpack_from('>I',data,pos)[0];site=va+pos
                if word>>26==18:
                    delta=word&0x03fffffc
                    if delta&0x02000000:delta-=0x04000000
                    target=(delta if word&2 else site+delta)&0xffffffff
                    if target in selected:selected[target]['direct_references'].append(
                        dict(site=site,link=bool(word&1),owner=owner(site)))
                # Only direct r13-relative single stores, not indexed/pointer/bulk writes.
                if word&0xfc1fffff==0xd00d82cc:
                    speed_stores.append(dict(site=site,owner=owner(site)))
    for record in selected.values():
        if sum(r['link'] for r in record['direct_references'])!=record['direct_link_sites'] or sum(not r['link'] for r in record['direct_references'])!=record['direct_nonlink_sites']:
            raise ValueError('Direct reference census disagrees with usage audit')
    info=inventory(iso)
    relevant_modules={imp['module'] for record in selected.values() for imp in record['rel_imports']}
    relevant_modules.add('rel/gor.rel')
    module_records={}
    with Path(iso).open('rb') as disc:
        for path in sorted(relevant_modules):
            entry=next(f for f in info['files'] if f['path']==path)
            disc.seek(entry['offset']);raw=disc.read(entry['size'])
            if len(raw)!=entry['size']:raise ValueError('Truncated REL')
            table=struct.unpack_from('>I',raw,0x10)[0]
            area=Path(path).stem
            for record in selected.values():
                for imp in record['rel_imports']:
                    if imp['module']!=path:continue
                    section_off,section_size=struct.unpack_from('>II',raw,table+imp['section']*8)
                    section_off &= ~3
                    if not section_off or not 0<=imp['offset']<section_size:
                        imp['file_offset']=None;imp['owner']=None;continue
                    site=section_off+imp['offset'];imp['file_offset']=site
                    imp['owner']=[r['name']+'@'+r['namespace'] for r in rows if r['area']==area and
                                  r['file_addr'] and int(r['file_addr'],16)<=site<int(r['file_addr'],16)+int(r['size'],16)] or None
            module_records[path]=dict(size=entry['size'],sha256=hashlib.sha256(raw).hexdigest())
            if path=='rel/gor.rel':
                if hashlib.sha256(raw[0x11784:0x117d8]).hexdigest()!='ef58d190898204409380f0c4e2eb7e14b2093b464c56aa117b526e9a998dc791':
                    raise ValueError('Tutorial key record helper mismatch')
                if hashlib.sha256(raw[0x67680:0x67f68]).hexdigest()!='5d1b151dce5e580660d7c60201a78a9c3429f6631a1426fd664d3e7808867282':
                    raise ValueError('Tutorial demonstration attack event mismatch')
                local,imports,_=read_bindings(raw)
                if imports.get(0x67c48)!=0x80107694 or imports.get(0x67c58)!=0x8010a78c:
                    raise ValueError('Tutorial defense/damage call bindings mismatch')
                for offset,words in ((0x67c10,(0x10022,0xfe363c80)),(0x67c18,(0x10024,3)),
                                     (0x67c2c,(0x10024,7)),(0x67c40,(0x31,))):
                    if struct.unpack_from('>'+str(len(words))+'I',raw,offset)!=words:
                        raise ValueError('Tutorial injection case graph mismatch')
                helper_bindings={}
                imp_off,imp_size=struct.unpack_from('>II',raw,0x28)
                for imp_site in range(imp_off,imp_off+imp_size,8):
                    module,stream=struct.unpack_from('>II',raw,imp_site)
                    if module:continue
                    section,offset=None,0
                    for _ in range(len(raw)//8):
                        delta,kind,target,addend=struct.unpack_from('>HBBI',raw,stream);stream+=8
                        if kind==203:break
                        if kind==202:section,offset=target,0;continue
                        offset+=delta
                        section_off=struct.unpack_from('>I',raw,table+section*8)[0]&~3
                        site=section_off+offset
                        if 0x11784<=site<0x117d8:helper_bindings[site]=(kind,addend)
                    else:raise ValueError('Unterminated module0 relocation stream')
                if helper_bindings!={0x1179e:(6,0x8041eb90),0x117a2:(4,0x8041eb90),0x117b8:(10,0x8011a38c)}:
                    raise ValueError('Tutorial helper global/pad reset relocation mismatch')
                helper_sites=sorted(site for site,binding in local.items() if binding['file_offset']==0x11784)
                if helper_sites!=[0x67c24,0x67c38]:raise ValueError('Tutorial key helper bindings mismatch')
                for site,mask in zip(helper_sites,(0x100,0x200)):
                    if struct.unpack_from('>I',raw,site-4)[0]!=0x2005b or struct.unpack_from('>I',raw,site+4)[0]!=mask:
                        raise ValueError('Tutorial key helper call/mask mismatch')
    return dict(schema=1,disc_id='G8ME01',revision=0,dol_sha256=usage['dol_sha256'],
                symbols_sha256=usage['symbols_sha256'],
                scope='Static DOL branches/data candidates and29 shipped REL module0 import streams',
                limitations='No executed reachability,branch feasibility,indirect/computed calls,complete '
                            'pause graph or all multiplier writers. REL imports need caller relocation/argument recovery.',
                body_checks=len(BODIES),instruction_checks=len(ANCHORS),data_checks=2,
                modules=module_records,tutorial_helper=dict(file_offset=0x11784,size=0x54,
                sha256='ef58d190898204409380f0c4e2eb7e14b2093b464c56aa117b526e9a998dc791',
                user_function_sites=[0x67c24,0x67c38],raw_masks=[0x100,0x200]),
                tutorial_event=dict(file_offset=0x67680,size=0x8e8,commands=162,
                sha256='5d1b151dce5e580660d7c60201a78a9c3429f6631a1426fd664d3e7808867282'),
                functions=list(selected.values()),direct_r13_speed_stores=speed_stores)


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--symbols',required=True)
    p.add_argument('--output',required=True,type=Path)
    args=p.parse_args()
    if args.output.resolve() in (Path(args.iso).resolve(),Path(args.symbols).resolve()):p.error('Cannot overwrite source')
    result=collect(args.iso,args.symbols)
    args.output.parent.mkdir(parents=True,exist_ok=True)
    args.output.write_text(json.dumps(result,indent=2)+'\n')
    for r in result['functions']:
        print(f"{r['name']}: {len(r['direct_references'])} direct references,{len(r['rel_imports'])} REL imports")
    print(f'Verified {len(BODIES)} DOL bodies,{len(ANCHORS)} instructions,2 data bindings,'
          '1 REL helper and selected demonstration script bindings')


if __name__=='__main__':main()
