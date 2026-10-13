#!/usr/bin/env python3
"""Export original US item presentation keys/icons; no game code or prose rewrite."""
import argparse, hashlib, json, struct
from pathlib import Path
from iso_assets import DEFAULT_ISO

TABLE=0x803108a8
COUNT=340
STRIDE=40
TABLE_SHA256='cd9e52c1f0946f785301879d0e9560a5b87860abd648e9721973159291d0add9'

def source_rows(iso):
    with Path(iso).open('rb') as f:
        header=f.read(0x440)
        if header[:6]!=b'G8ME01' or header[7]!=0:raise ValueError('US G8ME01 revision0 required')
        dol=struct.unpack_from('>I',header,0x420)[0];f.seek(dol);h=f.read(256)
        sections=[]
        for i in range(18):
            off,va,n=(struct.unpack_from('>I',h,k+4*i)[0] for k in (0,0x48,0x90))
            if n:
                f.seek(dol+off);data=f.read(n)
                if len(data)!=n:raise ValueError('Truncated DOL section')
                sections.append((va,data))
    def read(a,n):
        matches=[data[a-va:a-va+n] for va,data in sections if va<=a and a+n<=va+len(data)]
        if len(matches)!=1:raise ValueError('Unmapped/ambiguous item data')
        return matches[0]
    def key(a):
        if not a:return b''
        for va,data in sections:
            if va<=a<va+len(data):
                end=data.find(b'\0',a-va,min(a-va+256,len(data)))
                if end<0:raise ValueError('Missing item key terminator')
                raw=data[a-va:end+1];raw[:-1].decode('ascii');return raw
        raise ValueError('Unmapped item message key')
    table=read(TABLE,COUNT*STRIDE)
    if hashlib.sha256(table).hexdigest()!=TABLE_SHA256:raise ValueError('Original item table changed')
    rows=[]
    for i in range(COUNT):
        name,description=struct.unpack_from('>II',table,i*STRIDE+4)
        icon=struct.unpack_from('>H',table,i*STRIDE+32)[0]
        rows.append((name,description,icon,key(name),key(description)))
    return rows

def pack(rows):
    if len(rows)!=COUNT:raise ValueError('Original item table extent changed')
    result=bytearray(struct.pack('<8s3I',b'PMITEM01',COUNT,TABLE,STRIDE))
    for name,description,icon,nk,dk in rows:
        for address,key in ((name,nk),(description,dk)):
            if (address==0)!=(len(key)==0) or len(key)>256 or (key and (key[-1]!=0 or b'\0' in key[:-1])):
                raise ValueError('Invalid item key boundary')
            key[:-1].decode('ascii')
        result+=struct.pack('<2I4H',name,description,icon,len(nk),len(dk),0)+nk+dk
    return bytes(result)

def build(iso=DEFAULT_ISO,output='romfs'):
    out=Path(output);out.mkdir(parents=True,exist_ok=True)
    target=out/'item-presentations.bin'
    if target.resolve()==Path(iso).resolve():raise ValueError('Cannot overwrite ISO')
    rows=source_rows(iso);data=pack(rows)
    if not target.exists() or target.read_bytes()!=data:target.write_bytes(data)
    meta=dict(source_disc='G8ME01 revision0',source='main.dol',table=TABLE,count=COUNT,stride=STRIDE,
              table_sha256=TABLE_SHA256,pack_sha256=hashlib.sha256(data).hexdigest(),
              conversion='Index-preserved name+4/description+8 ASCII keys and iconU16+32; original pointer identities/nulls retained; no dialogue or numeric item logic imported',
              license='Original Nintendo/Intelligent Systems resources; no redistribution rights',
              items=[dict(id=i,name_address=n,description_address=d,icon=icon,name_key=nk[:-1].decode('ascii') if nk else None,description_key=dk[:-1].decode('ascii') if dk else None) for i,(n,d,icon,nk,dk) in enumerate(rows)])
    (out/'item-presentations.json').write_text(json.dumps(meta,indent=2)+'\n')
    return meta

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs');a=p.parse_args();build(a.iso,a.output)
