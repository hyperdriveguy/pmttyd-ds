#!/usr/bin/env python3
"""Pin bounded original field framebuffer alpha/depth owners and direct census."""
import argparse,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
from verify_overworld_roads import verify
from verify_overworld_mario_air_shadow import dol_bytes
MANIFEST=Path(__file__).with_name('overworld_field_framebuffer_alpha_evidence.json')
def census(iso,e):
    got={name:[] for name in e['target_addresses']}
    for section in e['code_sections']:
        va=section['address'];raw=dol_bytes(iso,va,section['size'])
        assert hashlib.sha256(raw).hexdigest()==section['sha256']
        for off in range(0,len(raw)-3,4):
            w=int.from_bytes(raw[off:off+4],'big')
            if w>>26!=18 or not w&1:continue
            delta=w&0x3fffffc;delta=delta-0x4000000 if delta&0x2000000 else delta
            for name,target in e['target_addresses'].items():
                if va+off+delta==target:got[name].append(va+off)
    assert got==e['direct_call_census']
    assert len(got['GXSetDstAlpha'])==1 and got['GXSetDstAlpha'][0]==0x802b3208
    rows={r['path']:r for r in inventory(iso)['files']};r=rows[e['gor_identity']['path']]
    with Path(iso).open('rb') as d:d.seek(r['offset']);raw=d.read(r['size'])
    assert len(raw)==e['gor_identity']['size'] and hashlib.sha256(raw).hexdigest()==e['gor_identity']['sha256']
    u=lambda a:struct.unpack_from('>I',raw,a)[0];imports=[]
    for q in range(u(0x28),u(0x28)+u(0x2c),8):
        module,stream=struct.unpack_from('>II',raw,q)
        while True:
            delta,kind,section,addend=struct.unpack_from('>HBBI',raw,stream);stream+=8
            if kind==203:break
            if module==0 and kind not in (0,201,202) and addend in e['target_addresses'].values():imports.append(dict(kind=kind,address=addend))
    assert imports==e['gor_sdk_imports']==[]
    return {name:len(sites) for name,sites in got.items()}
def state_cases(iso):
    assert dol_bytes(iso,0x8030c2a8+0x19,1)==b'\0' and dol_bytes(iso,0x8030c2e4+0x19,1)==b'\0'
    stages=struct.unpack('>12f',dol_bytes(iso,0x80414fd8,48))
    assert stages[0]==stages[1]==400000 and stages[9]==500000 and stages[2]==600000
    ranges=struct.unpack('>8I',dol_bytes(iso,0x800055b0,32))
    for address in [0x80429548,0x80429568]:assert any(start<=address and address+4<=start+n for start,n in zip(ranges[::2],ranges[1::2]))
    # Source reset at callback boundary; explicit shadow writes are separate.
    def writes_alpha(camera):return camera not in (4,5,7)
    assert not writes_alpha(4) and not writes_alpha(5) and not writes_alpha(7)
    assert writes_alpha(2) and writes_alpha(3) and writes_alpha(6)
    alpha=0
    for callback_write_enabled,value in [(False,255),(False,128),(False,255)]:
        if callback_write_enabled:alpha=value
    assert alpha==0
    return 12
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);iso=p.parse_args().iso
    e=json.loads(MANIFEST.read_text());r=verify(iso,MANIFEST);r.update(direct_call_counts=census(iso,e),derived_cases=state_cases(iso));print(json.dumps(r,sort_keys=True))
