#!/usr/bin/env python3
"""Pin bounded source straight-path branch and classify authentic camera records."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO, inventory
from verify_overworld_roads import verify

MANIFEST=Path(__file__).with_name('overworld_camera_straight_path_evidence.json')
def eligible_records(raw):
    u=lambda a:struct.unpack_from('>I',raw,a)[0]
    r,v=u(0xec),u(0xf8); result=[]
    for j in range(u(0xc8)):
        a=r+j*184;start=v+u(a+0xa0)*12;n=u(a+0xa4)//2
        points=[struct.unpack_from('>3f',raw,start+k*12) for k in range(n)]
        rows=[struct.unpack_from('>3f',raw,start+(n+k)*12) for k in range(n)]
        tangent=(rows[0][0],rows[0][2])
        same=all((t[0],t[2])==tangent for t in rows)
        cardinal=tangent in [(1.,0.),(-1.,0.),(0.,1.),(0.,-1.)]
        aligned=all((p[0]-points[0][0])*tangent[1]-(p[2]-points[0][2])*tangent[0]==0 for p in points)
        forward=all((points[k+1][0]-points[k][0])*tangent[0]+(points[k+1][2]-points[k][2])*tangent[1]>0 for k in range(n-1))
        config=(u(a+0x40),u(a+0x44),u(a+0x84),u(a+0xb4))==(2,3,1,0)
        if same and cardinal and aligned and forward and config:
            result.append(dict(record_index=j,record_offset=a,name=raw[a:a+32].split(b'\0')[0].decode('ascii'),row_count=n))
    return result

def verify_straight(iso):
    result=verify(iso,MANIFEST); e=json.loads(MANIFEST.read_text())
    files={r['path']:r for r in inventory(iso)['files']}
    with Path(iso).open('rb') as disc:
        for row in e['camera_files']:
            rec=files[row['path']];disc.seek(rec['offset']);raw=disc.read(rec['size'])
            if len(raw)!=row['size'] or hashlib.sha256(raw).hexdigest()!=row['sha256']:
                raise ValueError('Source camera container mismatch: '+row['path'])
            if eligible_records(raw)!=row['eligible']:
                raise ValueError('Source bounded-path classification mismatch: '+row['path'])
    result['eligible_records']=sum(len(r['eligible']) for r in e['camera_files'])
    return result
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO)
    print(json.dumps(verify_straight(p.parse_args().iso),sort_keys=True))
