#!/usr/bin/env python3
"""Export original US revision tables from the read-only DOL."""
import argparse,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
from item_presentation_assets import TABLE,COUNT,STRIDE,TABLE_SHA256

def build(iso=DEFAULT_ISO,output='romfs'):
    with Path(iso).open('rb') as f:
        header=f.read(0x440)
        if header[:6]!=b'G8ME01' or header[7]:raise ValueError('US revision0 required')
        dol=struct.unpack_from('>I',header,0x420)[0];f.seek(dol);h=f.read(256);sections=[]
        for i in range(18):
            offset,address,size=(struct.unpack_from('>I',h,k+4*i)[0] for k in (0,0x48,0x90))
            if size:
                f.seek(dol+offset);data=f.read(size)
                if len(data)!=size:raise ValueError('Truncated DOL')
                sections.append((address,data))
    def read(address,size):
        spans=[data[address-base:address-base+size] for base,data in sections if base<=address and address+size<=base+len(data)]
        if len(spans)!=1:raise ValueError('Unmapped original table')
        return spans[0]
    table=read(TABLE,COUNT*STRIDE);party=read(0x80315380,64)
    if hashlib.sha256(table).hexdigest()!=TABLE_SHA256 or hashlib.sha256(party).hexdigest()!='7bca019f8482bbf58aeef1735bf62276e136728dc6c9f7666301549b67d3a14a':raise ValueError('Source tables changed')
    costs=bytes(table[i*STRIDE+0x1c] for i in range(COUNT))
    if hashlib.sha256(costs).hexdigest()!='1a4438d061e35924f4ace71ed70663ff5a3f03e42ae9047e9042955964fbd5cc':raise ValueError('Source costs changed')
    packed=struct.pack('<8sI',b'PMPRV001',COUNT)+costs+struct.pack('<32h',*struct.unpack('>32h',party))
    out=Path(output);out.mkdir(parents=True,exist_ok=True);target=out/'pouch-revision.bin'
    if target.resolve()==Path(iso).resolve():raise ValueError('Cannot overwrite ISO')
    if not target.exists() or target.read_bytes()!=packed:target.write_bytes(packed)
    (out/'pouch-revision.json').write_text(json.dumps(dict(source='US main.dol',item_table=TABLE,party_table=0x80315380,count=COUNT,conversion='Signed BP bytes unchanged; signed party halfwords big-to-little endian only',sha256=hashlib.sha256(packed).hexdigest(),license='Nintendo/Intelligent Systems original data; local use only, no redistribution rights'),indent=2)+'\n')

if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs');a=p.parse_args();build(a.iso,a.output)
