#!/usr/bin/env python3
"""Pin camera4 source owners and authored Rogueport camera data/callers read-only."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO, inventory
from verify_overworld_roads import verify

MANIFEST=Path(__file__).with_name('overworld_camera_evidence.json')
def verify_camera(iso):
    result=verify(iso,MANIFEST)
    evidence=json.loads(MANIFEST.read_text())
    files={r['path']:r for r in inventory(iso)['files']}
    with Path(iso).open('rb') as disc:
        for row in evidence['camera_files']:
            source=files[row['path']]
            disc.seek(source['offset']); raw=disc.read(source['size'])
            if len(raw)!=row['size'] or hashlib.sha256(raw).hexdigest()!=row['sha256']:
                raise ValueError('Camera source container mismatch: '+row['path'])
            count=struct.unpack_from('>I',raw,0xc8)[0]
            pointer=struct.unpack_from('>I',raw,0xec)[0]
            names=[raw[pointer+i*184:pointer+i*184+64].split(b'\0')[0].decode('ascii') for i in range(count)]
            if (count,pointer,names)!=(row['record_count'],row['record_pointer'],row['record_names']):
                raise ValueError('Camera road metadata mismatch: '+row['path'])
    result.update(camera_files=len(evidence['camera_files']),camera_event_owners=len(evidence['camera_requests']),
                  camera_requests=sum(len(e['calls']) for e in evidence['camera_requests']))
    return result
if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    print(json.dumps(verify_camera(parser.parse_args().iso),sort_keys=True))
