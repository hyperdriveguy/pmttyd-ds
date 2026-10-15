#!/usr/bin/env python3
"""Read-only complete window8007b44c source and retained FIFO census."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
from iso_assets import DEFAULT_ISO
ADDRESS=0x8007b44c
SIZE=0x68c
# Whole original function bytes, not a convenience partial used-path listing.
SHA='fe1dc4fcff76fed45e6d185b88cc128b9ea57b31167cff0b7c10d22a29a18c02'

def verify(iso=DEFAULT_ISO,source=None):
    with Path(iso).open('rb') as f:
        if f.read(6)!=b'G8ME01':raise ValueError('US original disc required')
        f.seek(0x420);dol,=struct.unpack('>I',f.read(4));f.seek(dol);h=f.read(256)
        def read(a,n):
            offsets=[]
            for i in range(18):
                off,base,size=[struct.unpack_from('>I',h,k+4*i)[0] for k in (0,0x48,0x90)]
                if base<=a and a+n<=base+size:offsets.append(dol+off+a-base)
            if len(offsets)!=1:raise ValueError('Original DOL domain unavailable')
            f.seek(offsets[0]);return f.read(n)
        body=read(ADDRESS,SIZE)
        if hashlib.sha256(body).hexdigest()!=SHA:raise ValueError('Whole original window body differs')
        constants=[struct.unpack('>I',read(a,4))[0] for a in (0x80420538,0x80420540,0x80420544,0x80420548)]
        if constants!=[0,0x3eaaaaab,0x3f2aaaab,0x3f800000]:raise ValueError('Original nine-slice constants differ')
    source=Path(source or Path(__file__).resolve().parents[1]/'source/display/window_primitive_continuation.cpp').read_text()
    calls=[];fifo=0;loads=0;immediates={}
    for offset in range(0,SIZE,4):
        a=ADDRESS+offset;op,=struct.unpack_from('>I',body,offset)
        if op>>26==14 and (op>>16)&31==0:
            immediate=op&0xffff
            if immediate&0x8000:immediate-=0x10000
            immediates[(op>>21)&31]=immediate
        if op>>26==18 and op&3==1:
            d=op&0x03fffffc
            if d&0x02000000:d-=0x04000000
            target=(a+d)&0xffffffff;calls.append(dict(site=hex(a),target=hex(target)))
            if target in (0x8029dd04,0x8029dd64):needle=f'math(0x{target:08x})'
            else:needle=f'call(0x{target:08x},0x{a:08x}'
            if needle not in source:raise ValueError('Missing original retained child site '+hex(a))
            match=re.search(re.escape(f'call(0x{target:08x},0x{a:08x}')+r',\{([0-9,]+)\}',source)
            if match:
                values=[int(n) for n in match.group(1).split(',')]
                start=4 if target==0x802ba2fc else 3
                if any(immediates.get(start+i)!=v for i,v in enumerate(values)):
                    raise ValueError('Original immediate GX operands differ '+hex(a))
            immediates={k:v for k,v in immediates.items() if k>=14}
        if 0x8007b6b4<=a<=0x8007ba90:
            prefix=f'case 0x{a:08x}:'
            if source.count(prefix)!=1:raise ValueError('Missing/duplicate FIFO source PC')
            r=(op>>21)&31;ra=(op>>16)&31;d=op&0xffff
            if d&0x8000:d-=0x10000
            if op>>26==52:
                fifo+=1;needle=f'emit(0x{a:08x},c.f[{r}])'
            elif op>>26==48:
                loads+=1;needle=f'read(0x{0x80427440+d:08x},c.bits))return false;c.f[{r}]=scalar(c.bits)'
            elif op>>26==59:
                rb=(op>>11)&31;xo=(op>>1)&31;sign={21:'+',20:'-'}.get(xo)
                if sign is None:raise ValueError('Unexpected original float arithmetic')
                needle=f'c.f[{r}]=c.f[{ra}]{sign}c.f[{rb}]'
            else:raise ValueError('Unexpected original FIFO instruction')
            line=next(line for line in source.splitlines() if prefix in line)
            if needle not in line:raise ValueError('Retained FIFO source instruction mismatch '+hex(a))
    if fifo!=180:raise ValueError('Original FIFO census differs')
    return dict(address=hex(ADDRESS),size=SIZE,sha256=SHA,children=calls,fifo_floats=fifo,fresh_FIFO_constant_loads=loads,
        limitation='Static original source/retention pins; no native GPU raster equality claim')
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);a=p.parse_args();print(json.dumps(verify(a.iso),indent=2))
