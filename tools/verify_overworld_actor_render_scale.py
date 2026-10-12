#!/usr/bin/env python3
"""Pin original actor draw scale and explicitly converter-derived idle metadata."""
import argparse
import hashlib
import json
from pathlib import Path
import tempfile
import numpy as np
from character_bake import geometry
from iso_assets import DEFAULT_ISO, inventory
from verify_overworld_roads import verify

MANIFEST=Path(__file__).with_name('overworld_actor_render_scale_evidence.json')
def pose_metadata(path,animation):
    triangles=geometry(path,animation,0)
    points=np.array([p for _,t in triangles for p,_ in t]);lo,hi=points.min(0),points.max(0)
    factor=118/max(hi[0]-lo[0],hi[1]-lo[1]);center=(lo[:2]+hi[:2])/2
    return dict(triangles=len(triangles),bounds=[lo.tolist(),hi.tolist()],native_height=float((hi[1]-lo[1])*.02),pixels_per_model_unit=float(factor),root_pixel=[float(64-center[0]*factor),float(64+center[1]*factor)],native_quad_side=float(128/factor*.02))

def verify_scale(iso):
    result=verify(iso,MANIFEST);e=json.loads(MANIFEST.read_text());files={r['path']:r for r in inventory(iso)['files']}
    with Path(iso).open('rb') as disc,tempfile.TemporaryDirectory(prefix='pm-actor-evidence-',dir='/tmp') as temp:
        for row in e['model_files']:
            source=files[row['path']];disc.seek(source['offset']);raw=disc.read(source['size'])
            if len(raw)!=row['size'] or hashlib.sha256(raw).hexdigest()!=row['sha256']:raise ValueError('Actor source mismatch')
            target=Path(temp)/Path(row['path']).name;target.write_bytes(raw)
            actual=pose_metadata(target,row['animation']);want=row['derived']
            for key in want:
                if not np.allclose(actual[key],want[key],rtol=0,atol=1e-8):raise ValueError('Converter-derived actor metadata mismatch: '+key)
    result['derived_frame0_models']=len(e['model_files'])
    return result
if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--iso',default=DEFAULT_ISO)
    print(json.dumps(verify_scale(parser.parse_args().iso),sort_keys=True))
