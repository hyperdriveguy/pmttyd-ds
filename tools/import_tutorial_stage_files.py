#!/usr/bin/env python3
"""One-time original stage D/T transfer import into an empty editable path."""
import argparse
import hashlib
import json
from pathlib import Path
from iso_assets import DEFAULT_ISO,inventory
LICENSE='Nintendo/Intelligent Systems original game asset; no redistribution license granted'
SOURCE_HASHES={
 'm/bti_01/d':'213c62e758be15bc0731b3fa58558d1b2c565ea1c96e91111ea07b52cf751ca8',
 'm/bti_01/t':'18f1efc8f976ca5c82e5ed325c51432c7f2adc43edd25bd8ce86a60365ef7bc3',
 'm/stg_00_0/d':'0c5d16504d645f05d2569b017c6c755bdad8d913482619a6c25aca7d89542b82',
 'm/stg_00_0/t':'a0343ddf7199fb8ff1c19f701e1b4572f9d960a232737745d638be85dfb61767'}
CACHE_LINE_BYTES=32
def import_files(iso,destination):
    destination=Path(destination)
    if destination.exists() and (not destination.is_dir() or any(destination.iterdir())):
        raise ValueError('Original import requires an empty destination; preserve existing edits')
    info=inventory(iso)
    if info['disc_id']!='G8ME01':raise ValueError('Original US ISO required')
    outputs={};records=[]
    with Path(iso).open('rb') as disc:
        if disc.read(8)!=b'G8ME01\0\0':raise ValueError('Original US revision0 ISO required')
        for source,expected in SOURCE_HASHES.items():
            rows=[row for row in info['files']if row['path']==source]
            if len(rows)!=1:raise ValueError('Original FST entry missing or ambiguous')
            row=rows[0];logical=row['size'];transfer=(logical+CACHE_LINE_BYTES-1)&~(CACHE_LINE_BYTES-1)
            disc.seek(row['offset']);raw=disc.read(transfer)
            if len(raw)!=transfer or hashlib.sha256(raw[:logical]).hexdigest()!=expected:
                raise ValueError('Original source revision/payload changed')
            _,room,name=source.split('/');relative=room+'/'+name+'.bin';outputs[relative]=raw
            records.append(dict(file=relative,source=source,source_offset=row['offset'],logical_bytes=logical,
                transfer_bytes=transfer,source_sha256=expected,transfer_sha256=hashlib.sha256(raw).hexdigest(),
                trailing_hex=raw[logical:].hex(),license=LICENSE))
    destination.mkdir(parents=True,exist_ok=True)
    for name,raw in outputs.items():
        path=destination/name;path.parent.mkdir(parents=True,exist_ok=True);path.write_bytes(raw)
    (destination/'provenance.json').write_text(json.dumps(dict(schema=1,iso_id='G8ME01 revision0',files=records,
        conversion='Exact unrelocated original D/T plus actual ISO bytes through the next32-byte DVD boundary. No synthesized padding, geometry conversion, palette relocation or asset edit replacement.',
        license=LICENSE),indent=2)+'\n')
    (destination/'LICENSE.txt').write_text(LICENSE+'.\nOriginal rights retained; tracking a source grants no new license.\n')
    return records
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--destination',required=True,type=Path);args=p.parse_args();print(json.dumps(import_files(args.iso,args.destination),indent=2))
