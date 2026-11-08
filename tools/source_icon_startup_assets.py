#!/usr/bin/env python3
"""Read-only US IconInit immutable inputs and exact rounded BIN DVD suffix.

Raw runtime data is private, ignored build output; this exporter never touches
tracked editable images or the existing source-icons catalogue.
"""
import argparse
import hashlib
import json
import struct
from pathlib import Path
from iso_assets import DEFAULT_ISO, inventory

FORMAT_ADDRESS=0x802bf858
FORMAT_BYTES=24
FORMAT_SHA='b20463192bd4764d579f6b85f8f8119ebf8892396b7ccd6a9b28e05fec6ec59e'
DVD_TAIL_SHA='385fd512ffe41b128bd5ea66415eb4176f7123635c958b7e25b89d45fa40b360'
BIN_SHA='0be6b95a7e45e5d932641f24f8240a289a852889f1906d32be839046a680532b'

def read(iso):
    info=inventory(iso)
    matches=[f for f in info['files'] if f['path']=='icon.bin']
    if len(matches)!=1 or matches[0]['size']!=8174:
        raise ValueError('Exact US icon.bin required')
    entry=matches[0]
    with Path(iso).open('rb') as f:
        if f.read(8)!=b'G8ME01\0\0':
            raise ValueError('US revision-0 disc required')
        f.seek(0x420)
        dol,=struct.unpack('>I',f.read(4))
        f.seek(dol)
        header=f.read(256)
        offsets=[]
        for i in range(18):
            off,base,n=[struct.unpack_from('>I',header,t+4*i)[0] for t in (0,0x48,0x90)]
            if base<=FORMAT_ADDRESS and FORMAT_ADDRESS+FORMAT_BYTES<=base+n:
                offsets.append(dol+off+FORMAT_ADDRESS-base)
        if len(offsets)!=1:
            raise ValueError('Ambiguous initialized format span')
        f.seek(offsets[0])
        formats=f.read(FORMAT_BYTES)
        if hashlib.sha256(formats).hexdigest()!=FORMAT_SHA:
            raise ValueError('Original source icon formats changed')
        f.seek(entry['offset'])
        rounded=f.read((entry['size']+31)&~31)
        if len(rounded)!=8192 or hashlib.sha256(rounded[:entry['size']]).hexdigest()!=BIN_SHA:
            raise ValueError('Original BIN rounded read changed/truncated')
    tail=rounded[entry['size']:]
    if hashlib.sha256(tail).hexdigest()!=DVD_TAIL_SHA:
        raise ValueError('Original aligned DVD tail changed')
    blob=b'PMICST01'+formats+tail
    return blob,dict(source_disc='G8ME01 revision0',format_address=hex(FORMAT_ADDRESS),format_size=FORMAT_BYTES,
        format_sha256=FORMAT_SHA,source_path='icon.bin',source_offset=entry['offset'],source_size=entry['size'],
        rounded_read_size=len(rounded),dvd_tail_offset=entry['offset']+entry['size'],dvd_tail_size=len(tail),
        dvd_tail_sha256=hashlib.sha256(tail).hexdigest(),rounded_read_sha256=hashlib.sha256(rounded).hexdigest(),
        size=len(blob),sha256=hashlib.sha256(blob).hexdigest(),
        conversion='Unchanged source format bytes and actual physical-disc bytes beyond the FST logical EOF for original aligned DVDRead8192; no padding synthesized',
        license='Nintendo/Intelligent Systems original rights retained; private runtime pack, no redistribution rights')

def build(iso,output,verify_existing=False):
    out=Path(output)
    blob,provenance=read(iso)
    provenance['converter_sha256']=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
    if out.exists():
        if (verify_existing and out.is_dir() and {p.name for p in out.iterdir()}=={'startup.bin','sources.json'}
                and (out/'startup.bin').read_bytes()==blob
                and json.loads((out/'sources.json').read_text())==provenance):
            return provenance
        raise ValueError('New import requires a missing destination; existing data is never overwritten')
    out.parent.mkdir(parents=True,exist_ok=True)
    out.mkdir()
    (out/'startup.bin').write_bytes(blob)
    (out/'sources.json').write_text(json.dumps(provenance,indent=2)+'\n')
    return provenance
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--iso',default=DEFAULT_ISO)
    p.add_argument('--output',required=True)
    p.add_argument('--verify-existing',action='store_true',help='Read-only identity verification of an exact existing private cache; never overwrite')
    a=p.parse_args()
    print(json.dumps(build(a.iso,a.output,a.verify_existing),indent=2))
