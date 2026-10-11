"""Read original REL relocations and identify reciprocal Rogueport road exits.
BeroEntry layout reference: jdaster64/ttyd-tot, ttyd/evt_bero.h.
No original executable code is run or copied into the runtime.
"""
import struct
from pathlib import Path

def pointers(path):
    raw=Path(path).read_bytes()
    def u(o):return struct.unpack_from('>I',raw,o)[0]
    sections=[(u(u(16)+n*8)&~3,u(u(16)+n*8+4)) for n in range(u(12))]
    result={}
    for n in range(u(44)//8):
        module,stream=struct.unpack_from('>II',raw,u(40)+n*8);section=0;offset=0
        while True:
            delta,kind,target,addend=struct.unpack_from('>HBBI',raw,stream);stream+=8
            if kind==203:break
            if kind==202:section=target;offset=0;continue
            offset+=delta
            if kind==1 and module==u(0):result[sections[section][0]+offset]=sections[target][0]+addend
    return raw,result

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
