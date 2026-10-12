#!/usr/bin/env python3
"""Verify read-only US Rogueport road/case source evidence, never execute it."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from iso_assets import DEFAULT_ISO, inventory
from rel_event_inventory import read_bindings

MANIFEST = Path(__file__).with_name('overworld_road_evidence.json')

def verify(iso, manifest=MANIFEST):
    evidence=json.loads(Path(manifest).read_text())
    if evidence['schema'] != 1:
        raise ValueError('Unsupported road evidence schema')
    info=inventory(iso)
    records={r['path']:r for r in info['files']}
    sha=lambda data:hashlib.sha256(data).hexdigest()
    with Path(iso).open('rb') as disc:
        if disc.read(8) != b'G8ME01\0\0':
            raise ValueError('Expected US G8ME01 disc0 revision0')
        disc.seek(0x420)
        dol=struct.unpack('>I',disc.read(4))[0]
        disc.seek(dol)
        header=disc.read(256)
        def read_dol(address,size):
            positions=[]
            for i in range(18):
                off,va,length=(struct.unpack_from('>I',header,j+i*4)[0]
                               for j in (0,0x48,0x90))
                if va <= address and address+size <= va+length:
                    positions.append(dol+off+address-va)
            if len(positions)!=1:
                raise ValueError(f'Unmapped/ambiguous DOL span {address:#x}')
            disc.seek(positions[0])
            data=disc.read(size)
            if len(data)!=size:
                raise ValueError('Truncated DOL')
            return data
        def read_file(path):
            rec=records[path]
            disc.seek(rec['offset'])
            data=disc.read(rec['size'])
            if len(data)!=rec['size']:
                raise ValueError('Truncated disc file')
            return data
        for row in evidence['dol_spans']:
            if sha(read_dol(row['address'],row['size']))!=row['sha256']:
                raise ValueError('DOL body/event mismatch: '+row['name'])
        for address,word in evidence['dol_anchors'].items():
            if read_dol(int(address),len(word)//2).hex()!=word:
                raise ValueError('DOL anchor mismatch: '+address)
        raw=read_file('rel/gor.rel')
        local,imports,_=read_bindings(raw)
        for row in evidence['rel_spans']:
            a,n=row['offset'],row['size']
            if sha(raw[a:a+n])!=row['sha256']:
                raise ValueError('REL span mismatch: '+row['name']+'@'+row['namespace'])
        for address,expected in evidence['rel_bindings'].items():
            a=int(address)
            if expected['kind']=='dol':
                if imports.get(a)!=expected['address']:
                    raise ValueError('REL DOL binding mismatch: '+address)
            else:
                actual=local.get(a)
                if actual is None or (actual['section'],actual['addend'],actual['file_offset']) != (
                        expected['section'],expected['addend'],expected['offset']):
                    raise ValueError('REL self binding mismatch: '+address)
        node_count=0
        for row in evidence['maps']:
            raw=read_file(row['path'])
            if len(raw)!=row['size'] or sha(raw)!=row['sha256']:
                raise ValueError('Original map mismatch: '+row['path'])
            for node in row['nodes']:
                a=32+node['source_node']
                if sha(raw[a:a+104])!=node['node_header_sha256']:
                    raise ValueError('Map node mismatch: '+node['name'])
                ptr=32+struct.unpack_from('>I',raw,a)[0]
                name=raw[ptr:raw.index(b'\0',ptr)].decode('ascii')
                draw=32+struct.unpack_from('>I',raw,a+88)[0]
                attr=struct.unpack_from('>I',raw,draw+8)[0]
                if (name,attr,raw[draw+1])!=(node['name'],node['attributes'],node['cull']):
                    raise ValueError('Map node metadata mismatch: '+node['name'])
                if sha(raw[draw:draw+12])!=node['draw_header_sha256']:
                    raise ValueError('Map draw metadata mismatch: '+node['name'])
                node_count+=1
    return dict(scope=evidence['scope'],dol_spans=len(evidence['dol_spans']),
                anchors=len(evidence['dol_anchors']),rel_spans=len(evidence['rel_spans']),
                rel_bindings=len(evidence['rel_bindings']),maps=len(evidence['maps']),
                map_nodes=node_count)

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--iso',default=DEFAULT_ISO)
    parser.add_argument('--manifest',default=MANIFEST,type=Path)
    args=parser.parse_args()
    print(json.dumps(verify(args.iso,args.manifest),sort_keys=True))
