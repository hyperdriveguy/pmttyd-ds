#!/usr/bin/env python3
"""Unchanged battle-sequence strings/colors/listener constants, private only."""
import argparse,hashlib,json,struct
from pathlib import Path
from iso_assets import DEFAULT_ISO
from dol_event_closure import merge_segments
SPANS=((0x802c2abc,0x70),(0x804203b0,0x20))
def build(iso=DEFAULT_ISO,output='romfs'):
 segments=[];rows=[]
 with Path(iso).open('rb') as disc:
  if disc.read(8)!=b'G8ME01\0\0':raise ValueError('Original US disc0 revision0 required')
  disc.seek(0x420);dol=struct.unpack('>I',disc.read(4))[0];disc.seek(dol);header=disc.read(256)
  for address,size in SPANS:
   locations=[]
   for i in range(18):
    off,va,length=(struct.unpack_from('>I',header,b+4*i)[0] for b in (0,0x48,0x90))
    if off and va<=address and address+size<=va+length:locations.append(off+address-va)
   if len(locations)!=1:raise ValueError('Unmapped original battle-sequence data')
   disc.seek(dol+locations[0]);raw=disc.read(size)
   if len(raw)!=size:raise ValueError('Truncated original battle-sequence data')
   segments.append((address,raw));rows.append(dict(address=address,size=size,sha256=hashlib.sha256(raw).hexdigest()))
 packed=merge_segments(segments);out=Path(output);out.mkdir(parents=True,exist_ok=True)
 (out/'battle-sequence-entry.pmevt').write_bytes(packed)
 meta=dict(spans=rows,sha256=hashlib.sha256(packed).hexdigest(),policy='Unchanged original strings, colors and floats only; no PPC, BSS, saved mode, readiness or work-state fixtures.',license='Nintendo/Intelligent Systems original resources; local use only, no redistribution rights')
 (out/'battle-sequence-entry.json').write_text(json.dumps(meta,indent=2)+'\n');return meta
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--iso',default=DEFAULT_ISO);p.add_argument('--output',default='romfs');a=p.parse_args();build(a.iso,a.output)
