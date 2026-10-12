#!/usr/bin/env python3
"""Verify finite source air-shadow graph; export geometry only to ignored paths."""
import argparse,json,math,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
from verify_overworld_roads import verify
MANIFEST=Path(__file__).with_name('overworld_mario_air_shadow_evidence.json')
def dol_bytes(iso,address,size):
    with Path(iso).open('rb') as d:
        d.seek(0x420);dol=struct.unpack('>I',d.read(4))[0];d.seek(dol);h=d.read(256)
        spans=[]
        for i in range(18):
            off,va,n=(struct.unpack_from('>I',h,k+4*i)[0] for k in (0,0x48,0x90))
            if va<=address and address+size<=va+n:spans.append(dol+off+address-va)
        if len(spans)!=1:raise ValueError('Unmapped/ambiguous source span')
        d.seek(spans[0]);raw=d.read(size)
        if len(raw)!=size:raise ValueError('Truncated source')
        return raw

def geometry(iso=DEFAULT_ISO):
    """Original in-memory60XYZ, five indexed primitives, two RGBA palettes."""
    raw=dol_bytes(iso,0x80304ce0,720);v=struct.unpack('>180f',raw)
    vertices=[list(v[i:i+3]) for i in range(0,180,3)];lists=[]
    for name,a in [('top',0x80304b60),('bottom',0x80304ba0),('side',0x80304be0),('top_inside',0x80304c20),('top_outside',0x80304c60)]:
        raw=dol_bytes(iso,a,64);count=struct.unpack_from('>H',raw,1)[0]
        if 3+count*2>64 or raw[0] not in (0x98,0xa0):raise ValueError('Unsupported original primitive')
        pairs=[list(raw[3+2*i:5+2*i]) for i in range(count)]
        if any(p>=60 or c>=3 for p,c in pairs):raise ValueError('Indexed primitive outside arrays')
        if any(raw[3+count*2:]):raise ValueError('Unexpected non-NOP display-list tail')
        lists.append(dict(name=name,address=a,primitive=raw[0],pairs=pairs))
    palettes={name:[list(raw[i:i+4]) for i in (0,4,8)] for name,raw in [('ordinary',dol_bytes(iso,0x80415140,12)),('no_decay',dol_bytes(iso,0x80415160,12))]}
    return dict(source='US G8ME01 revision0 DOL; original local asset, no redistribution',vertices=vertices,lists=lists,palettes=palettes)

def check_graph(iso,e):
    for call in e['gx_calls']:
        a=call['address'];w=int.from_bytes(dol_bytes(iso,a,4),'big');delta=w&0x3fffffc
        if delta&0x2000000:delta-=0x4000000
        assert w>>26==18 and w&1 and a+delta==call['target']
        regs={};block=dol_bytes(iso,a-128,128)
        for i in range(0,128,4):
            word=int.from_bytes(block[i:i+4],'big')
            if word>>26==18 and word&1:regs={}
            if word>>26==14 and ((word>>16)&31)==0:
                val=word&65535;regs[(word>>21)&31]=val-65536 if val&32768 else val
        assert [regs.get(i) for i in range(3,3+len(call['args']))]==call['args']
    ranges=struct.unpack('>8I',dol_bytes(iso,0x800055b0,32))
    assert any(start<=0x80429540 and 0x80429548<=start+length for start,length in zip(ranges[::2],ranges[1::2]))
    return len(e['gx_calls'])

def f32(x):return struct.unpack('>f',struct.pack('>f',x))[0]
def plan(flags,global_alpha=110,opacity=1,inner_opacity=.2,decay=.4):
    if flags&4:return ['top','bottom','side']
    a=f32(f32(global_alpha*opacity)/255);b=f32(a*inner_opacity);inner=max(0,min(1,f32(1-decay)))
    if a<=0:return []
    if b<=0 or b>=1:b=0
    byte=math.trunc(f32(255*b))&255
    return (['top_inside'] if inner!=0 else [])+(['top_outside'] if inner!=1 and byte!=0 else [])+['bottom','side']
def derived(geo):
    v=geo['vertices'];assert len(v)==60 and all(math.isfinite(x) for row in v for x in row)
    assert v[0]==[0,0,0] and v[16]==[0,-1,0] and v[32]==[0,0,0]
    assert v[1]==v[15] and v[17]==v[31] and v[33]==v[47]
    assert all(v[i][1]==0 for i in range(16)) and all(v[i][1]==-1 for i in range(16,32))
    assert all(v[i][1]==0 for i in range(32,48))
    assert [len(p['pairs']) for p in geo['lists']]==[16,16,30,16,30]
    assert geo['palettes']['ordinary']==[[0,0,0,110],[0,0,0,0],[0,0,0,22]]
    assert geo['palettes']['no_decay']==[[0,0,0,110]]*3
    assert max(p for primitive in geo['lists'] for p,c in primitive['pairs'])==47
    assert plan(3)==['top_inside','top_outside','bottom','side']
    assert plan(7,0)==['top','bottom','side']
    assert plan(3,0)==[]
    assert plan(3,110,1,.2,0)==['top_inside','bottom','side']
    assert plan(3,110,1,.2,1)==['top_outside','bottom','side']
    assert plan(3,255,1,1,.4)==['top_inside','bottom','side']  # b>=1 resets0.
    return 17
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--export-geometry',type=Path);a=p.parse_args()
    e=json.loads(MANIFEST.read_text());r=verify(a.iso,MANIFEST);geo=geometry(a.iso)
    r.update(gx_calls=check_graph(a.iso,e),positions=60,indexed_lists=5,derived_cases=derived(geo))
    if a.export_geometry:
        target=a.export_geometry.resolve();ignored=Path(__file__).resolve().parent.parent/'local-assets'
        if not(target.is_relative_to(Path('/tmp')) or target.is_relative_to(ignored)):raise ValueError('Original geometry export must stay in /tmp or ignored local-assets')
        target.parent.mkdir(parents=True,exist_ok=True);target.write_text(json.dumps(geo,indent=2)+'\n');r['ignored_geometry_export']=str(target)
    print(json.dumps(r,sort_keys=True))
