#!/usr/bin/env python3
"""Verify original US Voyage callbacks read-only; never execute original code."""
import argparse,hashlib,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
HASHES={'logo': (15252, 844, 'd916dba0902c9dc728f6ed05e8daf7ae75ab468b91710eafed8efb3f83fe2456'), 'draw': (16096, 496, '54c6756686249b83a590d7e5e4ed630594f0955781d525e449b02714bb0e698f'), 'pressStartGX': (16592, 1240, 'a0fe91f90d153a71a0d8209da4285d52e447e42e191f6e4743902b5705504fff'), 'unk_muj_000045a8': (17832, 488, 'aafe1568f4af5482dcabe6ae6819ab7a772b7e202bcde161a1aa8815f9990142'), 'anm_yure': (211112, 528, '85e56968dc13a7a357ab3a45b13b8f96f95e94debf2c87df5ae790303c3c1b9f'), 'fune_eff': (18320, 672, '9d8071e51bdd9d6bbba2d7688de49dd4f10e45677171632de3b895f01b7b1ab2'), 'muj_20_init_evt': (213400, 1448, '131ac2bc41c846b6d8ea0c57d026eccde545dd4d80927307109c0bf4f03b31b2')}
def verify(iso=DEFAULT_ISO):
    info=inventory(iso)
    if info['disc_id']!='G8ME01':raise ValueError('Original US G8ME01 required')
    item=next(f for f in info['files'] if f['path']=='rel/muj.rel')
    with Path(iso).open('rb') as disc:disc.seek(item['offset']);raw=disc.read(item['size'])
    for name,(offset,size,digest) in HASHES.items():
        if hashlib.sha256(raw[offset:offset+size]).hexdigest()!=digest:raise ValueError('Original '+name+' changed')
    # Source init seeds original global WORD point before child animations.
    seeds=[(0x34674,-50000000,-230000000-2000*1024),(0x34680,-49999999,-230000000),(0x3468c,-49999998,-230000000-3000*1024)]
    for offset,destination,value in seeds:
        if raw[offset:offset+12]!=struct.pack('>3I',0x00020034,destination&0xffffffff,value&0xffffffff):raise ValueError('Original Voyage GW seed changed')
    # The complete original relocation stream does not register/call the
    # otherwise retained pressStartGX function (text section1+0x400c).
    u=lambda off:struct.unpack_from('>I',raw,off)[0]
    refs=[]
    for off in range(u(0x28),u(0x28)+u(0x2c),8):
        module,stream=struct.unpack_from('>II',raw,off)
        while True:
            _,kind,target,addend=struct.unpack_from('>HBBI',raw,stream);stream+=8
            if kind==203:break
            if kind not in (202,201) and module==u(0) and target==1 and addend==0x400c:refs.append(kind)
    if refs:raise ValueError('Previously uninvoked Voyage prompt now has relocation references')
    text_table=u(0x10)+8
    text_offset=u(text_table)&~3;text_size=u(text_table+4)
    for offset in range(0,text_size,4):
        opcode=u(text_offset+offset)
        if opcode&0xfc000002!=0x48000000:continue
        displacement=opcode&0x03fffffc
        if displacement&0x02000000:displacement-=0x04000000
        if offset+displacement==0x400c:raise ValueError('Previously uninvoked Voyage prompt now has a direct branch')
    # Bounded callback rodata: original language palettes21,21,22,23,24,25,
    # rotationzero/scalefactorone and whiteRGB with runtime alpha.
    if raw[0x11e10:0x11e10+24]!=struct.pack('>6I',21,21,22,23,24,25):raise ValueError('Original Voyage logo language table changed')
    base=0x805baa64-0xc4
    constants={0x805cc980:0.,0x805cc984:255.,0x805cc988:-50.,0x805cc98c:100.,0x805cc9b0:5.,0x805cc9b4:-6000.,0x805cc9b8:0.01745329238474369,0x805cc9bc:6000.}
    for address,value in constants.items():
        if raw[address-base:address-base+4]!=struct.pack('>f',value):raise ValueError('Original Voyage callback constant changed')
    return len(HASHES)+3+len(constants)+len(seeds)
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);a=p.parse_args();print(f'{verify(a.iso)} original Voyage callback anchors verified read-only')
