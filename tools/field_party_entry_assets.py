#!/usr/bin/env python3
"""Unchanged original ordinary Goombella construction data; local ISO only."""
import argparse,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
from field_map_npc_assets import Dol
SPANS=[(0x802cb178,12,'e852d0ceaa6a4d8ae6859337b6e3d25a56b1bef05a492d34283afabbdfe4db36'),
 (0x802cb4dc,8,'80a8f9d441e0d1999be3be3b562f13cd35746ef19ce8d3e7e8f3a60a44f7962e'),
 (0x802cb52c,8,'e96220e7cc58bbf3b533809e3d3a3cce4840b106246754543fc46bb75c719602'),
 (0x803141c4,124,'d410866bc63f92da0e0a7b3205b5a2b1cf74977458ee8c8158cc6ea6267206da'),
 (0x80314744,60,'7a458d068ea5e3d0cb5ba6c6079b3443adf63402ff7ad2d5f39ae775f757e2c7'),
 (0x80421584,8,'2985be53db9fc5414ed2f4f86d8675c2bb8cab4c8f627e16248070a2077e64fa')]
SPANS=sorted(SPANS+[(2150713876, 60, '7a458d068ea5e3d0cb5ba6c6079b3443adf63402ff7ad2d5f39ae775f757e2c7'), (2150412832, 12, '15ec7bf0b50732b49f8228e07d24365338f9e3ab994b00af08e5a3bffe55fd8b'), (2151813632, 4, 'de620abd6d3615746360c1d15ce3a56291236d37424124054a49d25e947ffc4d'), (2150412676, 12, 'e852d0ceaa6a4d8ae6859337b6e3d25a56b1bef05a492d34283afabbdfe4db36'), (2150412700, 12, 'e852d0ceaa6a4d8ae6859337b6e3d25a56b1bef05a492d34283afabbdfe4db36'), (2150412688, 12, '15ec7bf0b50732b49f8228e07d24365338f9e3ab994b00af08e5a3bffe55fd8b')])
def build(iso=DEFAULT_ISO,output='romfs'):
 root=Path(output);root.mkdir(parents=True,exist_ok=True);dol=Dol(iso);segments=[]
 try:
  for address,size,digest in SPANS:
   data=dol.read(address,size)
   if hashlib.sha256(data).hexdigest()!=digest:raise ValueError('Unexpected original party construction data')
   segments.append((address,data))
 finally:dol.close()
 offset=12+12*len(segments);out=bytearray(struct.pack('>8sI',b'PMEVT001',len(segments)))
 for address,data in segments:out+=struct.pack('>III',address,len(data),offset);offset+=len(data)
 out+=b''.join(data for _,data in segments);(root/'field-party-entry.pmevt').write_bytes(out)
 meta=dict(schema=1,source='Original US GameCube revision0 DOL',spans=[dict(address=a,size=n,sha256=h) for a,n,h in SPANS],pack_sha256=hashlib.sha256(out).hexdigest(),policy='Unchanged source bytes; runtime reads live shared VM mappings after first import',license='Nintendo/Intelligent Systems originals; local use only; no redistribution rights')
 (root/'field-party-entry.json').write_text(json.dumps(meta,indent=2)+'\n');return meta
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs');a=p.parse_args();build(a.iso,a.output)
