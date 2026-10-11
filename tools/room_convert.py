"""Read original REL relocations and identify reciprocal Rogueport road exits.
BeroEntry layout reference: jdaster64/ttyd-tot, ttyd/evt_bero.h.
No original executable code is run or copied into the runtime.
"""
import struct
import math
from pathlib import Path

def relocations(path):
    raw=Path(path).read_bytes()
    def u(o):return struct.unpack_from('>I',raw,o)[0]
    sections=[(u(u(16)+n*8)&~3,u(u(16)+n*8+4)) for n in range(u(12))]
    result={};imports={}
    for n in range(u(44)//8):
        module,stream=struct.unpack_from('>II',raw,u(40)+n*8);section=0;offset=0
        while True:
            delta,kind,target,addend=struct.unpack_from('>HBBI',raw,stream);stream+=8
            if kind==203:break
            if kind==202:section=target;offset=0;continue
            offset+=delta
            if kind==1 and module==u(0):result[sections[section][0]+offset]=sections[target][0]+addend
            if kind==1 and module==0:imports[sections[section][0]+offset]=addend
    return raw,result,imports

def pointers(path):
    raw,local,_=relocations(path)
    return raw,local

def damage_returns(path):
    """Read US evt_hit_damage_return_set tables; retain activation call sites.

    Does not execute events or resolve the original -1000 Y sentinel.
    Function address/layout verified from the original US DOL.
    """
    raw,local,imports=relocations(path);tables=[]
    for site,address in sorted(imports.items()):
        if address!=0x8007458c:continue
        if site<4 or struct.unpack_from('>I',raw,site-4)[0]!=0x0002005b:
            raise ValueError('Damage return function reference is not a two-argument event call')
        start=local.get(site+4)
        if start is None:raise ValueError('Damage return table argument lacks local relocation')
        entries=[]
        for n in range(4096):
            offset=start+n*16
            if offset+16>len(raw):raise ValueError('Truncated damage return table')
            name_pointer=local.get(offset)
            if name_pointer is None:
                if struct.unpack_from('>I',raw,offset)[0]!=0:raise ValueError('Unrelocated damage return node name')
                break
            end=raw.find(b'\0',name_pointer)
            if end<name_pointer:raise ValueError('Unterminated damage return node name')
            name=raw[name_pointer:end].decode('ascii')
            if not name or any(ord(c)<32 or ord(c)>=127 for c in name):raise ValueError('Invalid damage return node name')
            point=list(struct.unpack_from('>3f',raw,offset+4))
            if not all(math.isfinite(v) for v in point):raise ValueError('Nonfinite damage return position')
            entries.append({'offset':offset,'node':name,'position':point,'automatic_position':point[1]==-1000.0})
        else:raise ValueError('Damage return table lacks bounded terminator')
        tables.append({'call_offset':site-4,'table_offset':start,'entries':entries})
    return tables

def exits(path):
    raw,ptr=pointers(path)
    def string(o):
        if not o:return ''
        try:return raw[o:raw.index(b'\0',o)].decode('ascii')
        except (ValueError,UnicodeError):return ''
    entries=[]
    for site,to in sorted(ptr.items()):
        name=string(to);target=string(ptr.get(site+40,0));arrival=string(ptr.get(site+44,0))
        if name in ('n_bero','s_bero','e_bero','w_bero') and target.startswith('gor_') and arrival.endswith('_bero'):
            kind,sfx,direction,x,y,z,length=struct.unpack_from('>hh5i',raw,site+4)
            if kind==0:entries.append({'offset':site,'name':name,'target':target,'arrival':arrival,'direction':direction,'center':[x,y,z],'length':length})
    return entries
