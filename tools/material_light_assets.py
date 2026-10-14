#!/usr/bin/env python3
"""Original battle light/fog records and fallback ambient; private local pack."""
import argparse, hashlib, json, struct
from pathlib import Path
from iso_assets import DEFAULT_ISO, extract
from battle_hit_assets import SOURCES

def decode(raw, expected):
    if hashlib.sha256(raw).hexdigest()!=expected: raise ValueError('Original battle map revision changed')
    u=lambda o:struct.unpack_from('>I',raw,o)[0]
    table=32+u(4)+4*u(8);names=table+8*u(12)
    def name(o):return raw[o:raw.index(b'\0',o)].decode('ascii')
    chunks={name(names+u(table+8*i+4)):32+u(table+8*i) for i in range(u(12))}
    light,fog=chunks['light_table'],chunks['fog_table']
    if u(light)!=0:raise ValueError('Authored battle lights require entry import')
    return raw[light:light+4]+raw[fog:fog+20],dict(light_offset=light,light_count=u(light),fog_offset=fog,fog_words=list(struct.unpack_from('>5I',raw,fog)))

def build(iso=DEFAULT_ISO):
    work=Path('local-assets/material-light');work.mkdir(parents=True,exist_ok=True);payload=bytearray(b'PMLTSC01');maps={}
    for name,digest in SOURCES.items():
        p=work/(name+'.d');extract(iso,'m/'+name+'/d',p);data,meta=decode(p.read_bytes(),digest);payload+=data
        maps[name]=dict(source='m/'+name+'/d',sha256=digest,**meta)
    with Path(iso).open('rb') as disc:
        disc.seek(0x420);base=struct.unpack('>I',disc.read(4))[0];disc.seek(base);header=disc.read(256)
        for i in range(18):
            off,va,size=(struct.unpack_from('>I',header,o+4*i)[0] for o in (0,0x48,0x90))
            if va<=0x8041f90c and 0x8041f914<=va+size:
                disc.seek(base+off+0x8041f90c-va);ambient=disc.read(8);break
        else:raise ValueError('Missing original ambient span')
    if ambient!=bytes.fromhex('707070ff707070ff'):raise ValueError('Original fallback ambient changed')
    payload+=ambient;assert len(payload)==64
    out=Path('romfs');out.mkdir(exist_ok=True);(out/'material-light.bin').write_bytes(payload)
    (out/'material-light.json').write_text(json.dumps(dict(maps=maps,ambient_address=0x8041f90c,ambient_sha256=hashlib.sha256(ambient).hexdigest(),license='Original Nintendo/Intelligent Systems resources; local use only, no redistribution rights',policy='Original bytes only.32 zeroed mutable light records are native lightInit state, not imported BSS.'),indent=2)+'\n')
    print('Original material light scene:',len(payload),'bytes')
if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);build(p.parse_args().iso)
