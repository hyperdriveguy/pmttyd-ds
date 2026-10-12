#!/usr/bin/env python3
"""Pin authentic camera straight dispatch and duplicate-record classification."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO, inventory
from verify_overworld_roads import verify

MANIFEST=Path(__file__).with_name('overworld_camera_segment_dispatch_evidence.json')
EPSILON=0.009999999776482582

def classify(raw):
    u=lambda a:struct.unpack_from('>I',raw,a)[0]
    roads,vectors=u(0xec),u(0xf8);out=[]
    for j in range(u(0xc8)):
        a=roads+184*j;start=vectors+12*u(a+0xa0);n=u(a+0xa4)//2
        points=[struct.unpack_from('>3f',raw,start+12*k) for k in range(n)]
        rows=[struct.unpack_from('>3f',raw,start+12*(n+k)) for k in range(n)]
        collapsed=[];duplicates=[]
        for k in range(n-1):
            if abs(rows[k][0]-rows[k+1][0])<EPSILON and abs(rows[k][2]-rows[k+1][2])<EPSILON:
                collapsed.append(k)
            if (points[k][0],points[k][2])==(points[k+1][0],points[k+1][2]):
                if rows[k]!=rows[k+1]:raise ValueError('Duplicate fraction affects source row outputs')
                duplicates.append(k)
        tangent=(rows[0][0],rows[0][2])
        cardinal=tangent in [(1.,0.),(-1.,0.),(0.,1.),(0.,-1.)]
        constant=all((r[0],r[2])==tangent for r in rows)
        collinear=all((p[0]-points[0][0])*tangent[1]-(p[2]-points[0][2])*tangent[0]==0 for p in points)
        out.append(dict(index=j,offset=a,name=raw[a:a+32].split(b'\0')[0].decode('ascii'),collapsed_pairs=collapsed,duplicate_pairs=duplicates,bounded_cardinal_duplicate=bool(duplicates and cardinal and constant and collinear)))
    return out

def verify_dispatch(iso):
    result=verify(iso,MANIFEST);e=json.loads(MANIFEST.read_text());files={r['path']:r for r in inventory(iso)['files']};records=[]
    with Path(iso).open('rb') as disc:
        for row in e['camera_files']:
            source=files[row['path']];disc.seek(source['offset']);raw=disc.read(source['size'])
            if len(raw)!=row['size'] or hashlib.sha256(raw).hexdigest()!=row['sha256']:raise ValueError('Camera source mismatch')
            actual=classify(raw)
            if actual!=row['records']:raise ValueError('Camera segment classification mismatch')
            records+=actual
    result.update(records=len(records),collapsed_pairs=sum(len(r['collapsed_pairs']) for r in records),nonduplicate_records=sum(not r['duplicate_pairs'] for r in records),duplicate_records=sum(bool(r['duplicate_pairs']) for r in records),fraction_independent_duplicates=sum(len(r['duplicate_pairs']) for r in records),bounded_cardinal_duplicates=sum(r['bounded_cardinal_duplicate'] for r in records))
    if (result['records'],result['collapsed_pairs'],result['nonduplicate_records'],result['duplicate_records'],result['bounded_cardinal_duplicates'])!=(44,220,34,10,8):raise ValueError('Source dispatch inventory mismatch')
    return result
if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--iso',default=DEFAULT_ISO)
    print(json.dumps(verify_dispatch(parser.parse_args().iso),sort_keys=True))
