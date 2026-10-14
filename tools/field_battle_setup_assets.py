#!/usr/bin/env python3
"""Finite original field-side battle setup views; no complete battle export."""
import csv,hashlib,struct
from pathlib import Path
from rel_data_pack import bounded_segments

GOR_SHA256='d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a'

def gor_setup_zero_segments(iso,raw,symbols):
    """Exactly the gor row0 graph dereferenced by npcSetupBattleInfo.

    Stage/music/group auxiliary pointers retain original relocated identity;
    their later consumers must separately admit resources and data.
    """
    if hashlib.sha256(raw).hexdigest()!=GOR_SHA256:raise ValueError('Unexpected original US gor REL')
    with Path(symbols).open() as source:rows=list(csv.DictReader(source))
    views=[(0x60e30,0x44,'battle_database_gor.o','btlsetup_gor_tbl'),
        (0x60da0,0x18,'battle_database_gor.o','btlgrpsel_gor_btlno_gor_test1_off'),
        (0x60d80,0x20,'battle_database_gor.o','btlgrp_gor_gor_02_01_off_1'),
        (0x60d50,0x30,'battle_database_gor.o','btlparty_gor_gor_02_01_off_1'),
        (0x61c40,4,'unit_monban.o','unit_monban'),
        (0x1dbc8,0x10,'battle_database_gor.o','str_マップstg_00_0_gor_0001dbc8'),
        (0x1dbd8,0x14,'battle_database_gor.o','str_BGM_ZAKO_BATTLE1_gor_0001dbd8')]
    for begin,size,namespace,name in views:
        found=[r for r in rows if r['area']=='gor' and r['namespace']==namespace and r['name']==name and r['sec_type']=='data']
        if len(found)!=1:raise ValueError('Missing unique battle setup view owner: '+name)
        start,length=(int(found[0][k],16) for k in ('file_addr','size'))
        # String views include only source alignment padding beyond the symbol.
        limit=(start+length+3)&~3
        if not start<=begin<begin+size<=limit:raise ValueError('Battle setup view exceeds source owner: '+name)
    segments,meta=bounded_segments(raw,[(a,n) for a,n,_,_ in views])
    for row,(_,_,namespace,name) in zip(meta,views):row.update(namespace=namespace,name=name)
    def data(address):return next(blob for a,blob in segments if a==address)
    def words(address):return struct.unpack('>'+str(len(data(address))//4)+'I',data(address))
    configuration=words(0x90540da8);selection=words(0x90540d18);group=words(0x90540cf8);unit=words(0x90540cc8)
    if configuration[:8]!=(0x9040a0c8,0,0,0xffffffff,0,0x90540d18,0,0) or configuration[-1]!=0x9040a0d8 or data(0x90540da8)[0x20:0x40]!=bytes([1,1])*16:raise ValueError('Unexpected original gor setup0 configuration')
    if selection!=(50,0x90540cf8,0x90540870,0,0,0) or group[:2]!=(1,0x90540cc8) or unit[0]!=0x90541ba8 or unit[-1]!=0x80417880 or data(0x90541bb8)[2]!=0:raise ValueError('Unexpected original gor setup0 group/unit graph')
    with Path(iso).open('rb') as disc:
        if disc.read(8)!=b'G8ME01\0\0':raise ValueError('Original US disc0 revision0 required')
        disc.seek(0x420);dol=struct.unpack('>I',disc.read(4))[0];disc.seek(dol);header=disc.read(256)
        found=[]
        for i in range(18):
            off,va,size=(struct.unpack_from('>I',header,b+4*i)[0] for b in (0,0x48,0x90))
            if va<=0x80417880 and 0x80417888<=va+size:found.append(off+0x80417880-va)
        if len(found)!=1:raise ValueError('Missing original monban item table')
        disc.seek(dol+found[0]);items=disc.read(8)
    if items!=bytes(8):raise ValueError('Unexpected original monban item table')
    segments.append((0x80417880,items))
    return segments,dict(area='gor',numeric_indices=[0],configuration=0x90540da8,
        rel_views=meta,dol_views=[dict(address=0x80417880,size=8,sha256=hashlib.sha256(items).hexdigest())],
        stage=0x90540870,group=0x90540cf8,unit=0x90540cc8,kind=0x90541ba8,
        source='rel/gor.rel',source_sha256=GOR_SHA256,
        policy='Finite npcSetupBattleInfo row0 reads only. Stage and auxiliary group/kind/resource pointers retain identity; no complete battle/resource closure or mutable area registration.')
