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
            words={str(a):struct.unpack_from('>I',raw,a)[0] for a in range(0xc4,0x10c,4)}
            if words!=row['header_words'] or struct.unpack_from('>I',raw,0)[0]!=len(raw):
                raise ValueError('Camera source header mismatch: '+row['path'])
            if (raw[4:68].split(b'\0')[0].decode('ascii'),raw[68:132].split(b'\0')[0].decode('ascii'))!=(row['magic'],row['version']):
                raise ValueError('Camera source identifiers mismatch: '+row['path'])
            lens=struct.unpack_from('>I',raw,0xe8)[0]
            if lens!=row['lens_pointer'] or hashlib.sha256(raw[lens:lens+12]).hexdigest()!=row['lens_sha256']:
                raise ValueError('Camera source lens pointee mismatch: '+row['path'])
            for span in row['road_path_ranges']:
                a=pointer+span['record_index']*184
                start,n=struct.unpack_from('>2I',raw,a+0xa0)
                if (start,n)!=(span['vector_start'],span['combined_vector_count']) or n%2 or start+n>words[str(0xd4)]:
                    raise ValueError('Camera source path range mismatch: '+row['path'])
            for count_field,pointer_field,stride in [(0xc8,0xec,184),(0xcc,0xf0,104),(0xd0,0xf4,8),(0xd4,0xf8,12),(0xd8,0xfc,12),(0xdc,0x100,4)]:
                a=words[str(pointer_field)];n=words[str(count_field)]
                if a>len(raw) or a+n*stride>len(raw):
                    raise ValueError('Camera source collection outside file: '+row['path'])
            if (count,pointer,names)!=(row['record_count'],row['record_pointer'],row['record_names']):
                raise ValueError('Camera road metadata mismatch: '+row['path'])
    result.update(camera_files=len(evidence['camera_files']),camera_event_owners=len(evidence['camera_requests']),
                  camera_requests=sum(len(e['calls']) for e in evidence['camera_requests']))
    return result
if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    print(json.dumps(verify_camera(parser.parse_args().iso),sort_keys=True))
