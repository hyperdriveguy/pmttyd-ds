#!/usr/bin/env python3
"""Scoped original REL data closure, retaining ADDR32 bindings and excluding code.

Addresses use an explicit native section map. Nothing executes original PPC.
This supports new areas without packaging their executable sections or inventing
script words. The supplied symbol map defines finite object/event boundaries.
"""
import csv,hashlib,struct
from pathlib import Path
from rel_event_inventory import read_bindings

def package(raw,symbols,module,roots):
    u=lambda offset:struct.unpack_from('>I',raw,offset)[0]
    sections=[]
    for i in range(u(12)):
        offset,size=struct.unpack_from('>II',raw,u(16)+i*8)
        sections.append((offset&~3,size,0x90000000+i*0x100000,bool(offset&1)))
    with Path(symbols).open() as source:
        rows=[r for r in csv.DictReader(source) if r['area']==module and r['file_addr'] and r['size']]
    by_offset={}
    for row in rows:by_offset.setdefault(int(row['file_addr'],16),[]).append(row)
    def address(offset):
        found=[base+offset-start for start,size,base,_ in sections if start and start<=offset<start+size]
        if len(found)!=1:raise ValueError('Ambiguous REL address')
        return found[0]
    local,imports,foreign=read_bindings(raw)
    selected={};pending=[]
    def add(row):
        start,size=int(row['file_addr'],16),int(row['size'],16)
        owners=[s for s in sections if s[0] and s[0]<=start and start+size<=s[0]+s[1]]
        if len(owners)!=1 or owners[0][3] or row['sec_type']!='data':raise ValueError('Scoped pack cannot include executable code')
        if start not in selected:selected[start]=row;pending.append(row)
    for name in roots:
        matches=[r for r in rows if r['name']==name]
        if len(matches)!=1:raise ValueError('Choose one exact original data root: '+name)
        add(matches[0])
    while pending:
        row=pending.pop();start=int(row['file_addr'],16);size=int(row['size'],16)
        for site,binding in local.items():
            if not start<=site<start+size:continue
            target=binding['file_offset']
            if target is None or sections[binding['section']][3]:continue
            matches=by_offset.get(target,[])
            if not matches or len({(r['size'],r['type']) for r in matches})!=1:
                raise ValueError('Unbounded/ambiguous REL data dependency: '+hex(target))
            add(matches[0])
        if any(start<=site<start+size for site in foreign):raise ValueError('Unbound foreign REL data dependency')
    intervals=[];objects=[]
    for start,row in sorted(selected.items()):
        size=int(row['size'],16);end=(start+size+(4 if row['type']=='evt' else 0)+3)&~3;begin=start&~3
        owners=[s for s in sections if s[0] and s[0]<=begin and end<=s[0]+s[1] and not s[3]]
        if len(owners)!=1:raise ValueError('Data closure crosses section boundary')
        if intervals and begin<=intervals[-1][1]:intervals[-1]=(intervals[-1][0],max(end,intervals[-1][1]))
        else:intervals.append((begin,end))
        objects.append(dict(name=row['name'],type=row['type'],file_offset=start,size=size,address=address(start),sha256=hashlib.sha256(raw[start:start+size]).hexdigest()))
    segments=[]
    for begin,end in intervals:
        data=bytearray(raw[begin:end])
        for site,binding in local.items():
            if begin<=site<end:
                struct.pack_into('>I',data,site-begin,sections[binding['section']][2]+binding['addend'])
        for site,value in imports.items():
            if begin<=site<end:struct.pack_into('>I',data,site-begin,value)
        segments.append((address(begin),bytes(data),begin))
    offset=12+12*len(segments);table=bytearray();payload=bytearray()
    for base,data,_ in segments:table+=struct.pack('>III',base,len(data),offset);payload+=data;offset+=len(data)
    return b'PMEVT001'+struct.pack('>I',len(segments))+table+payload,dict(objects=objects,
        sections=[dict(file_offset=s,size=n,address=a,executable=x) for s,n,a,x in sections],
        segments=[dict(address=a,size=len(d),file_offset=f,sha256=hashlib.sha256(d).hexdigest()) for a,d,f in segments])
