#!/usr/bin/env python3
"""Extract unchanged US ordinary/auxiliary party records, kinds1,8,13."""
import argparse,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
from field_map_npc_assets import Dol
RECORDS=[(1,0x80314410,'66223e5e99847e374a78c965305df7da505db3794f40503a107dbd8c5e2e0e3c'),
 (8,0x80314528,'0736c199924450fd0f59d4b42682e20fdd5086e0d8c6920f37fc3a4195aebb9d'),
 (13,0x803145f0,'8f4f2986bff8e23c7b9fd34a86ee6079a4672253779644f67188345792f3f6d9')]
def build(iso=DEFAULT_ISO,output='romfs'):
 root=Path(output);root.mkdir(parents=True,exist_ok=True);dol=Dol(iso);segments=[];rows=[]
 try:
  for kind,address,digest in RECORDS:
   data=dol.read(address,0x28)
   if hashlib.sha256(data).hexdigest()!=digest:raise ValueError('Unexpected original party record')
   segments.append((address,data));rows.append(dict(kind=kind,address=address,size=len(data),sha256=digest,words=list(struct.unpack('>10I',data))))
 finally:dol.close()
 offset=12+12*len(segments);out=bytearray(struct.pack('>8sI',b'PMEVT001',len(segments)))
 for address,data in segments:out+=struct.pack('>III',address,len(data),offset);offset+=len(data)
 out+=b''.join(data for _,data in segments)
 (root/'field-party-definitions.pmevt').write_bytes(out)
 meta=dict(schema=1,source='Original US GameCube revision0 DOL partyDataTbl803143e8',records=rows,pack_sha256=hashlib.sha256(out).hexdigest(),policy='Unchanged initialized records only; preserve kind13 as auxiliary, never canonicalize it to1',license='Nintendo/Intelligent Systems originals; local use only; no redistribution rights')
 (root/'field-party-definitions.json').write_text(json.dumps(meta,indent=2)+'\n');return meta
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs');a=p.parse_args();build(a.iso,a.output)
