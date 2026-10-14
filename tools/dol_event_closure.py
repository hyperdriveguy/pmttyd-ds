#!/usr/bin/env python3
"""Unchanged linked DOL EVT objects referenced by an original REL data closure.

This follows references to known EVT objects only, not PPC functions, arbitrary
mutable globals, or a complete world-service/data dependency closure.
"""
import bisect,csv,hashlib,struct
from pathlib import Path
from rel_event_inventory import read_bindings

def linked_events(iso,raw,objects,symbols):
    with Path(symbols).open() as f:
        rows=sorted([r for r in csv.DictReader(f) if r['area']=='_main' and r['type']=='evt' and r['sec_type']=='data'],key=lambda r:int(r['ram_addr'],16))
    starts=[int(r['ram_addr'],16) for r in rows]
    def event(address):
        n=bisect.bisect_right(starts,address)-1
        return n if n>=0 and starts[n]<=address<starts[n]+int(rows[n]['size'],16) else None
    _,imports,_=read_bindings(raw);pending=set()
    spans=[(o['file_offset'],o['file_offset']+o['size']) for o in objects]
    for site,address in imports.items():
        if any(a<=site<b for a,b in spans):
            n=event(address)
            if n is not None:pending.add(n)
    selected={}
    with Path(iso).open('rb') as disc:
        if disc.read(8)!=b'G8ME01\0\0':raise ValueError('Original US disc0 revision0 required')
        disc.seek(0x420);dol=struct.unpack('>I',disc.read(4))[0];disc.seek(dol);header=disc.read(256)
        def read(address,size):
            found=[]
            for n in range(18):
                off,va,length=(struct.unpack_from('>I',header,b+4*n)[0] for b in (0,0x48,0x90))
                if va<=address and address+size<=va+length:found.append(off+address-va)
            if len(found)!=1:raise ValueError('DOL event/context crosses loaded section')
            disc.seek(dol+found[0]);blob=disc.read(size)
            if len(blob)!=size:raise ValueError('Truncated DOL event')
            return blob
        while pending:
            n=min(pending);pending.remove(n)
            if n in selected:continue
            row=rows[n];size=int(row['size'],16)
            if size%4 or size<4:raise ValueError('Unaligned original EVT')
            blob=read(starts[n],size+4);selected[n]=blob
            # Only event-body references participate; adjacent scanner context
            # does not enlarge the object's semantics or reference graph.
            for address, in struct.iter_unpack('>I',blob[:size]):
                target=event(address)
                if target is not None and target not in selected:pending.add(target)
    return [(starts[n],blob) for n,blob in sorted(selected.items())],[dict(name=rows[n]['name'],address=starts[n],size=int(rows[n]['size'],16),sha256=hashlib.sha256(blob[:-4]).hexdigest(),prefetch_address=starts[n]+len(blob)-4,prefetch_word=struct.unpack('>I',blob[-4:])[0]) for n,blob in sorted(selected.items())]

def merge_segments(segments):
    """Deduplicate overlapping original words only when every byte agrees."""
    merged=[]
    for address,blob in sorted(segments):
        if not merged or address>merged[-1][0]+len(merged[-1][1]):merged.append((address,bytearray(blob)));continue
        start,data=merged[-1];offset=address-start;overlap=min(len(blob),len(data)-offset)
        if data[offset:offset+overlap]!=blob[:overlap]:raise ValueError('Conflicting linked original data')
        data.extend(blob[overlap:])
    offset=12+12*len(merged);table=bytearray();payload=bytearray()
    for address,blob in merged:table+=struct.pack('>III',address,len(blob),offset);payload+=blob;offset+=len(blob)
    return b'PMEVT001'+struct.pack('>I',len(merged))+table+payload
